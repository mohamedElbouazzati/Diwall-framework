/*!
 * \file      board.c
 *
 * \brief     Target board general functions implementation
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
 *
 * \author    Marten Lootsma(TWTG) on behalf of Microchip/Atmel (c)2017
 */
#include "board-config.h"
#include "utilities.h"
#include "../system/delay.h"
#include "../system/gpio.h"
//#include "../system/adc.h"
#include "../system/spi.h"
//#include "../system/i2c.h"
//#include "../system/uart.h"
//#include "../system/timer.h"
//#include "../system/gps.h"
//#include "rtc-board.h"
#include "sx1276-board.h"
#include "board.h"
#include <stddef.h>
#include <stdio.h>
/*
 * MCU objects
 */
//Gpio_t Led1;
//Uart_t Uart1;
//I2c_t I2c;

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;

void BoardCriticalSectionBegin( uint32_t *mask )
{
    //NON GERÉE
    /* *mask = __get_PRIMASK( );
    __disable_irq( );*/
}

void BoardCriticalSectionEnd( uint32_t *mask )
{
    //NON GERÉE
 //   __set_PRIMASK( *mask );
}

void BoardInitPeriph( void )
{
    //LED 0
    //GpioInit( &Led1, LED_1, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
}

void BoardInitMcu( void )
{
    //init_mcu( );
    //NON GÉRÉ !
    //delay_init( SysTick );
    //hri_gclk_write_PCHCTRL_reg( GCLK, EIC_GCLK_ID, CONF_GCLK_EIC_SRC | ( 1 << GCLK_PCHCTRL_CHEN_Pos ) );
    //hri_mclk_set_APBAMASK_EIC_bit( MCLK );

    //RtcInit( );

    //NON GÉRÉ !
    //UartInit( &Uart1, UART_1, UART_TX, UART_RX );
    //UartConfig( &Uart1, RX_TX, 921600, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL );

    SpiInit();
    SX1276IoInit( );
    

    //NON GÉRÉ !
    //I2cInit( &I2c, I2C_1, I2C_SCL, I2C_SDA );

    McuInitialized = true;
    //SX1276IoDbgInit( );
    //SX1276IoTcxoInit( );
}

void BoardResetMcu( void )
{
    //CRITICAL_SECTION_BEGIN( );

    //Restart system
    //NVIC_SystemReset( );
}

void BoardDeInitMcu( void )
{
    SpiDeInit(0);// unique spi
}

uint32_t BoardGetRandomSeed( void )
{
    return 0;
}

void BoardGetUniqueId( uint8_t *id )
{
    // We don't have an ID, so use the one from Commissioning.h
}

uint8_t BoardGetBatteryLevel( void )
{
    return 0; //  Battery level [0: node is connected to an external power source ...
}

uint8_t GetBoardPowerSource( void )
{
    return USB_POWER;
}

void BoardLowPowerHandler( void )
{
    //__disable_irq( );
    /*!
     * If an interrupt has occurred after __disable_irq( ), it is kept pending 
     * and cortex will not enter low power anyway
     */

    // Call low power handling function.

    //__enable_irq( );
}


/*
 * Function to be used by stdout for printf etc
 */
int _write( int fd, const void *buf, size_t count )
{
    printf("%s",(uint8_t*)buf);//while( UartPutBuffer( &Uart2, ( uint8_t* )buf, ( uint16_t )count ) != 0 ){ };
    return count;
}

/*
 * Function to be used by stdin for scanf etc
 */
int _read( int fd, const void *buf, size_t count )
{
    size_t bytesRead = 0;
    bytesRead=scanf("%s",(uint8_t*)buf);//while( UartGetBuffer( &Uart2, ( uint8_t* )buf, count, ( uint16_t* )&bytesRead ) != 0 ){ };
    // Echo back the character
    printf("%s",(uint8_t*)buf);//while( UartPutBuffer( &Uart2, ( uint8_t* )buf, ( uint16_t )bytesRead ) != 0 ){ };
    return bytesRead;
}


