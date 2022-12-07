/*! -----------------------------------------------------------------------------------------------------
 * \file      libirq.h libirq.c
 *
 * \brief     delay implementation for Litex
 *
 *
 * \code
 *            _______              __                                            
 *           /       \            /  |                                           
 *           $$$$$$$  | ______   _$$ |_     ______                               
 *           $$ |__$$ |/      \ / $$   |   /      \                              
 *           $$    $$//$$$$$$  |$$$$$$/   /$$$$$$  |                             
 *           $$$$$$$/ $$ |  $$ |  $$ | __ $$ |  $$ |                             
 *           $$ |     $$ \__$$ |  $$ |/  |$$ \__$$ |                             
 *           $$ |     $$    $$/   $$  $$/ $$    $$/                              
 *           $$/       $$$$$$/     $$$$/   $$$$$$/                                                                                                                                                                                
 *                                   ╔═╗╔╦╗         
 *                                   ║   ║          
 *                                   ╚═╝╔═╗┬ ┬┬ ┬  ┬
 *                                      ╚═╗└┬┘│ └┐┌┘
 *                                      ╚═╝ ┴ ┴─┘└┘    opensource                                                                          
 * \endcode
 *
 * \author    TERRINE Christophe ( MadHighTech )
 * ------------------------------------------------------------------------------------------------------
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
#include "../lora/system/gpio.h"

#include "defint.h"

#define NBDIO 3
/*dio_edge_read, dio_edge_write, dio_mode_read, dio_mode_write, dio_in_read, dio_out_read, out_write,*/ //<-- INUTILE DANS LA STRUCTURE NON INDEPENDANT
typedef struct   
{
    IrqPriorities irqPriority; // 1
    
    uint32_t (*oe_read)(void);
    void(*oe_write)(uint32_t);

    uint32_t (*pending_read)(void); //2
    void (*pending_write)(uint32_t);//3
    
    uint32_t (*enable_read)(void);//4
    void (*enable_write)(uint32_t);//5
    
    uint32_t (*edge_read)(void);
    void (*edge_write)(uint32_t);
    
    uint32_t (*mode_read)(void);
    void (*mode_write)(uint32_t);
    
    uint32_t (*in_read)(void);

    uint32_t (*out_read)(void);
    void (*out_write)(uint32_t);
    

    uint32_t (*status_read)(void);    

    void (*irqHandler)(void *);
    void *context;
    uint8_t INTERRUPT;
    uint8_t pinNumber;
}GPIOs_control;



void testa(uint8_t i);
typedef unsigned char uint8_t;
/*********************************************************************
DIO FUNCTION
**********************************************************************/
void dio_isr(unsigned int irqs);
void dio_init(void);
/*********************************************************************
RESET FUNCTION
**********************************************************************/
void rst_isr(void);
void rst_init(void);
    
/*static void Dio0IrqHandler( void );
static void Dio1IrqHandler( void );
static void Dio2IrqHandler( void );
static void Dio3IrqHandler( void );
static void Dio4IrqHandler( void );
static void Dio5IrqHandler( void );*/

/*
IRQ_VERY_LOW_PRIORITY = 0,
IRQ_LOW_PRIORITY = 1,
IRQ_MEDIUM_PRIORITY = 2,
IRQ_HIGH_PRIORITY = 3,
IRQ_VERY_HIGH_PRIORITY = 4
*/


/*********************************************************************
CONTROL FUNCTION
**********************************************************************/

/*Fonction inactif*/
void void0(void (*f));
void void1(void);

/**
 * @brief Echange les données a 2 adresses
 * 
 * @param a 
 * @param b 
 */
void permut(GPIOs_control *a,GPIOs_control *b);
/**
 * @brief Set the Order Priority object
 * 
 */
void SetOrderPriority(void);

/**
 * @brief Mettre en place l'interruption
 * 
 * @param irqMode 
 * @param irqPriority 
 * @param irqHandler 
 * @param pinNumber 
 */
void SetInterrupt(IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler, uint8_t pinNumber );

/**
 * @brief Efface l'interruption
 * 
 * @param pinNumber 
 */
void RemoveInterrupt(uint8_t pinNumber );

/**
 * @brief Set the Context object
 * 
 * @param context 
 * @param pinNumber 
 */
void Toggle(uint8_t pinNumber );
void SetContext( void* context,uint8_t pinNumber  );
void Init(uint8_t PinNumber, PinModes io, PinTypes edge, uint8_t value);
uint32_t Read(uint8_t pinNumber );
#ifdef __cplusplus
}
#endif    

#endif


