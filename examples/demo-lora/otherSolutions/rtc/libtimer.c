#include "libtimer.h"
#include "generated/csr.h"
#include <stdio.h>


/*********************************************************************
LOCAL DEFINITION
**********************************************************************/  
uint32_t TIMER_INT[NBTIMER]={TIMER0_INTERRUPT,TIMER1_INTERRUPT};
DateTimeSecond dateTimeSecond;
Alarm alarm={.enableAlarm=false};
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
    printf("\n");
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
void updateSoftTimerInterrupt(void)
{
    dateTimeSecond.millisecond+=1;
    if(dateTimeSecond.millisecond>=1000)
    {
        dateTimeSecond.millisecond=0;
        dateTimeSecond.seconds+=1;
        printf("\rdateTimeSecond: %lld s -         ",dateTimeSecond.seconds);
    }
    //RtcProcess();//alarme (option 1) avec verification intégré plus lent
    if(HwTimerGetStatesAlarm()>=2)  
    {
        // l'alarme est à la seconde ou ?
        alarm.RtcAlarmIrq();// alarme (option 2)
    }
}

AlarmStates HwTimerGetStatesAlarm(void)
{
    if(!alarm.enableAlarm)return ALARM_STOPPED;
    else
    {
        if((dateTimeSecond.seconds>alarm.setValueRing.seconds)||
        (dateTimeSecond.seconds==alarm.setValueRing.seconds&&dateTimeSecond.millisecond>alarm.setValueRing.millisecond))
        return ALARM_OVERFLOW;
       

        else if((dateTimeSecond.seconds==alarm.setValueRing.seconds
        &&dateTimeSecond.millisecond>=alarm.setValueRing.millisecond))
        return ALARM_IN_PHASE;

        else
        return ALARM_RUNNING;
    }
}



void HwTimerInit(void)
{
    dateTimeSecond.millisecond=0;
    dateTimeSecond.seconds=1670538510;

    RunTimerWithConfig(COUNTER_TIMER,true,true,true,true,TIMER1);
}


/*return value in ms*/
uint64_t HwTimerGetTime(void) 
{
    return dateTimeSecond.millisecond+dateTimeSecond.seconds*1000;
}

void HwTimerAlarmSetCallback(void (*RtcAlarmIrq)( void ))
{
    alarm.RtcAlarmIrq=RtcAlarmIrq;
}

void HwTimerOverflowSetCallback(void (*RtcOverflowIrq)( void ))
{
    alarm.RtcOverflowIrq=RtcOverflowIrq;
}

/* Le minimum du timer a set en value*/
#define MINIMUM_DELAY_MS 5U
bool HwTimerLoadAbsoluteTicks(uint32_t ticks) //?
{
    if((ticks > MINIMUM_DELAY_MS))
    return true;
    else false;
}

void HwTimerSetStatesAlarm(AlarmStates alarmN)
{
    switch (alarmN)
    {
        case ALARM_RUNNING:     alarm.enableAlarm=true;     break;
        
        default:                alarm.enableAlarm=false;     break;
    }
}

uint64_t HwTimerGetAlarm(void) 
{
    return alarm.setValueRing.millisecond+alarm.setValueRing.seconds*1000;
}

/*
void setTimestamp(unsigned long seconds)
{
    dateTime.hours=(unsigned char)(seconds/3600)%24;
    dateTime.minutes=(unsigned char)(seconds/60)%60;
    dateTime.seconds=(unsigned char)seconds%60;
 
    dateTime.day=(unsigned char)((seconds/86400)%32);
    dateTime.month=(unsigned char)((seconds/2629743)%13);
    dateTime.year=(unsigned short)(seconds/31556926);
}*/
