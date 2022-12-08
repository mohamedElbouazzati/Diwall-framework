/*! -----------------------------------------------------------------------------------------------------
 * \file      libirq.h libirq.c
 *
 * \brief     delay implementation for Litex
 *
 * \copyright MadHighTech of source \ref LICENSE.
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
 *                                      ╚═╝ ┴ ┴─┘└┘                                                                              
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


#include "defint.h"



/*********************************************************************
TIMER0 and TIMER1 FUNCTION
**********************************************************************/
void time0_init(void);
void time1_init(void);
void timer1_isr(void);    



#ifdef __cplusplus
}
#endif    

#endif


