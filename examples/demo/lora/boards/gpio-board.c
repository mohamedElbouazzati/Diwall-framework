/*!
 * \file      gpio-board.c
 *
 * \brief     Target board GPIO driver implementation
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

#include "utilities.h"
#include "../../tools/libdio.h"
#include "board-config.h"
#include "rtc-board.h"
#include "gpio-board.h"
#if defined( BOARD_IOE_EXT )
#include "gpio-ioe.h"
#endif

static Gpio_t *GpioIrq[16];

void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
    obj->pin = pin;
         // Sets initial output value
    if( pin > 5 )
    {
        return;
    }    
    printf("mode %d  -  pin %d \n",mode,obj->pin);
    InitGPIO(mode,false,false,true,obj->pin);

}

void GpioMcuSetContext( Gpio_t *obj, void* context )
{
    obj->Context = context;
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    SetInterrupt(irqMode,irqPriority,irqHandler, obj->pin);
    obj->IrqHandler=irqHandler;
    
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    RemoveInterrupt(obj->pin);
    obj->IrqHandler=((void *)0);
}

void GpioMcuToggle( Gpio_t *obj )
{
  //  Toggle(obj->pin );
    obj->pull^=1;
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
   return Read((uint8_t)obj->pin);
}
void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
   Write((uint8_t)obj->pin, value);
}
