#include "libisr.h"
#include <stdio.h>
#include <generated/csr.h>

/*********************************************************************
TIMER FUNCTION
**********************************************************************/  
void time0_init(void)
{
	int t;
	t = 2 * CONFIG_CLOCK_FREQUENCY;

	/** Initialising timer0, for system functions **/
	timer0_en_write(0);
	timer0_reload_write(t);
	timer0_load_write(t);
	timer0_en_write(1);	
}

void time1_init(void){
	int t;
	t = CONFIG_CLOCK_FREQUENCY;

	/** Initialising timer1, for timing functions
	 *  It is initialised in periodic mode in order to 
	 * 	time longer than usual things
	 *  **/
	timer1_en_write(0);
	timer1_load_write(0);
	timer1_reload_write(t);
	timer1_en_write(1);
	
	// Enabling the interrupt
	timer1_ev_pending_write(timer1_ev_pending_read());
	timer1_ev_enable_write(1);
	irq_setmask(irq_getmask() | (1 << TIMER1_INTERRUPT));
}

void timer1_isr(void){
	timer1_ev_pending_write(1);
	//printf("Timer has elapsed!\n");
	timer1_ev_enable_write(1);
}








