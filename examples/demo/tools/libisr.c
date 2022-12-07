#include "libisr.h"
#include "../lora/system/gpio.h"
#include <stdio.h>



GPIOs_control DIOs[6]=  
        {    
            {   
                IRQ_VERY_LOW_PRIORITY,
                dio0_oe_read,
                dio0_oe_write,
                dio0_ev_pending_read,
                dio0_ev_pending_write, 
                dio0_ev_enable_read, 
                dio0_ev_enable_write, 
                dio0_edge_read,
                dio0_edge_write,
                dio0_mode_read,
                dio0_mode_write,
                dio0_in_read,
                dio0_out_read,
                dio0_out_write,
                dio0_ev_status_read, 
                void0, 
                void1, 
                0,
                1<<DIO0_INTERRUPT,
            },

            {   
                IRQ_VERY_LOW_PRIORITY,
                dio1_oe_read,
                dio1_oe_write,
                dio1_ev_pending_read,
                dio1_ev_pending_write, 
                dio1_ev_enable_read, 
                dio1_ev_enable_write, 
                dio1_edge_read,
                dio1_edge_write,
                dio1_mode_read,
                dio1_mode_write,
                dio1_in_read,
                dio1_out_read,
                dio1_out_write,
                dio1_ev_status_read, 
                void0, 
                void1, 
                1,
                1<<DIO1_INTERRUPT,
            },

            {   
                IRQ_VERY_LOW_PRIORITY,
                dio2_oe_read,
                dio2_oe_write,
                dio2_ev_pending_read,
                dio2_ev_pending_write, 
                dio2_ev_enable_read, 
                dio2_ev_enable_write, 
                dio2_edge_read,
                dio2_edge_write,
                dio2_mode_read,
                dio2_mode_write,
                dio2_in_read,
                dio2_out_read,
                dio2_out_write,
                dio2_ev_status_read, 
                void0, 
                void1, 
                2,
                1<<DIO2_INTERRUPT,
            },

            {   
                IRQ_VERY_LOW_PRIORITY,
                dio3_oe_read,
                dio3_oe_write,
                dio3_ev_pending_read,
                dio3_ev_pending_write, 
                dio3_ev_enable_read, 
                dio3_ev_enable_write, 
                dio3_edge_read,
                dio3_edge_write,
                dio3_mode_read,
                dio3_mode_write,
                dio3_in_read,
                dio3_out_read,
                dio3_out_write,
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
void dio_isr(unsigned int irqs) //Active le pending dans l'ordre
{
        if(irqs & DIOs[0].INTERRUPT)
        {
            DIOs[0].pending_write(1);
            printf("interruption dio3\n");//DIOs[0].irqHandler();
            DIOs[0].enable_write(1);  
        }     
        if(irqs & DIOs[1].INTERRUPT)
        { 
            DIOs[1].pending_write(1);
            printf("interruption dio1\n");//DIOs[1].irqHandler();
            DIOs[1].enable_write(1);  
        }    
        if(irqs & DIOs[2].INTERRUPT)
        {
            DIOs[2].pending_write(1);
            printf("interruption dio2\n");//DIOs[2].irqHandler();
            DIOs[2].enable_write(1);  
        }   
        if(irqs & DIOs[3].INTERRUPT)
        {                             
            DIOs[3].pending_write(1);
            printf("interruption dio3\n");//DIOs[3].irqHandler();
            DIOs[3].enable_write(1);  
        }     
}


void dio_init(void)//ok
{   
    /*uint8_t tab[]={1<<DIO0_INTERRUPT,1<<DIO1_INTERRUPT,1<<DIO2_INTERRUPT,1<<DIO3_INTERRUPT};
    for(uint8_t i=0;i<4;i++)
    {
        irq_setmask(irq_getmask()|tab[i]);
        DIOs[i].enable_write(1);
    }*/
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
RESET FUNCTION
**********************************************************************/
void rst_init(void)
{
    rst_edge_write(1);
    irq_setmask(irq_getmask()|(1<<RST_INTERRUPT));
    rst_ev_enable_write(1);
}

void rst_isr(void)
{
    rst_ev_pending_write(1);
    printf("interruption rst detecté\r\n");
    rst_ev_enable_write(1);
}
   



void dio_set_IRQ(void)//ok
{
    for(uint8_t i =0; i<NBDIO ;i++)
        DIOs[i].mode_write(1);
}


void dio_erase_IRQ(uint8_t num)
{
    DIOs[num].mode_write(1);
}
void void0(void (*f))
{
    //rien faire
}
void void1(void)
{
    //rien faire
}
void permut(GPIOs_control *a,GPIOs_control *b)
{
    GPIOs_control save = *a;
    *a=*b;
    *b=save;
}
void SetOrderPriority(void)
{
    for(int8_t i =0;i<NBDIO-1;i++)
    {
        if(DIOs[i].irqPriority<DIOs[i+1].irqPriority)  // Si le dio a la position la plus prioritaire est inférieur à dio à la position la moins priotaire permuter
        {
            permut(&DIOs[i],&DIOs[i+1]);
            i=-1;
        }
    }
}


void Init(uint8_t PinNumber, PinModes io, PinTypes edge, uint8_t value)
{
    DIOs[PinNumber].edge_write(edge);
    DIOs[PinNumber].oe_write(io);
    if(DIOs[PinNumber].oe_read)
        DIOs[PinNumber].out_write(value);
}




void SetInterrupt(IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler, uint8_t num )
{
    DIOs[num].enable_write(1);// autoriser evenement
    
    //dio_edge_write(dio_edge_read()&~(num<<irqMode));// front montant   (set in structure )


    DIOs[num].irqPriority=irqPriority;// mettre la priorité
    DIOs[num].irqHandler=irqHandler;// mettre la fonction de handler
    SetOrderPriority();
}

void RemoveInterrupt(uint8_t num )
{
    DIOs[num].enable_write(0);// ne pas autoriser evenement
}
void SetContext( void* context,uint8_t num  )
{
    DIOs[num].context=context;
}
void Toggle(uint8_t pinNumber )
{
    DIOs[pinNumber].out_write(DIOs[pinNumber].out_read()^1);
}
uint32_t Read(uint8_t pinNumber )
{
  return DIOs[pinNumber].in_read();  
}
