#include "libtimer.h"

///#define TICKS_UNIT 1000// unit of tick in (ms)
#define COUNTER_TIMER 10000
#define REG_CNTR_TIMER 0xFFFF-COUNTER_TIMER

/**
* \brief This function is used to set the callback when the hw timer
* expires.
* \param callback Callback to be registered
*/
void HwTimerAlarmSetCallback(static void (*RtcAlarmIrq)(void))
{
    
}

/**
* \brief This function is used to set the callback when the hw timer
* overflows.
* \param callback Callback to be registered
*/
void HwTimerOverflowSetCallback()
{

}

/**
* \brief Initializes the hw timer module
*/
void HwTimerInit(static void (*RtcOverflowIrq)( void ) )
{

}

/**
* \brief Gets the absolute time value
* \retval Absolute time in ticks
*/
HwTimerGetTime()
{

}

/**
* \brief Loads the timeout in terms of ticks into the hardware
* \ticks Time value in terms of timer ticks
*/
HwTimerLoadAbsoluteTicks()
{
    
}

void RTC_Handler(void)
{
    
}