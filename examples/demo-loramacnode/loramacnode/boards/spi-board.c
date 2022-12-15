/*!
 * \file      spi-board.c
 *
 * \brief     Target board SPI driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */

#include "spi-board.h"
#include "../system/delay.h"
#include "LitexLib/libspi.h"
#include <generated/csr.h>

//void SpiFormat( void)
//{
    /*DEFINE IN LITEX*/
//}
//void SpiFrequency( void )
//{
    /*DEFINE IN LITEX*/
//}

void SpiInit(void)
{
    InitSPI();
}
/*
void SpiDeInit(void )
{
 SpiInit();
}
*/
uint16_t SpiInOut(uint16_t outData )
{   
    loraspi_mosi_write(outData);
    loraspi_control_start_write(1);
    WaitXfer();
    outData = loraspi_miso_read();  
    return outData;

}
