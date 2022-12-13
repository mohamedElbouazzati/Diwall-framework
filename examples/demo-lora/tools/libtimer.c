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
void time0_init(void)
{
	int t;
	t = 2 * CONFIG_CLOCK_FREQUENCY;
    TimerSelect timer=TIMER0;

	/** Initialising timer0, for system functions **/
	TIMERs[timer].en_write(0);//timer0_en_write(0);
	TIMERs[timer].reload_write(t);
	TIMERs[timer].load_write(t);
	TIMERs[timer].en_write(1);	
}

void time1_init(void){
	int t;
	t = CONFIG_CLOCK_FREQUENCY;
    TimerSelect timer=TIMER1;
	/** Initialising timer1, for timing functions
	 *  It is initialised in periodic mode in order to 
	 * 	time longer than usual things
	 *  **/
	TIMERs[timer].en_write(0);//timer1_en_write(0);
	TIMERs[timer].load_write(0);//timer1_load_write(0);
	TIMERs[timer].reload_write(t);//timer1_reload_write(t);
	TIMERs[timer].en_write(1);//timer1_en_write(1);
	
	// Enabling the interrupt
	irq_setmask(irq_getmask() | (1 << TIMER1_INTERRUPT));
	TIMERs[timer].ev_pending_write(1);//timer1_ev_pending_write(timer1_ev_pending_read());
	TIMERs[timer].ev_enable_write(1);//time r1_ev_enable_write(1);
}

void RunTimerWithConfig(
 uint32_t loadReloadValue,bool enableReaload,
 bool enableTimer, bool enableUptadate,
 bool enableInterrupt, TimerSelect timerSelect)
{
    // set value
    enableReaload?
    TIMERs[timerSelect].reload_write(loadReloadValue):
    TIMERs[timerSelect].load_write(loadReloadValue);

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
	updateSoftTimerInterrupt();
    //printf("Timer has elapsed!\n");
	timer1_ev_enable_write(1);
}

void timer0_isr(void){
	timer0_ev_pending_write(1);
	//printf("Timer has elapsed!\n");
	timer0_ev_enable_write(1);
}

/*********************************************************************
TIMER SOFTWARE FUNCTION
**********************************************************************/  
/*--------------------------------------------------------------------
Human Readable Time	Seconds
----------------------------------------------------------------------
1 Minute                60          Seconds
1 Hour	                3600        Seconds
1 Day	                86400       Seconds
1 Week	                604800      Seconds
1 Month (30.44 days)	2629743     Seconds
1 Year (365.24 days)	31556926    Seconds
----------------------------------------------------------------------*/
#define COMPARE_COUNT_MAX_VALUE ( uint32_t )( -1 )

void updateSoftTimerInterrupt(void)
{
    timer.milliseconds+=1;
    if(timer.milliseconds>=1000)
    {
        timer.milliseconds=0;
        timer.seconds+=1;
        printf("\rtimer: %lld s -         ",timer.seconds);
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


void HwTimerInit(void)
{
    timer.seconds=0;
    timer.milliseconds=0;
    RunTimerWithConfig(COUNTER_TIMER,true,true,true,true,TIMER1);
}

void HwTimerAlarmSetCallback(void (*f)(void))
{
    timer.TimerAlarmCallback=f;
}

void HwTimerOverflowSetCallback(void (* f)(void) )
{
    timer.RtcOverflowIrq=f;
}

uint64_t HwTimerGetTime(void)
{
    return timer.seconds*1000+timer.milliseconds;
}

/**
* \brief Loads the timeout in terms of ticks into the hardware
* \ticks Time value in terms of timer ticks
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