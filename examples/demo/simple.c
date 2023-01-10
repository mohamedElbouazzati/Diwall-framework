#include <stdio.h>
#include <string.h>
//#include <generated/csr.h>

#include "tools/libspi.h"
#include "tools/delai.h"
#include "tools/binary.h"

#include "lora/radio/sx1276Regs-LoRa.h"
#define ms_wait 100
void simple_test(void);
void simple_test(void)
{
    uint8_t reg = REG_LR_OPMODE; //REG_LR_PARAMP
    uint8_t reg_value= 0b1000;
    InitSPI();
    delay_ms(ms_wait);
    printf("initiale value :\n         ");
    println_bin(Read_SPI(reg));// afficher registre REG LR MODE 
    delay_ms(ms_wait);
    //Write_SPI(REG_LR_OPMODE,0); // Sleep mode
    delay_ms(ms_wait);
    printf("modify value :\n");
    while(1)
    {
        printf("         \r         ");
        //print_bin(WriteRead_SPI(reg,reg_value) );// afficher
        delay_ms(ms_wait);
    }
    
    printf("Test finish.\n");
}