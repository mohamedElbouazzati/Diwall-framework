#include "tools/delai.h"
#include "lora/system/spi.h"
#include "tools/binary.h"
#include <libbase/console.h>
#include <generated/csr.h>
#include "lora/radio/sx1276Regs-LoRa.h"
#include "tools/libspi.h"

// DIO0=TxDone 
// DIO1= detected the IRQ
// DIO3=CADDone
// DIO2=FhssChangeChannel

/*
POUR DEMARRER VERIFIER QUE LA SX127X :
    AI LA PIN RESET EN HAUTE IMPEDANCE(INPUT),
    AI LA PIN ENABLE DÉBRANCHÉ OU EN 3V3
*/
/*
    LES PINS DOIVENT ETRE CONFIGURABLE EN DOUBLE MODE (INPUT/OUTPUT) 
    PLUS PRÉCISEMENT POUR LE RESET.
*/
bool active=0;
void testspi(void);
void main_test(char * buff);
/******************************/
void main_test1(char * val);

void ecrire(uint8_t addr, uint8_t cmd);
void lire(uint8_t val);
void startSPI(void);

void stopSPI(void);
/*****************************/
/*----------------------------------------------*/
// FONCTION DE CMD BIOS
/*----------------------------------------------*/
void testspi(void)
{   
  //main_test("");
  main_test1("");
}

/*----------------------------------------------*/
// FONCTION DE PRINCIPAL POUR LES TESTS
/*----------------------------------------------*/
void main_test(char * buff)
{
    InitSPI();
    println_bin(Read_SPI(REG_LR_OPMODE));
    Write_SPI(REG_LR_OPMODE, 0b0001000);
    println_bin(Read_SPI(REG_LR_OPMODE));
}

void main_test1(char *val)
{

    //TEST1 
    InitSPI();
    println_bin(Read_SPI(REG_LR_OPMODE));
    println_bin(WriteRead_SPI(REG_LR_OPMODE, 0b0001000));
    

}


