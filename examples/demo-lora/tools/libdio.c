#include "libdio.h"
#include "../lora/system/gpio.h"
#include <stdio.h>
#include "libspi.h"
#include <generated/csr.h>
#include <generated/soc.h>
#include "isr.h"
#include <irq.h>
/*********************************************************************
LOCAL VARIABLES
**********************************************************************/  
uint32_t DIO_INT[NBDIO]={DIO0_INTERRUPT,DIO1_INTERRUPT,DIO2_INTERRUPT,DIO3_INTERRUPT};
GPIOs_control DIOs[NBDIO]=  
{    
    {   
        IRQ_VERY_LOW_PRIORITY,  //irqPriority       // 1
        dio0_oe_read,           //(*oe_read)        // 2
        dio0_oe_write,          //(*oe_write)       // 3
        dio0_in_read,           //(*in_read)        // 4
        dio0_out_read,          //(*out_read)       // 5
        dio0_out_write,         //(*out_write)      // 6
        dio0_mode_read,         //(*mode_read)      // 7
        dio0_mode_write,        //(*mode_write)     // 8
        dio0_edge_read,         //(*edge_read)      // 9
        dio0_edge_write,        //(*edge_write)     // 10
        dio0_ev_pending_read,   //(*pending_read)   // 11 
        dio0_ev_pending_write,  //(*pending_write)  // 12
        dio0_ev_enable_read,    //(*enable_read)    // 13
        dio0_ev_enable_write,   //(*enable_write)   // 14
        dio0_ev_status_read,    //(*status_read)    // 15
        void0,                  //(*irqHandler)     // 16
        void1,                  // *context;        // 17
        0,                      // INTERRUPT;       // 18
        1<<DIO0_INTERRUPT,      // pinNumber;       // 19
    },
    {   
        IRQ_VERY_LOW_PRIORITY,  //irqPriority
        dio1_oe_read,           //(*oe_read)
        dio1_oe_write,          //(*oe_write)
        dio1_in_read,           //(*in_read)
        dio1_out_read,          //(*out_read)
        dio1_out_write,         //(*out_write)
        dio1_mode_read,         //(*mode_read)
        dio1_mode_write,        //(*mode_write)
        dio1_edge_read,         //(*edge_read)
        dio1_edge_write,        //(*edge_write)
        dio1_ev_pending_read,   //(*pending_read)
        dio1_ev_pending_write,  //(*pending_write)
        dio1_ev_enable_read,    //(*enable_read)
        dio1_ev_enable_write,   //(*enable_write)
        dio1_ev_status_read,    //(*status_read)
        void0,                  //(*irqHandler)
        void1,                  // *context;
        1,                      // INTERRUPT;
        1<<DIO1_INTERRUPT,      // pinNumber;
    },
    {   
        IRQ_VERY_LOW_PRIORITY,  //irqPriority
        dio2_oe_read,           //(*oe_read)
        dio2_oe_write,          //(*oe_write)
        dio2_in_read,           //(*in_read)
        dio2_out_read,          //(*out_read)
        dio2_out_write,         //(*out_write)
        dio2_mode_read,         //(*mode_read)
        dio2_mode_write,        //(*mode_write)
        dio2_edge_read,         //(*edge_read)
        dio2_edge_write,        //(*edge_write)
        dio2_ev_pending_read,   //(*pending_read)
        dio2_ev_pending_write,  //(*pending_write)
        dio2_ev_enable_read,    //(*enable_read)
        dio2_ev_enable_write,   //(*enable_write)
        dio2_ev_status_read,    //(*status_read)
        void0,                  //(*irqHandler)
        void1,                  // *context;
        2,                      // INTERRUPT;
        1<<DIO2_INTERRUPT,      // pinNumber;
    },
    {   
        IRQ_VERY_LOW_PRIORITY,  //irqPriority
        dio3_oe_read,           //(*oe_read)
        dio3_oe_write,          //(*oe_write)
        dio3_in_read,           //(*in_read)
        dio3_out_read,          //(*out_read)
        dio3_out_write,         //(*out_write)
        dio3_mode_read,         //(*mode_read)
        dio3_mode_write,        //(*mode_write)
        dio3_edge_read,         //(*edge_read)
        dio3_edge_write,        //(*edge_write)
        dio3_ev_pending_read,   //(*pending_read)
        dio3_ev_pending_write,  //(*pending_write)
        dio3_ev_enable_read,    //(*enable_read)
        dio3_ev_enable_write,   //(*enable_write)
        dio3_ev_status_read,    //(*status_read)
        void0,                  //(*irqHandler)
        void1,                  // *context;
        3,                      // INTERRUPT;
        1<<DIO3_INTERRUPT,      // pinNumber;
    },
    

};


