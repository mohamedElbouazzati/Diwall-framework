
#include <generated/csr.h>
#include <generated/soc.h>
#include <irq.h>
#include <libbase/uart.h>
#include <stdio.h>

#include "isr.h"
#include "libisr.h"
#include "libtimer.h"



void isr(void)
{
    __attribute__((unused)) unsigned int irqs;

	irqs = irq_pending() & irq_getmask();

    #ifdef CSR_TIMER1_BASE
        #ifdef TIMER1_INTERRUPT
            if(irqs & (1 << TIMER1_INTERRUPT))
                timer1_isr();
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
                dio0_isr();
               // printf("interruption\n");
            }
        #endif
    #endif
    #ifdef CSR_DIO1_BASE
        #ifdef DIO1_INTERRUPT
            if(irqs & (1 << DIO1_INTERRUPT))
            {
                dio1_isr();
               // printf("interruption\n");
            }
        #endif
    #endif
     #ifdef CSR_DIO2_BASE
        #ifdef DIO2_INTERRUPT
            if(irqs & (1 << DIO2_INTERRUPT))
            {
                dio2_isr();
               // printf("interruption\n");
            }
        #endif
    #endif
         #ifdef CSR_DIO3_BASE
        #ifdef DIO3_INTERRUPT
            if(irqs & (1 << DIO3_INTERRUPT))
            {
                dio3_isr();
               // printf("interruption\n");
            }
        #endif
    #endif
    
}


