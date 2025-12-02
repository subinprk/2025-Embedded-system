#include "../include/twi.h"
#include "../include/uart.h"
#include <stdio.h>
#include <util/delay.h>

#define TWI_TIMEOUT 50000  // Timeout counter (증가: 10000 → 50000)

void TWI0_init(void)
{
    // SDA = PA2, SCL = PA3
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;

    // 400 kHz → MBAUD = (F_CPU/(2*SCL)) - 5
    // 20MHz → MBAUD = (20,000,000 / 800,000) - 5 = 20
    TWI0.MBAUD = 20;

    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
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
    USART1_sendString(buffer);
    
    uint8_t busstate = (TWI0.MSTATUS >> 0) & 0x03;
    USART1_sendString("Bus State: ");
    if (busstate == 0) USART1_sendString("UNKNOWN\r\n");
    else if (busstate == 1) USART1_sendString("IDLE\r\n");
    else if (busstate == 2) USART1_sendString("OWNER\r\n");
    else USART1_sendString("BUSY\r\n");
}

void TWI0_scan(void)
{
    USART1_sendString("\r\nScanning I2C bus...\r\n");
    char buffer[30];
    uint8_t found = 0;
    
    for (uint8_t addr = 1; addr < 128; addr++)
    {
        if (TWI0_start((addr << 1) | 0))  // Try write address
        {
            snprintf(buffer, sizeof(buffer), "Found device at 0x%02X\r\n", addr);
            USART1_sendString(buffer);
            found++;
        }
        TWI0_stop();
        _delay_ms(2);  // ← delay 증가 (1ms → 2ms)
    }
    
    if (found == 0) {
        USART1_sendString("No I2C devices found!\r\n");
    } else {
        snprintf(buffer, sizeof(buffer), "Total: %d device(s)\r\n", found);
        USART1_sendString(buffer);
    }
    
    TWI0_reset_bus();  // ← 스캔 후 버스 리셋
    USART1_sendString("Bus reset after scan\r\n");
}

uint8_t TWI0_start(uint8_t address)
{
    // Clear any pending flags before starting
    uint8_t status = TWI0.MSTATUS;
    if (status & TWI_ARBLOST_bm) {
        USART1_sendString("Bus arbitration lost\r\n");
        TWI0_reset_bus();
    }
    
    TWI0.MADDR = address;
    uint16_t timeout = TWI_TIMEOUT;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm)) && --timeout);
    if (timeout == 0) {
        char buf[50];
        snprintf(buf, sizeof(buf), "Start TO, MSTATUS=0x%02X\r\n", TWI0.MSTATUS);
        USART1_sendString(buf);
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
        USART1_sendString("Read ACK Timeout (RIF not set)\r\n");
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
        USART1_sendString(buf);
        return 0xFF;
    }
    
    uint8_t data = TWI0.MDATA;  // Read data first
    
    // Send NACK (no more bytes needed)
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
    
    return data;
}