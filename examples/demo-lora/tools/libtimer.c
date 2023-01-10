#include "libtimer.h"
#include "generated/csr.h"
#include <stdio.h>


/*********************************************************************
LOCAL DEFINITION
**********************************************************************/  
uint32_t TIMER_INT[NBTIMER]={TIMER0_INTERRUPT,TIMER1_INTERRUPT};
TIMERs_control TIMERs[NBTIMER]=
{
    {
        timer0_load_read,
        timer0_load_write,
        timer0_reload_read,
        timer0_reload_write,
        timer0_en_read,
        timer0_en_write,
        timer0_update_value_read,
        timer0_update_value_write,
        timer0_value_read,
        timer0_ev_status_zero_read,
        timer0_ev_pending_zero_read,
        timer0_ev_pending_zero_write,
        timer0_ev_enable_zero_read,
        timer0_ev_enable_zero_write
    },
    {
        timer1_load_read,
        timer1_load_write,
        timer1_reload_read,
        timer1_reload_write,
        timer1_en_read,
        timer1_en_write,
        timer1_update_value_read,
        timer1_update_value_write,
        timer1_value_read,
        timer1_ev_status_zero_read,
        timer1_ev_pending_zero_read,
        timer1_ev_pending_zero_write,
        timer1_ev_enable_zero_read,
        timer1_ev_enable_zero_write
    }
};

Time timer={0,0,.RtcProcess=RtcProcess};


/*********************************************************************
TIMER HARDWARE FUNCTION
**********************************************************************/  

/**
 * @brief This function is used to Config your TIMER.
 * 
 * @param loadReloadValue Value of counter in Load or Reload mode.
 * @param enableReaload   Active Reaload or Load. 
 *                        false : Load is set.
 *                        true  : Reload is set.
 * 
 * @param enableTimer     Active timer.
 *                        false : Timer off.
 *                        true  : Timer on.
 * 
 * @param enableUptadate  Active update of timer.
 *                        false : Timer update off.
 *                        true  : Timer update on.
 * 
 * @param enableInterrupt Active interrupt of timer.
 *                        false : Timer interrupt off.
 *                        true  : Timer interrupt on.
 * 
 * @param timerSelect     Select timer to configure. 
 *                        (TIMER|//<--TODO >|)
 */
void InitTimer(
 uint32_t loadReloadValue,bool enableReaload,
 bool enableTimer, bool enableUptadate,
 bool enableInterrupt, TimerSelect timerSelect)
{
    TIMERs[timerSelect].ev_pending_write(1); 
    // set value
    enableReaload   ?
    (TIMERs[timerSelect].load_write(0),
    TIMERs[timerSelect].reload_write(loadReloadValue)):
    (TIMERs[timerSelect].load_write(loadReloadValue),
    TIMERs[timerSelect].reload_write(0));

    //update
    if(enableUptadate)TIMERs[timerSelect].update_value_write(1);
    //start timer

    if(enableTimer)TIMERs[timerSelect].en_write(1);
    // interupt
    
    if(enableInterrupt)
    {
        irq_setmask(irq_getmask() | (1 << TIMER_INT[timerSelect]));
        TIMERs[timerSelect].ev_enable_write(1);
        
    }
    
}

void timer1_isr(void){
	timer1_ev_pending_write(1);
    //if(debugmode)printf("Timer has elapsed!\n");
	updateSoftTimerInterrupt();
	timer1_ev_enable_write(1);
}

void timer0_isr(void){
	timer0_ev_pending_write(1);
	//if(debugmode)printf("Timer has elapsed!\n");
	timer0_ev_enable_write(1);
}

/*********************************************************************
TIMER SOFTWARE FUNCTION
**********************************************************************/  

#define COMPARE_COUNT_MAX_VALUE ( uint32_t )( -1 )

/**
 * @brief this function is used to update the time register, 
 *        every milisecond. 
 */
void updateSoftTimerInterrupt(void)
{
    timer.milliseconds+=1;
    if(timer.milliseconds>=1000)
    {
        timer.milliseconds=0;
        timer.seconds+=1;
    }

    if(timer.TimerAlarmCallback==NULL)// RTC SANS FONCTION INTERRUPTION
    {
        timer.RtcProcess();
    }
    else                            // RTC AVEC FONCTION INTERRUPTION
    {
        timer.TimerAlarmCallback();
    }
}

/**
 * @brief This function is used to initialise the TIMER1.
 */
void HwTimerInit(void)
{
    timer.seconds=0;
    timer.milliseconds=0;
    InitTimer(COUNTER_TIMER,true,true,true,true,TIMER1);
}
/**
 * @brief This function is used to set Alarm call back function.
 * 
 * @param f function handler alarm.
 */
void HwTimerAlarmSetCallback(void (*f)(void))
{
    timer.TimerAlarmCallback=f;
}

/**
 * @brief This function is used to set Overflow call back function.
 * 
 * @param f Function handler overflow.
 */
void HwTimerOverflowSetCallback(void (* f)(void) )
{
    timer.RtcOverflowIrq=f;
}

/**
 * @brief This function is used to retrieve value of timer since first start.
 * 
 * @return uint64_t : Value of timer since first start.
 */
uint64_t HwTimerGetTime(void)
{
    return timer.seconds*1000+timer.milliseconds;
}

/**
 * @brief Loads the timeout in terms of ticks into the hardware
 * 
 * @param ticks Time value in terms of timer ticks
 * @return true : value set is right for hardware.
 */
bool HwTimerLoadAbsoluteTicks(uint32_t ticks)
{
    uint64_t current = HwTimerGetTime() ;
    if((ticks - current- 1) >= (COMPARE_COUNT_MAX_VALUE >> 1)) {
        // if difference is more than half of max assume timer has passed
        return false;
    }

    if((ticks - current) < 10) {
        // if too close the matching interrupt does not trigger, so handle same as passed
        return false;
    }
    return true;
}