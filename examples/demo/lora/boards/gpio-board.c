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
 *
 * \author    Marten Lootsma(TWTG) on behalf of Microchip/Atmel (c)2017
 */


#include "gpio-board.h"

#define MODE_EDGE 0
#define MODE_CHANGE 1
#include <stdio.h>
#include <generated/csr.h>

static Gpio_t *GpioIrqDIO[6];
static Gpio_t *GpioIrqRST[1];

void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinStates value )
{
    if(obj->pin ==DIO_NAME)
    {
        if(mode == PIN_INPUT)
        {
            dio_oe_write(dio_oe_read()&~(1<<obj->number));
            obj->state=PIN_NO_PULL;
        }
        else if(mode == PIN_OUTPUT)
        {
            dio_oe_write(dio_oe_read()|(1<<obj->number));
            value ? dio_out_write(dio_out_read()|(1<<obj->number)) 
            : dio_out_write(dio_out_read()&~(1<<obj->number));
            obj->state=value;
        }
    }
    else if (pin == RST_NAME)
    {
        if(mode == PIN_INPUT)
        {
            rst_oe_write(rst_oe_read()&~(1<<obj->number));
            obj->state=PIN_NO_PULL;
        }
        else if(mode == PIN_OUTPUT)
        {
            rst_oe_write(rst_oe_read()|(1<<obj->number));
            value ? rst_out_write(rst_out_read()|(1<<obj->number)) 
            : rst_out_write(rst_out_read()&~(1<<obj->number));
            obj->state=value;
        }
    }         
                    
}

void GpioMcuSetContext( Gpio_t *obj, void* context )
{
    obj->Context = context;
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    //ext_irq_register( obj->pin, irqHandler );
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    //ext_irq_register( obj->pin, NULL );
}

/*
void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode,  GpioIrqHandler *irqHandler )//IrqPriorities irqPriority,
{
   
        uint32_t priority = 0;

        if( irqHandler == NULL )
        {
            return;
        }

        obj->IrqHandler = irqHandler;

        if( irqMode == IRQ_RISING_EDGE )
        {
           dio_mode_write(MODE_EDGE);
           dio_edge_write( dio_edge_read()&~(1<<obj->number)); //ACTIVE RISING EDGE
        }
        else if( irqMode == IRQ_FALLING_EDGE )
        {
            dio_mode_write(MODE_EDGE);
            dio_edge_write( dio_edge_read()|(1<<obj->number)); //ACTIVE FALLING EDGE
        }
        else
        {
            dio_mode_write(MODE_CHANGE);
        }
        
        // AUCUNE IDÉE POUR GERER LES PRIORITÉES.
            //switch( irqPriority )
            //{
            //case IRQ_VERY_LOW_PRIORITY:
            //case IRQ_LOW_PRIORITY:
            //    priority = 3;
            //    break;
            //case IRQ_MEDIUM_PRIORITY:
            //    priority = 2;
            //    break;
            //case IRQ_HIGH_PRIORITY:
            //    priority = 1;
            //    break;
            //case IRQ_VERY_HIGH_PRIORITY:
            //default:
            //    priority = 0;
            //    break;
            //}

        if(obj->pin==DIO_NAME )
        {
     
           obj->IrqHandler=&EXT_DIO_IRQHandler;
           
        }

        else if(obj->pin==RST_NAME)
        {
           
            obj->IrqHandler=&EXT_DIO_IRQHandler;
           
        }
}*/

/*void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    if( obj->pin == DIO_NAME )
        // Clear callback before changing pin mode
        GpioIrqDIO[obj->number] = NULL;
    
    else if(obj->pin == RST_NAME) 
        GpioIrqRST[obj->number] = NULL;

    else printf("ERROR: Not define PinNames in GPIO\n\r");
}*/


void GpioMcuWrite( Gpio_t *obj, PinStates value )
{
    if( obj->mode == PIN_OUTPUT)
    {
        if( obj == NULL )
        {
            printf("ERROR: GPIO is not detected ! \n\r");
            //return 0xFFFF;
        }
       
        value ? dio_out_write(dio_out_read()|(1<<obj->number)) : dio_out_write(dio_out_read()&~(1<<obj->number))  ;
        obj->state=value;
    }
    else
    {
        printf("ERROR: GPIO is input you cant write!\n\r");
        //return 0xFFFF;
    }
}

void GpioMcuToggle( Gpio_t *obj )
{
    if( obj->mode == PIN_OUTPUT)
    {
        if( obj == NULL )
        {
            printf("ERROR: GPIO is not detected ! \n\r");
            //return 0xFFFF;
        }

        dio_out_write(dio_out_read()^(1<<obj->number));
        obj->state^=1;
    }
    else
    {
        printf("ERROR: GPIO is input you cant write!\n\r");
        //return 0xFFFF;
    }
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
    if( obj->mode == PIN_INPUT)
    {
        if( obj == NULL )
        {
            printf("ERROR: GPIO is not detected ! \n\r");
            return 0xFFFF;
        }
        return dio_in_read() &~( 1<<obj->number);
    }
    else
    {
        printf("ERROR: GPIO is output you cant read!\n\r");
        return 0xFFFF;
    }
}

