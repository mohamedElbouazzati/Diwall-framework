#include "libtimer.h"
#include "generated/csr.h"
#include <stdio.h>

/*********************************************************************
LOCAL DEFINITION
**********************************************************************/  
uint32_t TIMER_INT[NBTIMER]={TIMER0_INTERRUPT,TIMER1_INTERRUPT};
DateTime dateTime;
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
    dateTime.millisecond+=1;
    dateTimeSecond.millisecond+=1;
    if(dateTime.millisecond>=1000||dateTimeSecond.millisecond>=1000)
    {

        if(dateTimeSecond.seconds==dateTime.seconds)
        RtcProcess();//alarme sans interrupti (option 1)
                     // alarm
        dateTime.millisecond=0;
        dateTimeSecond.millisecond=0;
        dateTime.seconds+=1;
        dateTimeSecond.seconds+=1;
        printf("\rdateTimeSecond: %lld s -  dateTime: %d min %d s                   "
        ,dateTimeSecond.seconds,dateTime.minutes,dateTime.seconds);
    }

    if(dateTime.seconds>=SECONDS_IN_1MINUTE)//minute
    {
        dateTime.seconds=0;
        dateTime.minutes+=1;
    }
    if(dateTime.minutes>=MINUTES_IN_1HOUR)//hours
    {
        dateTime.minutes=0;
        dateTime.hours+=1;
    }
    if(dateTime.hours>=HOURS_IN_1DAY)//days
    {
        dateTime.hours=0;
        dateTime.day+=1;
    }
    if(!dateTime.year%4)//LeapYear
    {
        if(dateTime.day>=DaysInMonthLeapYear[dateTime.month])//mouth
        {
            dateTime.day=0;
            dateTime.month+=1;
        }
        if (dateTime.month>=MONTH_IN_YEARS)//year
        {
             dateTime.month=0;
            dateTime.year+=1;
        }
    }
}

AlarmStates deadTimeAlarm(void)
{
    if(!alarm.enableAlarm)return ALARM_STOPPED;
    else
    {
        if((dateTimeSecond.seconds>alarm.setValueRingSeconds.seconds)||
        (dateTimeSecond.seconds==alarm.setValueRingSeconds.seconds&&dateTimeSecond.millisecond>alarm.setValueRingSeconds.millisecond))
        return ALARM_OVERFLOW;
       

        else if((dateTimeSecond.seconds==alarm.setValueRingSeconds.seconds
        &&dateTimeSecond.millisecond>=alarm.setValueRingSeconds.millisecond))
        return ALARM_IN_PHASE;

        else
        return ALARM_RUNNING;
    }
}

void setTimestamp(unsigned long seconds)
{
    dateTime.hours=(unsigned char)(seconds/3600)%24;
    dateTime.minutes=(unsigned char)(seconds/60)%60;
    dateTime.seconds=(unsigned char)seconds%60;
 
    dateTime.day=(unsigned char)((seconds/86400)%32);
    dateTime.month=(unsigned char)((seconds/2629743)%13);
    dateTime.year=(unsigned short)(seconds/31556926);
}

void HwTimerInit(void)
{
    dateTime.millisecond=0;
    dateTime.seconds=50;
    dateTime.minutes=0;
    dateTime.hours=0;
    dateTime.day=9;
    dateTime.month=12;
    dateTime.year=2022;

    dateTimeSecond.millisecond=0;
    dateTimeSecond.seconds=1670538510;

    RunTimerWithConfig(COUNTER_TIMER,true,true,true,true,TIMER1);
}



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
#define MINIMUM_DELAY_MS 20U
bool HwTimerLoadAbsoluteTicks(uint32_t ticks)
{

    //uint64_t current = alarm.setValueRingSeconds.millisecond+alarm.setValueRingSeconds.seconds*1000;
    //if((ticks > MINIMUM_DELAY_MS))
    return true;
}

unsigned char getHours(void) 
{
    return dateTime.hours;
}

unsigned char getMinutes(void) 
{
    return dateTime.minutes;
}

unsigned char getSeconds(void) 
{
    return dateTime.seconds;
}

unsigned char getMs(void) 
{
    return dateTime.millisecond;
}


