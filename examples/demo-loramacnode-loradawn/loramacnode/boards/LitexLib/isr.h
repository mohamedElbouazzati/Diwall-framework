/*! -----------------------------------------------------------------------------------------------------
 * \file      isr.h isr.c
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

#ifndef __ISR_H
#define __ISR_H

#ifdef __cplusplus
extern "C" {
#endif

#define DIO_SET_INTERRUPT

    /**
     * @brief This function handles all interrupt transfers, you can set your irq functions here.
     * 
     */
    void isr(void);

    /**
     * @brief List of interruption handler functions. 
     * 
     */
 
#ifdef __cplusplus
}
#endif

#endif
