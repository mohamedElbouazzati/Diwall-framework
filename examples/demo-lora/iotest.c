#include "tools/delai.h"
#include <stdio.h>
#include "tools/defint.h"
#include <libbase/console.h>
#include "tools/binary.h"
#include <system.h>
#include <generated/csr.h>
#include <irq.h>
#include "tools/isr.h"
#include "tools/libisr.h"
#include "tools/libtimer.h"

#include "timer.h"
/**
 * @brief POUR TESTER IOTEST:
 *          il faut mettre en commentaire dio_isr() dans le fichier isr.c
 */
static TimerEvent_t Led1Timer;
volatile bool Led1TimerEvent = false;

static TimerEvent_t Led2Timer;
volatile bool Led2TimerEvent = false;

static TimerEvent_t Led3Timer;
volatile bool Led3TimerEvent = false;


void OnLed1TimerEvent( void* context )
{
    printf("OnLed1TimerEvent\n");
    Led1TimerEvent = true;
}

void OnLed2TimerEvent( void* context )
{
    Led2TimerEvent = true;
}

/*!
 * \brief Function executed on Led 3 Timeout event
 */
void OnLed3TimerEvent( void* context )
{
    Led3TimerEvent = true;
}

void iotest(void);
//void INIT_ALL_GPIO(void);
//void MODE_GPIO(uint32_t num, uint8_t mode);
//uint32_t READ_GPIO(uint32_t num);
//void WRITE_GPIO(uint32_t num, bool value);


//void IRQTEST (void) __attribute__ ((interrupt ("machine")));

void iotest(void)
{   
  // HwTimerInit();//RunTimerWithConfig(COUNTER_TIMER,true,true,true,true,TIMER1);
    TimerInit( &Led1Timer, OnLed1TimerEvent );
    #define VAL 1
    TimerSetValue( &Led1Timer, VAL );
    TimerInit( &Led2Timer, OnLed2TimerEvent );
    TimerSetValue( &Led2Timer, VAL );

    TimerInit( &Led3Timer, OnLed3TimerEvent );
    TimerSetValue( &Led3Timer, VAL );
    printf("led 1 on \n ");
    TimerStart( &Led1Timer );
    printf("timer started\n");
        while( 1 )
    {
        if( Led1TimerEvent == true )
        {
            Led1TimerEvent = false;

            // Switch LED 1 OFF
             printf("led 1 off \n ");
            // Switch LED 2 ON
             printf("led 2 on \n ");
            TimerStart( &Led2Timer );
        }
        
        if( Led2TimerEvent == true )
        {
            Led2TimerEvent = false;

            // Switch LED 2 OFF
          printf("led 2 off \n ");
            // Switch LED 3 ON
         printf("led 3 on \n ");
            TimerStart( &Led3Timer );
        }

        if( Led3TimerEvent == true )
        {
            Led3TimerEvent = false;

            // Switch LED 3 OFF
            printf("led 3 off \n ");
            // Switch LED 1 ON
            printf("led 1 on \n ");
            TimerStart( &Led1Timer );
        }
    }

}


/*void INIT_ALL_GPIO(void)
{
    dio_edge_write(0);
    dio_out_write(0);
    dio_mode_write(0);
    dio_ev_enable_write(0);
    dio_ev_pending_write(0);
    dio_oe_write(0);
}

void MODE_GPIO(uint32_t num, uint8_t mode)
{
    mode ? dio_oe_write(dio_oe_read()|1<< num)
    :dio_oe_write(dio_oe_read()&~(1<< num));
}

uint32_t READ_GPIO(uint32_t num)
{
    return dio_in_read()&(1<<num);
}*/

/*void WRITE_GPIO(uint32_t num, bool value)
{
    value?dio_out_write(dio_out_read()|1<<num)
    :dio_out_write(dio_out_read()&~(1<<num));
}*/

