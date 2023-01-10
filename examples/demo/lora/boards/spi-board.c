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
#include "libspi.h"
#include <stddef.h>


void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{
    /*DEFINE IN LITEX*/
}
void SpiFrequency( Spi_t *obj, uint32_t hz )
{
    /*DEFINE IN LITEX*/
}

void SpiInit( Spi_t *obj, SpiId_t spiId, PinNames mosi, PinNames miso, PinNames sclk, PinNames nss )
{
    InitSPI();
}

void SpiDeInit( Spi_t *obj )
{
    SpiInit(NULL,0,0,0,0,0);
}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
//uint32_t SpiInOut(uint32_t outData )
{
    /*
    // Wait for bus idle (ready to write)
    while( ( SERCOM_SPI_INTFLAG_DRE & hri_sercomspi_read_INTFLAG_reg( SERCOM4 ) ) == 0 )
    {

    }
    hri_sercomspi_clear_INTFLAG_reg( SERCOM4, SERCOM_SPI_INTFLAG_DRE );

    // Write byte
    hri_sercomspi_write_DATA_reg( SERCOM4, outData );

    // Wait for ready to read
    while( ( SERCOM_SPI_INTFLAG_RXC & hri_sercomspi_read_INTFLAG_reg( SERCOM4 ) ) == 0 )
    {

    }
    hri_sercomspi_clear_INTFLAG_reg( SERCOM4, SERCOM_SPI_INTFLAG_RXC );

    // Read byte
    outData = ( uint16_t )hri_sercomspi_read_DATA_reg( SERCOM4 );

    return outData;*/
    return WriteRead_SPI(outData);
}
