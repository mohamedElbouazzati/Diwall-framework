
#include <generated/csr.h>
#include <generated/soc.h>
#include <irq.h>
#include <libbase/uart.h>
#include <stdio.h>

#include "isr.h"
#include "libisr.h"



void isr(void)
{
    __attribute__((unused)) unsigned int irqs;

	irqs = irq_pending() & irq_getmask();

    #ifdef CSR_TIMER1_BASE
        #ifdef TIMER0_INTERRUPT
            if(irqs & (1 << TIMER0_INTERRUPT))
            {
                //timer0_isr();
            }
                
        #endif
    #endif

    #ifdef CSR_UART_BASE
        #ifndef UART_POLLING
            if(irqs & (1 << UART_INTERRUPT))
                
                uart_isr();
        #endif
    #endif
    
    #ifdef CSR_DIO0_BASE
        #ifdef DIO0_INTERRUPT
            if(irqs & (1 << DIO0_INTERRUPT))
            {
                //dio0_isr();
            }
                
        #endif
    #endif

    #ifdef CSR_DIO1_BASE
        #ifdef DIO1_INTERRUPT
            if(irqs & (1 << DIO1_INTERRUPT))
            {
                //dio1_isr();
            }
                
        #endif
    #endif

    #ifdef CSR_DIO2_BASE
        #ifdef DIO2_INTERRUPT
            if(irqs & (1 << DIO2_INTERRUPT))
            {
                //dio2_isr();
            }
                
        #endif
    #endif

    #ifdef CSR_DIO3_BASE
        #ifdef DIO3_INTERRUPT
            if(irqs & (1 << DIO3_INTERRUPT))
            {
                //dio3_isr();
            }
                
        #endif
    #endif

    /*
    #if defined(CSR_DIO0_BASE) || defined(CSR_DIO1_BASE) || defined(CSR_DIO2_BASE) || defined(CSR_DIO3_BASE)
        #if defined(DIO0_INTERRUPT) || defined(DIO1_INTERRUPT) || defined(DIO2_INTERRUPT) || defined(DIO3_INTERRUPT)
            if(irqs & ((1 << DIO0_INTERRUPT) | (1 << DIO1_INTERRUPT) | (1 << DIO2_INTERRUPT) | (1 << DIO3_INTERRUPT)))
    */
    /*#ifdef CSR_DIO_BASE
        #ifdef DIO_INTERRUPT
            {
                dio_isr(irqs);
            }
        #endif
    #endif
    */

    #ifdef CSR_RST_BASE
        #ifdef RST_INTERRUPT
            if(irqs & (1 << RST_INTERRUPT))
            {
                //printf("\r\ninterruption rst \r\n");
                rst_isr();
            }
                
        #endif
    #endif

    
}


