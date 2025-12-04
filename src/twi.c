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
    // For 16MHz @ 100kHz I2C: MBAUD = (16,000,000 / 200,000) - 5 = 75
    // For 16MHz @ 400kHz I2C: MBAUD = (16,000,000 / 800,000) - 5 = 15
    // Use 100kHz for more reliable communication
    TWI0.MBAUD = 75;  // 100kHz I2C @ 16MHz CPU

    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    
    _delay_ms(10);  // Let bus stabilize
}

void TWI0_reset_bus(void)
{
    // Force bus to idle state
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
    _delay_us(50);
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
    TWI_DEBUG_SEND_STRING("\r\nScanning I2C bus...\r\n");
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
    // Clear any pending flags before starting
    uint8_t status = TWI0.MSTATUS;
    if (status & TWI_ARBLOST_bm) {
        TWI_DEBUG_SEND_STRING("Bus arbitration lost\r\n");
        TWI0_reset_bus();
    }
    
    TWI0.MADDR = address;
    uint16_t timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)) && --timeout);
    if (timeout == 0) {
        char buf[50];
        snprintf(buf, sizeof(buf), "Start TO, MSTATUS=0x%02X\r\n", TWI0.MSTATUS);
        TWI_DEBUG_SEND_STRING(buf);
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
        TWI_DEBUG_SEND_STRING("Read ACK Timeout (RIF not set)\r\n");
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
        char buf[60];
        snprintf(buf, sizeof(buf), "Read NACK TO, MSTATUS=0x%02X, RIF=%d\r\n", 
                 TWI0.MSTATUS, !!(TWI0.MSTATUS & TWI_RIF_bm));
        TWI_DEBUG_SEND_STRING(buf);
        return 0xFF;
    }
    
    uint8_t data = TWI0.MDATA;  // Read data first
    
    // Send NACK (no more bytes needed)
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
    
    return data;
}