/*
YOU CAN TEST CODE HERE
https://www.onlinegdb.com/online_c_compiler
*/
#include <stdio.h>

typedef struct DateTime DateTime;
typedef struct DateTimeSecond DateTimeSecond;
typedef struct Alarm Alarm;
typedef enum AlarmStates AlarmStates;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef enum bool bool;

enum bool
{
   false=0,
   true=1
};
struct DateTime
{
    uint16_t millisecond; // to ms
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;

    uint8_t day;
    uint8_t month;
    uint16_t year;
};
struct DateTimeSecond
{
    uint16_t millisecond;
    uint64_t seconds;
};
enum AlarmStates
{
    ALARM_STOPPED  = 0,
    ALARM_RUNNING  = 1,
    ALARM_IN_PHASE = 2,
    ALARM_OVERFLOW = 3,
};
struct Alarm
{
    bool enableAlarm;
    DateTimeSecond setValueRingSeconds;
    DateTime setValueRingDate;
    void (*RtcAlarmIrq)( void ); //alarm INTERRUPT
    void (*RtcOverflowIrq)( void ); // débortement INTERRUPT
};

AlarmStates deadTimeAlarm(void);
void test(AlarmStates val);

DateTime dateTime;
DateTimeSecond dateTimeSecond;
Alarm alarm={.enableAlarm=false};

int main()
{
    /*Change value here*/
    alarm.enableAlarm=true;
    dateTimeSecond.millisecond=1;
    dateTimeSecond.seconds=1;
    alarm.setValueRingSeconds.millisecond=1;
    alarm.setValueRingSeconds.seconds=1;
    /*Test function*/
    test(deadTimeAlarm());
    return 0;
}

void test(AlarmStates val)
{
    switch(val)
    {
            case ALARM_STOPPED  : printf("ALARM_STOPPED");  break;
            case ALARM_RUNNING  : printf("ALARM_RUNNING");  break;
            case ALARM_IN_PHASE : printf("ALARM_IN_PHASE"); break;
            case ALARM_OVERFLOW : printf("ALARM_OVERFLOW"); break;
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
