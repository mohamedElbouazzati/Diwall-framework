
#include <generated/csr.h>
#include <generated/soc.h>
#include <irq.h>
#include <libbase/uart.h>
#include <stdio.h>

#include "lisr.h"
#include "libdio.h"
#include "libtimer.h"


#define PLIC_EXT_IRQ_BASE 1

void plic_init(void);
void plic_init(void)
{
	int i;

	// priorities for first 8 external interrupts
	for (i = 0; i < 8; i++)
		*((unsigned int *)PLIC_BASE + PLIC_EXT_IRQ_BASE + i) = 1;
	// enable first 8 external interrupts
	*((unsigned int *)PLIC_ENABLED) = 0xff << PLIC_EXT_IRQ_BASE;
	// set priority threshold to 0 (any priority > 0 triggers interrupt)
	*((unsigned int *)PLIC_THRSHLD) = 0;
}

void lisr(void)
{
	unsigned int claim;

	while ((claim = *((unsigned int *)PLIC_CLAIM))) {
		switch (claim - PLIC_EXT_IRQ_BASE) {
		case UART_INTERRUPT:
			uart_isr();
			break;
        case TIMER1_INTERRUPT:
			timer1_isr();
			break; 
        case TIMER0_INTERRUPT:
			timer0_isr();
			break;      
         case DIO0_INTERRUPT:
            dio0_isr();
			break;
        case DIO1_INTERRUPT:
            dio1_isr();
			break;
        case DIO2_INTERRUPT:
            dio2_isr();
			break;
        case DIO3_INTERRUPT:
            dio3_isr();
			break;                         
		default:
			printf("## no interrupt \n");

			break;
		}
		*((unsigned int *)PLIC_CLAIM) = claim;
	}
}

