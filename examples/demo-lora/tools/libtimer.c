#include "libtimer.h"

#define TICKS_UNIT 1000//to ms

DateTime dateTime;

void config_timer()
{
    DateTime configDateTime;
    setDateTime(configDateTime);
    
}

void handlerTIMER(void)
{
    ticks+=TICKS_UNIT;/*Tick*/
    if(ticks>=1000)
    {
        ticks=0;
        dateTime.secondes+=1;
    }
}

void setDateTime(DateTime configDateTime)
{
    setTime(configDateTime.hours,configDateTime.minutes,configDateTime.secondes);
    setDate(configDateTime.day,configDateTime.mouth,configDateTime.year);
}

void setDate(unsigned char day,unsigned char mouth,unsigned short year)
{
    dateTime.ticks=0;
    dateTime.day=day;
    dateTime.mouth=configDateTime.mouth;
    dateTime.year=configDateTime.year;
}

void setTime(unsigned char hours, unsigned char minutes, unsigned char secondes)
{
    dateTime.hours=hours;
    dateTime.minutes=minutes;
    dateTime.secondes=secondes;
}

void setTimestamp(unsigned long secondes)
{
    dateTime.hours=(unsigned char)(secondes/3600)%24;
    dateTime.minutes=(unsigned char)(secondes/60)%60;
    dateTime.secondes=(unsigned char)secondes%60;
 
    dateTime.day=(unsigned char)((secondes/86400)%32);
    dateTime.mouth=(unsigned char)((secondes/2629743)%13);
    dateTime.year=(unsigned short)(secondes/31556926);
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
unsigned char getTicks(void) 
{
    return dateTime.ticks;
}
DateTime getDateTime(void) 
{
    return dateTime;
}

/*
#include <stdio.h>

int main()
{
    unsigned long utcSecondes=1670334518;
    
    printf("%da:",(unsigned short)(utcSecondes/31556926));
    printf("%dm:",(unsigned char)((utcSecondes/2629743)%13));
    printf("%dj:",(unsigned char)((utcSecondes/86400)%32));
    printf("%dh:",(unsigned char)((utcSecondes/3600)%24));

    printf("%dm:",(unsigned char)((utcSecondes/60)%60));

    printf("%ds",(unsigned char)(utcSecondes%60));
    return 0;
}
*/