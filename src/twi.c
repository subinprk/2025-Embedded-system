#include "../include/twi.h"
#include "../include/uart.h"
#include <stdio.h>
#include <util/delay.h>

#define TWI_TIMEOUT 50000  // Timeout counter (증가: 10000 → 50000)

// Debug UART wrapper macros based on TWI_DEBUG_UART setting
#if TWI_DEBUG_ENABLE
    #if TWI_DEBUG_UART == 1
        #define TWI_DEBUG_SEND_STRING(s)  USART1_sendString(s)
        #define TWI_DEBUG_SEND_CHAR(c)    USART1_sendChar(c)
    #elif TWI_DEBUG_UART == 2
        #define TWI_DEBUG_SEND_STRING(s)  USART2_sendString(s)
        #define TWI_DEBUG_SEND_CHAR(c)    USART2_sendChar(c)
    #else
        #error "TWI_DEBUG_UART must be 1 or 2"
    #endif
#else
    #define TWI_DEBUG_SEND_STRING(s)  ((void)0)
    #define TWI_DEBUG_SEND_CHAR(c)    ((void)0)
#endif

void TWI0_init(void)
{
    // SDA = PA2, SCL = PA3
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;

    // I2C clock calculation: MBAUD = (F_CPU / (2 * f_SCL)) - 5
    // For 16MHz CPU:
    //   @ 400kHz I2C: MBAUD = (16,000,000 / 800,000) - 5 = 15
    //   @ 200kHz I2C: MBAUD = (16,000,000 / 400,000) - 5 = 35
    //   @ 100kHz I2C: MBAUD = (16,000,000 / 200,000) - 5 = 75
    //   @  50kHz I2C: MBAUD = (16,000,000 / 100,000) - 5 = 155
    // MLX90640 supports up to 1MHz, try 400kHz for good speed/reliability balance
    TWI0.MBAUD = 15;  // 400kHz I2C @ 16MHz CPU

    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    
    _delay_ms(10);  // Let bus stabilize
}

void TWI0_reset_bus(void)
{
    // Disable TWI
    TWI0.MCTRLA = 0;
    
    // Set SCL and SDA as outputs temporarily
    PORTA.DIRSET = PIN2_bm | PIN3_bm;  // SDA=PA2, SCL=PA3
    
    // Toggle SCL up to 9 times to free stuck slave
    for (uint8_t i = 0; i < 9; i++) {
        PORTA.OUTCLR = PIN3_bm;  // SCL low
        _delay_us(20);
        PORTA.OUTSET = PIN3_bm;  // SCL high
        _delay_us(20);
    }
    
    // Generate STOP condition
    PORTA.OUTCLR = PIN2_bm;  // SDA low
    _delay_us(20);
    PORTA.OUTSET = PIN3_bm;  // SCL high
    _delay_us(20);
    PORTA.OUTSET = PIN2_bm;  // SDA high (STOP)
    _delay_us(20);
    
    // Return pins to input with pullup
    PORTA.DIRCLR = PIN2_bm | PIN3_bm;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
    
    // Re-enable TWI at 400kHz
    TWI0.MBAUD = 15;  // 400kHz
    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    
    _delay_us(100);
}

void TWI0_debug_status(void)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "MSTATUS: 0x%02X, MBAUD: %d\r\n", 
             TWI0.MSTATUS, TWI0.MBAUD);
    TWI_DEBUG_SEND_STRING(buffer);
    
    uint8_t busstate = (TWI0.MSTATUS >> 0) & 0x03;
    TWI_DEBUG_SEND_STRING("Bus State: ");
    if (busstate == 0) TWI_DEBUG_SEND_STRING("UNKNOWN\r\n");
    else if (busstate == 1) TWI_DEBUG_SEND_STRING("IDLE\r\n");
    else if (busstate == 2) TWI_DEBUG_SEND_STRING("OWNER\r\n");
    else TWI_DEBUG_SEND_STRING("BUSY\r\n");
}

void TWI0_scan(void)
{
    char buffer[30];
    uint8_t found = 0;
    
    for (uint8_t addr = 1; addr < 128; addr++)
    {
        if (TWI0_start((addr << 1) | 0))  // Try write address
        {
            snprintf(buffer, sizeof(buffer), "Found device at 0x%02X\r\n", addr);
            TWI_DEBUG_SEND_STRING(buffer);
            found++;
        }
        TWI0_stop();
        _delay_ms(2);  // ← delay 증가 (1ms → 2ms)
    }
    
    if (found == 0) {
        TWI_DEBUG_SEND_STRING("No I2C devices found!\r\n");
    } else {
        snprintf(buffer, sizeof(buffer), "Total: %d device(s)\r\n", found);
        TWI_DEBUG_SEND_STRING(buffer);
    }
    
    TWI0_reset_bus();  // ← 스캔 후 버스 리셋
    TWI_DEBUG_SEND_STRING("Bus reset after scan\r\n");
}

uint8_t TWI0_start(uint8_t address)
{
    // Check bus state first
    uint8_t busstate = TWI0.MSTATUS & TWI_BUSSTATE_gm;
    
    // If bus is not IDLE or OWNER, try to recover
    if (busstate == TWI_BUSSTATE_BUSY_gc || busstate == TWI_BUSSTATE_UNKNOWN_gc) {
        TWI0_reset_bus();
        _delay_ms(1);
    }
    
    // Clear any pending flags before starting
    if (TWI0.MSTATUS & (TWI_ARBLOST_bm | TWI_BUSERR_bm)) {
        TWI0.MSTATUS = TWI_ARBLOST_bm | TWI_BUSERR_bm;  // Clear by writing 1
        TWI0_reset_bus();
    }
    
    TWI0.MADDR = address;
    uint16_t timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)) && --timeout);
    if (timeout == 0) {
        TWI0_reset_bus();
        return 0;  // Timeout
    }
    
    if (TWI0.MSTATUS & TWI_RXACK_bm) {
        // NACK received
        return 0;
    }
    return 1;  // ACK received
}

void TWI0_stop(void)
{
    TWI0.MCTRLB = TWI_MCMD_STOP_gc;
    _delay_us(10);  // ← STOP 후 delay 추가
}

uint8_t TWI0_write(uint8_t data)
{
    TWI0.MDATA = data;
    uint16_t timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & TWI_WIF_bm) && --timeout);
    if (timeout == 0) return 0;  // Timeout
    return !(TWI0.MSTATUS & TWI_RXACK_bm);
}

uint8_t TWI0_read_ack(void)
{
    // Wait for RIF (data available)
    uint16_t timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & TWI_RIF_bm) && --timeout);
    if (timeout == 0) {
        return 0xFF;
    }
    
    uint8_t data = TWI0.MDATA;  // Read data first
    
    // Send ACK and request next byte
    TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
    
    return data;
}

uint8_t TWI0_read_nack(void)
{
    // Wait for RIF (data available)
    uint16_t timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & TWI_RIF_bm) && --timeout);
    if (timeout == 0) {
        return 0xFF;
    }
    
    uint8_t data = TWI0.MDATA;  // Read data first
    
    // Send NACK (no more bytes needed)
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
    
    return data;
}