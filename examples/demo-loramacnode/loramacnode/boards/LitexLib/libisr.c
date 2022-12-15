#include "libisr.h"
#include "../../system/gpio.h"
#include <stdio.h>
#include "libspi.h"
#include <generated/csr.h>


GPIOs_control DIOs[4]=  
        {    
            {   
                IRQ_VERY_LOW_PRIORITY,
                dio0_ev_pending_read,
                dio0_ev_pending_write, 
                dio0_ev_enable_read, 
                dio0_ev_enable_write, 
                dio0_edge_read,
                dio0_edge_write,
                dio0_mode_read,
                dio0_mode_write,
                dio0_in_read,
                dio0_ev_status_read, 
                void0, 
                void1, 
                0,
                1<<DIO0_INTERRUPT,
            },
            {   
                IRQ_VERY_LOW_PRIORITY,
                dio1_ev_pending_read,
                dio1_ev_pending_write, 
                dio1_ev_enable_read, 
                dio1_ev_enable_write, 
                dio1_edge_read,
                dio1_edge_write,
                dio1_mode_read,
                dio1_mode_write,
                dio1_in_read,
                dio1_ev_status_read, 
                void0, 
                void1, 
                1,
                1<<DIO1_INTERRUPT,
            },
            {   
                IRQ_VERY_LOW_PRIORITY,
                dio2_ev_pending_read,
                dio2_ev_pending_write, 
                dio2_ev_enable_read, 
                dio2_ev_enable_write, 
                dio2_edge_read,
                dio2_edge_write,
                dio2_mode_read,
                dio2_mode_write,
                dio2_in_read,
                //void0, 
                //void0, 
                dio2_ev_status_read, 
                void0, 
                void1, 
                2,
                1<<DIO2_INTERRUPT,
            },
            {   
                IRQ_VERY_LOW_PRIORITY,
                dio3_ev_pending_read,
                dio3_ev_pending_write, 
                dio3_ev_enable_read, 
                dio3_ev_enable_write, 
                dio3_edge_read,
                dio3_edge_write,
                dio3_mode_read,
                dio3_mode_write,
                dio3_in_read,
                dio3_ev_status_read, 
                void0, 
                void1, 
                3,
                1<<DIO3_INTERRUPT,
            },
            

        };

/*********************************************************************
DIO FUNCTION
**********************************************************************/        
void dio0_isr(void){
            DIOs[0].pending_write(1);
            printf("interruption dio0\n");//DIOs[0].irqHandler();
            DIOs[0].irqHandler(void0);
            DIOs[0].enable_write(1);             
}
void dio1_isr(void){ 
            DIOs[1].pending_write(1);
            printf("interruption dio1\n");//DIOs[1].irqHandler();
            DIOs[1].irqHandler(void0);
            DIOs[1].enable_write(1); }
void dio2_isr(void){
            DIOs[2].pending_write(1);
            printf("interruption dio2\n");//DIOs[2].irqHandler();
            DIOs[2].irqHandler(void0);
            DIOs[2].enable_write(1);  
}
void dio3_isr(void){            
            DIOs[3].pending_write(1);
            printf("interruption dio3\n");//DIOs[3].irqHandler();
            DIOs[3].irqHandler(void0);
            DIOs[3].enable_write(1);}

void dio_init(void)
{   
 irq_setmask(irq_getmask()|1<<DIO0_INTERRUPT);
 DIOs[0].enable_write(1);

 irq_setmask(irq_getmask()|1<<DIO1_INTERRUPT);
 DIOs[1].enable_write(1);

 irq_setmask(irq_getmask()|1<<DIO2_INTERRUPT);
 DIOs[2].enable_write(1);

 irq_setmask(irq_getmask()|1<<DIO3_INTERRUPT);
 DIOs[3].enable_write(1);

}
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





void void0(void (*f))
{
    //rien faire
}
void void1(void)
{
    //rien faire
}

void SetInterrupt(IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler, uint8_t num )
{
    
   DIOs[num].enable_write(1);// autoriser evenement
    switch (irqMode)
    {
    case IRQ_RISING_EDGE:
        DIOs[num].mode_write(0);
        DIOs[num].edge_write(0);
        printf("DIO %d done with mode %d and edge %d ", num,DIOs[num].mode_read(),DIOs[num].edge_read() );
        break;
    case IRQ_FALLING_EDGE:
        DIOs[num].mode_write(0);
        DIOs[num].edge_write(1);
        printf("DIO %d done with mode %d and edge %d ", num,DIOs[num].mode_read(),DIOs[num].edge_read() );
        break;
    case IRQ_RISING_FALLING_EDGE:
        DIOs[num].mode_write(1);
        printf("DIO %d done with mode %d and edge %d ", num,DIOs[num].mode_read(),DIOs[num].edge_read() );
        break;    
    default:
        break;
    }
   
    DIOs[num].irqPriority=irqPriority;// mettre la priorité
    DIOs[num].irqHandler=irqHandler;// mettre la fonction de handler
}
void RemoveInterrupt(uint8_t num )
{
    DIOs[num].enable_write(0);// ne pas autoriser evenement
}
uint32_t Read(uint8_t pinNumber )
{
  return DIOs[pinNumber].in_read();  
}
uint32_t Write(uint8_t pinNumber,uint32_t value )
{  
    // chip select SPI :
    // Reset Lora Module :       
    if((pinNumber==RADIO_NSS)){

        switch(value)
        {
        case 0 : 
        startSPI();
        break;
        case 1 : 
        stopSPI();
        break;

         }
    }

   }

