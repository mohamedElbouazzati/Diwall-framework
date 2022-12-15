/*! -----------------------------------------------------------------------------------------------------
 * \file      libirq.h libirq.c
 *
 * \brief     SPI implementation for Litex (loraspi)
 *
 * \code                                                                                                                                                                          
 *          ___  _ __   ___ _ __                  
 *         / _ \| '_ \ / _ \ '_ \                 
 *        | (_) | |_) |  __/ | | |                
 *         \___/| .__/_\___|_| |_| _ _ __ ___ ___ 
 *              | | / __|/ _ \| | | | '__/ __/ _ \
 *              |_| \__ \ (_) | |_| | | | (_|  __/
 *                  |___/\___/ \__,_|_|  \___\___|
 *                                                                                                                      
 * \endcode
 *
 * \author    TERRINE Christophe ( www.ct-sylv.com )
 * \author    Mohamed El Bouazzati ()  
 *-------------------------------------------------------------------------------------------------------
 * \brief TO USE THIS LIBRARY :
 *        - define |CONFIG_CPU_HAS_INTERRUPT|
 *        - define Your interruption like |TIMER0_SET_INTERRUPT|
 *        - at the end you can use your interruption handler functions like |void timer0_isr(void){ // TODO }|
 * 
 *        TO CREATE INTERRUPTION HANDLER IRQ:
 *        - follow this exemple statement on libirq.c :
 *                  #ifdef TIMER0_SET_INTERRUPT                #ifdef XXXXX_SET_INTERRUPT
 *                      if(irqs & (1 << TIMER0_INTERRUPT))          if(irqs & (1 << XXXXX_INTERRUPT))
 *                      timer0_isr();                               XXXXX_isr();
 *                  #endif                                      #endif
 * 
 *        - follow this exemple statement on libirq.h :
 *                  void timer0_isr(void);                  void xxxxx_isr(void);
 * 
 *        HELP ATTENTION !! : (isr functions) are special function reconize by RISCV implementation and autogenration files 
 * -----------------------------------------------------------------------------------------------------
 */

#ifndef __LIBIRQ_H
#define __LIBIRQ_H


#ifdef __cplusplus
extern "C" {
#endif
    /**
     * @brief List of interruption handler functions. 
     * 
    */
#include "gpio.h"
#include "board-config.h"
#include "defint.h"
#include "sx1276.h"

#define NBDIO 4


/*dio_edge_read, dio_edge_write, dio_mode_read, dio_mode_write, dio_in_read, dio_out_read, out_write,*/ //<-- INUTILE DANS LA STRUCTURE NON INDEPENDANT

/**
 * @brief Control structure of the GPIOS 
 * 
 */
typedef struct   
{
    IrqPriorities irqPriority; // 1
    uint32_t (*pending_read)(void); //2
    void (*pending_write)(uint32_t);//3
    
    uint32_t (*enable_read)(void);//4
    void (*enable_write)(uint32_t);//5
    
    uint32_t (*edge_read)(void);
    void (*edge_write)(uint32_t);
    
    uint32_t (*mode_read)(void);
    void (*mode_write)(uint32_t);
    
    uint32_t (*in_read)(void);
    

    uint32_t (*status_read)(void);    
    //DioIrqHandler irqhandler;
   void (*irqHandler)(void *);
   void *context;
    uint8_t INTERRUPT;
    uint8_t pinNumber;
}GPIOs_control;


typedef unsigned char uint8_t;
/*********************************************************************
DIO FUNCTION
**********************************************************************/
void dio0_isr(void);
void dio1_isr(void);
void dio2_isr(void);
void dio3_isr(void);
void dio_init(void);

/*********************************************************************
CONTROL FUNCTION
**********************************************************************/

/*Fonction inactif*/
void void0(void (*f));
void void1(void);

void SetInterrupt(IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler, uint8_t pinNumber );

/**
 * @brief Efface l'interruption
 * 
 * @param pinNumber 
 */
void RemoveInterrupt(uint8_t pinNumber );

uint32_t Read(uint8_t pinNumber );
void Write(uint8_t pinNumber,uint32_t value );

#ifdef __cplusplus
}
#endif    

#endif