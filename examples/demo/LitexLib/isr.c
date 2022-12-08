
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
    
   
    
}


