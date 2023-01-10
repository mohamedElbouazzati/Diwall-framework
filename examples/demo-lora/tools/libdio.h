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



    /**
     * @brief List of interruption handler functions. 
     * 
    */
#include "../lora/system/gpio.h"
#include "../lora/boards/board-config.h"
#include "defint.h"
#include "../lora/radio/sx1276.h"
#include "generated/csr.h"

#define NBDIO 4

/*dio_edge_read, dio_edge_write, dio_mode_read, dio_mode_write, dio_in_read, dio_out_read, out_write,*/ //<-- INUTILE DANS LA STRUCTURE NON INDEPENDANT

/**
 * @brief Control structure of the GPIOS 
 * 
 */
typedef struct   
{
    //---------materiel----------
    IrqPriorities irqPriority;      // 1

    uint32_t (*oe_read)(void);      // 2
    void (*oe_write)(uint32_t);     // 3

    uint32_t (*in_read)(void);      // 4

    uint32_t (*out_read)(void);     // 5
    void (*out_write)(uint32_t);    // 6

    uint32_t (*mode_read)(void);    // 7
    void (*mode_write)(uint32_t);   // 8
    
    uint32_t (*edge_read)(void);    // 9
    void (*edge_write)(uint32_t);   // 10
    
    uint32_t (*pending_read)(void); // 11
    void (*pending_write)(uint32_t);// 12
    
    uint32_t (*enable_read)(void);  // 13
    void (*enable_write)(uint32_t); // 14
    
    uint32_t (*status_read)(void);  // 15

    //---------logiciel----------
    void (*irqHandler)(void *);     // 16
    void *context;                  // 17

    uint8_t INTERRUPT;              // 18
    uint8_t pinNumber;              // 19

}GPIOs_control;

typedef enum DIOSelect DIOSelect;
enum DIOSelect 
{
    DIO0 = 0,
    DIO1 = 1,
    DIO2 = 2,
    DIO3 = 3,
        //<--TODO HERE
};

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
void InitGPIO(bool setOutput, bool flagOnChangeState, bool setFallingEdge, bool setEnableInterrupt,uint8_t dioNumber);
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

extern GPIOs_control DIOs[NBDIO];
 

#endif