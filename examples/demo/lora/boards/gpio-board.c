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
#include "../../tools/libisr.h"
#include "board-config.h"
#include "rtc-board.h"
#include "gpio-board.h"
#if defined( BOARD_IOE_EXT )
#include "gpio-ioe.h"
#endif

static Gpio_t *GpioIrq[16];

void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
    
}

void GpioMcuSetContext( Gpio_t *obj, void* context )
{
    obj->Context = context;
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    SetInterrupt(irqMode,irqPriority,irqHandler, obj->pinIndex);
    obj->IrqHandler=irqHandler;
    
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    RemoveInterrupt(obj->pinIndex);
    obj->IrqHandler=((void *)0);
}

void GpioMcuToggle( Gpio_t *obj )
{
    Toggle(obj->pinIndex );
    obj->pull^=1;
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
   return Read((uint8_t)obj->pinIndex);
}
