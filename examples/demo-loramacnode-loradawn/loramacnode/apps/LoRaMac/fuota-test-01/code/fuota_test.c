
#include <generated/csr.h>


#include "libtimer.h"
#include "timer.h"
/**
 * @brief POUR TESTER IOTEST:
 *          il faut mettre en commentaire dio_isr() dans le fichier isr.c
 */
// static TimerEvent_t Led1Timer;
//  bool Led1TimerEvent = false;

// static TimerEvent_t Led2Timer;
//  bool Led2TimerEvent = false;

// static TimerEvent_t Led3Timer;
//  bool Led3TimerEvent = false;


// void OnLed1TimerEvent( void* context )
// {
//     printf("OnLed1TimerEvent\n");
//     leds_out_write(0b0000);
//      leds_out_write(0b0010);
//       TimerStart( &Led2Timer );
//     //Led1TimerEvent = true;
// }

// void OnLed2TimerEvent( void* context )
// {
//     //Led2TimerEvent = true;
//     leds_out_write(0b0000);
//     leds_out_write(0b0100);
//      TimerStart( &Led3Timer );
// }

// /*!
//  * \brief Function executed on Led 3 Timeout event
//  */
// void OnLed3TimerEvent( void* context )
// {
//     //Led3TimerEvent = true;
//     leds_out_write(0b0000);
//     leds_out_write(0b0001);
//      TimerStart( &Led1Timer );
// }

void fuota_test(void);

void fuota_test(void)
{   
    // HwTimerInit();
    // TimerInit( &Led1Timer, OnLed1TimerEvent );
    // #define VAL 1000
    // TimerSetValue( &Led1Timer, VAL );
    // TimerInit( &Led2Timer, OnLed2TimerEvent );
    // TimerSetValue( &Led2Timer, VAL );

    // TimerInit( &Led3Timer, OnLed3TimerEvent );
    // TimerSetValue( &Led3Timer, VAL );
    // printf("led 1 on \n ");
    // leds_out_write(0b0001);
    // TimerStart( &Led1Timer );
   
    //     while( 1 )
    // {
    //     if( Led1TimerEvent == true )
    //     {
    //         Led1TimerEvent = false;

    //         // Switch LED 1 OFF
    //         printf("led 1 off \n ");
    //         leds_out_write(0b0000);
    //         // Switch LED 2 ON
    //         printf("led 2 on \n ");
    //         leds_out_write(0b0010);
    //         TimerStart( &Led2Timer );
    //     }
        
    //     if( Led2TimerEvent == true )
    //     {
    //         Led2TimerEvent = false;

    //         // Switch LED 2 OFF
    //         printf("led 2 off \n ");
    //         leds_out_write(0b0000);
    //         // Switch LED 3 ON
    //         printf("led 3 on \n ");
    //         leds_out_write(0b0100);
    //         TimerStart( &Led3Timer );
    //     }

    //     if( Led3TimerEvent == true )
    //     {
    //         Led3TimerEvent = false;

    //         // Switch LED 3 OFF
    //         printf("led 3 off \n ");
    //         leds_out_write(0b0000);
    //         // Switch LED 1 ON
    //         printf("led 1 on \n ");
    //         leds_out_write(0b0001);
    //         TimerStart( &Led1Timer );
    //     }
    // }
}


