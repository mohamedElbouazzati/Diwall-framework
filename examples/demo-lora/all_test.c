#include "libdio.h"
#include"libtimer.h"
#include "binary.h"
#include <stdio.h>

/**********TEST GPIO*************/
bool firstTestOutput = true;
void input_test(void);          //validé en tristate
void output_test(void);         //validé en tristate
void interruptIO_test(void);    //validé en tristate

/**********TEST SPI**************/
void test_spiInOut(void);

/*********TEST TIMER*************/
void timer_oneshot(void);       // validé
void timer_reloadshot(void);    // validé
void timer_interruption(void);  // validé décommenter printf à l'isr


/*********DESCRIPTION*************/
void input_test(void)  
{
    InitGPIO(false,false,false,false,DIO0);
    while (1)
    {
        printf("Une valeur du DIO a été détecté : %ld            \r",DIOs[0].in_read());
    }
}
void output_test(void)
{
    char *val;
    printf("write on/off ");
    printf("value = %s",val=input(val)); //écrire valeur
    InitGPIO(true,false,false,false,DIO0);

    if(val[0]=='o'&&val[1]=='n') //ALLUMER LED
    {
        printf("On active.\n");
        int compte=0;
        while (1)
        {
            if(compte==10) // rapport cyclique 1/10 pour diminuer la puissance consommé
            {
                DIOs[DIO0].out_write(1);
                compte=0;
            }       
            else
            {
                DIOs[DIO0].out_write(0);
                compte++;
            }  
        } 
    }
    else // ETEINDRE LED
    {
        printf("Off active.\n");
        DIOs[DIO0].out_write(0);  
    }   
}
void interruptIO_test(void)
{
    InitGPIO(false,false,false,true,DIO0);
    firstTestOutput=true;
}

void test_spiInOut(void)
{

}
void timer_oneshot(void)
{
    uint32_t one_second=COUNTER_TIMER*1000; //1secondes
    InitTimer(one_second,false,true,true,false,TIMER1);
    TIMERs[TIMER1].ev_pending_write(1);  
    while(!TIMERs[TIMER1].ev_pending_read())
    {
        printf("TIMER 1 Running        \r");
    }
    printf("\nTIMER 1 elapse        \n");
}

void timer_reloadshot(void)
{
    uint32_t one_second=COUNTER_TIMER*1000; //2secondes
    uint32_t cmpt=0;
    InitTimer(one_second,true,true,true,false,TIMER1);
    while(1)
    {
        TIMERs[TIMER1].ev_pending_write(1);   
        while (!TIMERs[TIMER1].ev_pending_read()) // attendre flag
        printf("TIMER 1 Running        \r");
    
        cmpt++;
        printf("\nTIMER 1 : %ld secondes elapse        \n",cmpt);
    }
}

void timer_interruption(void)
{
    // interrupt chaque seconde
    bool debugmode=true;
    InitTimer(COUNTER_TIMER*1000,true,true,true,true,TIMER1);
}
