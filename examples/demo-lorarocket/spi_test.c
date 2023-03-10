#include <stdio.h>
#include <string.h>
#include <generated/csr.h>
#include "loramacnode/boards/board.h"
#include "loramacnode/system/gpio.h"
#include "loramacnode/system/delay.h"
#include "loramacnode/radio/radio.h"
#include "loramacnode/radio/sx1276.h"
#include "loramacnode/boards/LitexLib/libspi.h"
#include "loramacnode/boards/sx1276-board.h"
#include "loramacnode/radio/sx1276Regs-LoRa.h"

/**
 * @brief SPI test  SPITEST:
 *        Read all registers from LoRa Module and compare with Data sheet default values
 */
void spi_test(void);
void spi_test(void)
{    
 
    BoardInitMcu();
    SX1276Reset();
for (uint16_t addr = 0; addr < 0x3F; addr++) {
        uint8_t val = Read_SPI(addr);
        printf("Register 0x%02x = 0x%02x\r\n", addr, val);
    }
for (uint16_t addr = 0; addr < 0x3F; addr++) {
        uint8_t val = SX1276Read(addr);
        printf("SX Register 0x%02x = 0x%02x\r\n", addr, val);
    }
}