/*********************************************************************
DIO FUNCTION
**********************************************************************/        
void dio0_isr(void)
{
    DIOs[0].pending_write(1);
    printf("interruption dio0\n");//DIOs[0].irqHandler();
    DIOs[0].irqHandler(void0);
    DIOs[0].enable_write(1);             
}
void dio1_isr(void)
{ 
    DIOs[1].pending_write(1);
    printf("interruption dio1\n");//DIOs[1].irqHandler();
    DIOs[1].irqHandler(void0);
    DIOs[1].enable_write(1); 
}
void dio2_isr(void)
{
    DIOs[2].pending_write(1);
    printf("interruption dio2\n");//DIOs[2].irqHandler();
    DIOs[2].irqHandler(void0);
    DIOs[2].enable_write(1);  
}
void dio3_isr(void)
{            
    DIOs[3].pending_write(1);
    printf("interruption dio3\n");//DIOs[3].irqHandler();
    DIOs[3].irqHandler(void0);
    DIOs[3].enable_write(1);
}

void dio_init(void)
{   
    //irq_setmask(irq_getmask()|1<<DIO0_INTERRUPT);
    //DIOs[0].enable_write(1);
    InitGPIO(false,false,false,true,DIO0);
    printf("DIO0 init ");

    //irq_setmask(irq_getmask()|1<<DIO1_INTERRUPT);
    //DIOs[1].enable_write(1);
    InitGPIO(false,false,false,true,DIO1);
    printf("DIO1 init ");
    
    //irq_setmask(irq_getmask()|1<<DIO2_INTERRUPT);
    //DIOs[2].enable_write(1);
    InitGPIO(false,false,false,true,DIO2);
    printf("DIO2 init\n");

    //irq_setmask(irq_getmask()|1<<DIO3_INTERRUPT);
    //DIOs[3].enable_write(1);
    //InitGPIO(false,false,false,true,DIO3);
}
void InitGPIO(bool setOutput, bool flagOnChangeState, bool setFallingEdge, bool setEnableInterrupt,uint8_t dioNumber)
{
    setOutput? DIOs[dioNumber].oe_write(1) : DIOs[dioNumber].oe_write(0);
    flagOnChangeState? DIOs[dioNumber].mode_write(1) : DIOs[dioNumber].mode_write(0);
    setFallingEdge? DIOs[dioNumber].edge_write(1) : DIOs[dioNumber].edge_write(0);
    setEnableInterrupt?(irq_setmask(irq_getmask()|1<<DIO_INT[dioNumber]),DIOs[dioNumber].enable_write(1)):DIOs[dioNumber].enable_write(0);
}   


void SetInterrupt(IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler, uint8_t num )
{
   DIOs[num].enable_write(1);// autoriser evenement
    switch (irqMode)
    {
        case IRQ_RISING_EDGE:
            DIOs[num].mode_write(0);
            DIOs[num].edge_write(0);
            //printf("DIO %d done with mode %d and edge %d ", num,DIOs[num].mode_read(),DIOs[num].edge_read() );
            break;
        case IRQ_FALLING_EDGE:
            DIOs[num].mode_write(0);
            DIOs[num].edge_write(1);
            //printf("DIO %d done with mode %d and edge %d ", num,DIOs[num].mode_read(),DIOs[num].edge_read() );
            break;
        case IRQ_RISING_FALLING_EDGE:
            DIOs[num].mode_write(1);
            //printf("DIO %d done with mode %d and edge %d ", num,DIOs[num].mode_read(),DIOs[num].edge_read() );
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
void Write(uint8_t pinNumber,uint32_t value )
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




void void0(void (*f))
{
    //rien faire
}
void void1(void)
{
    //rien faire
}



