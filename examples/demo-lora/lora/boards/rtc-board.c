/*!
 * \file      rtc-board.c
 *
 * \brief     Target board RTC timer and low power modes management
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Marten Lootsma(TWTG) on behalf of Microchip/Atmel (c)2017
 */
//#include <hal_init.h>
//#include <hw_timer.h>
#include "board-config.h"
#include "board.h"
#include "timer.h"
#include "systime.h"
#include "gpio.h"

#include "rtc-board.h"

#define RTC_DEBUG_ENABLE                            1
#define RTC_DEBUG_DISABLE                           0

#define RTC_DEBUG_GPIO_STATE                        RTC_DEBUG_DISABLE
#define RTC_DEBUG_PRINTF_STATE                      RTC_DEBUG_DISABLE

#define MIN_ALARM_DELAY                             3 // in ticks

/*!
 * \brief Indicates if the RTC is already Initialized or not
 */
static bool RtcInitialized = false;
static volatile bool RtcTimeoutPendingInterrupt = false;
static volatile bool RtcTimeoutPendingPolling = false;

typedef enum AlarmStates_e
{
    ALARM_STOPPED = 0,
    ALARM_RUNNING = !ALARM_STOPPED
} AlarmStates_t;

/*!
 * RTC timer context 
 */
typedef struct
{
    uint32_t Time;  // Reference time
    uint32_t Delay; // Reference Timeout duration 
    AlarmStates_t AlarmState; // etat
}RtcTimerContext_t;

/*!
 * Keep the value of the RTC timer when the RTC alarm is set
 * Set with the \ref RtcSetTimerContext function
 * Value is kept as a Reference to calculate alarm
 */
static RtcTimerContext_t RtcTimerContext;

#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
Gpio_t DbgRtcPin0;
Gpio_t DbgRtcPin1;
#endif

/*!
 * Used to store the Seconds and SubSeconds.
 * 
 * WARNING: Temporary fix fix. Should use MCU NVM internal
 *          registers
 */
uint32_t RtcBkupRegisters[] = { 0, 0 };

/*!
 * \brief Callback for the hw_timer when alarm expired
 */
static void RtcAlarmIrq( void );

/*!
 * \brief Callback for the hw_timer when counter overflows
 */
static void RtcOverflowIrq( void );

void RtcInit( void )//ok
{   
    printf("RtcInit()\n");
    if( RtcInitialized == false )
    {
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
        GpioInit( &DbgRtcPin0, RTC_DBG_PIN_0, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
        GpioInit( &DbgRtcPin1, RTC_DBG_PIN_1, PIN_OUTPUT, PIN_PUSH_PULL, PIN_NO_PULL, 0 );
#endif
        // RTC timer
        HwTimerInit( );
        HwTimerAlarmSetCallback( RtcAlarmIrq );
        HwTimerOverflowSetCallback( RtcOverflowIrq );

        RtcTimerContext.AlarmState = ALARM_STOPPED;
        RtcSetTimerContext( );
        RtcInitialized = true;
    }
}

uint32_t RtcSetTimerContext( void )//ok
{
    //printf("RtcSetTimerContext()\n");
    RtcTimerContext.Time = ( uint32_t )HwTimerGetTime( );
    return ( uint32_t )RtcTimerContext.Time;
}

uint32_t RtcGetTimerContext( void )//ok
{
    //printf("RtcGetTimerContext()\n");
    return RtcTimerContext.Time;
}

uint32_t RtcGetMinimumTimeout( void )//ok
{
    return( MIN_ALARM_DELAY );
}

uint32_t RtcMs2Tick( TimerTime_t milliseconds )//ok
{
    return ( uint32_t )( ( ( ( uint64_t )milliseconds ) << 10 ) / 1000 );
}

TimerTime_t RtcTick2Ms( uint32_t tick )//ok
{
    uint32_t seconds = tick >> 10;

    tick = tick & 0x3FF;
    return ( ( seconds * 1000 ) + ( ( tick * 1000 ) >> 10 ) );
}

void RtcDelayMs( TimerTime_t milliseconds )//ok
{   
    printf("RtcDelayMs()\n");
    uint32_t delayTicks = 0;
    uint32_t refTicks = RtcGetTimerValue( );

    delayTicks = RtcMs2Tick( milliseconds );

    // Wait delay ms
    while( ( ( RtcGetTimerValue( ) - refTicks ) ) < delayTicks );
    /*
    {
        __NOP( );
    }*/
}

void RtcSetAlarm( uint32_t timeout )//ok
{
    printf("RtcSetAlarm()\n");
    RtcStartAlarm( timeout );
}

void RtcStopAlarm( void )//ok
{
    printf("RtcStopAlarm()\n");
    RtcTimerContext.AlarmState = ALARM_STOPPED;
}

void RtcStartAlarm( uint32_t timeout )//ok
{
    CRITICAL_SECTION_BEGIN( );
    printf("RtcStartAlarm()\n");
    RtcStopAlarm( );

    RtcTimerContext.Delay = timeout;//timeout*1000;
    printf("RtcTimerContext.Delay = %d\n",RtcTimerContext.Delay);

#if( RTC_DEBUG_PRINTF_STATE == RTC_DEBUG_ENABLE )
    printf( "TIMEOUT \t%010ld\t%010ld\n", RtcTimerContext.Time, RtcTimerContext.Delay );
#endif
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
    GpioWrite( &DbgRtcPin0, 0 );
    GpioWrite( &DbgRtcPin1, 0 );
#endif

    RtcTimeoutPendingInterrupt = true;
    RtcTimeoutPendingPolling = false;

    RtcTimerContext.AlarmState = ALARM_RUNNING;
    printf("ALARM RUNNING\n");
    if( HwTimerLoadAbsoluteTicks( RtcTimerContext.Time + RtcTimerContext.Delay ) == false )
    {
        // If timer already passed
        if( RtcTimeoutPendingInterrupt == true )
        {
            // And interrupt not handled, mark as polling
            RtcTimeoutPendingPolling = true;
            RtcTimeoutPendingInterrupt = false;
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
            GpioWrite( &DbgRtcPin0, 1 );
#endif
        }
    }
    CRITICAL_SECTION_END( );
}

uint32_t RtcGetTimerValue( void ) //ok
{
    //printf("RtcGetTimerValue()\n");
    return ( uint32_t )HwTimerGetTime( );
}

uint32_t RtcGetTimerElapsedTime( void ) //ok
{
    uint32_t val =( uint32_t)(HwTimerGetTime( ) - RtcTimerContext.Time);
    //printf("RtcGetTimerElapsedTime() = %d - %d =  %ld\n",HwTimerGetTime(),RtcTimerContext.Time,val);
    return val;
}

uint32_t RtcGetCalendarTime( uint16_t *milliseconds ) //ok
{
    printf("RtcGetCalendarTime()\n");
    uint32_t ticks = 0;

    uint32_t calendarValue = HwTimerGetTime( );

    uint32_t seconds = ( uint32_t )calendarValue >> 10;

    ticks =  ( uint32_t )calendarValue & 0x3FF;

    *milliseconds = RtcTick2Ms( ticks );

    return seconds;
}

void RtcBkupWrite( uint32_t data0, uint32_t data1 )//ok
{
    CRITICAL_SECTION_BEGIN( );
    RtcBkupRegisters[0] = data0;
    RtcBkupRegisters[1] = data1;
    CRITICAL_SECTION_END( );
}

void RtcBkupRead( uint32_t* data0, uint32_t* data1 ) //ok
{
    CRITICAL_SECTION_BEGIN( );
    *data0 = RtcBkupRegisters[0];
    *data1 = RtcBkupRegisters[1];
    CRITICAL_SECTION_END( );
}

void RtcProcess( void ) //ok
{
    CRITICAL_SECTION_BEGIN( );
    //printf("Rtc0\n");
    if( (  RtcTimerContext.AlarmState == ALARM_RUNNING ) )//&& ( RtcTimeoutPendingPolling == true ) )
    {
        //printf("Rtc1\n");
        //printf("RtcGetTimerElapsedTime = %d   RtcTimerContext.Delay=%d\n",RtcGetTimerElapsedTime( ),RtcTimerContext.Delay);
        if( RtcGetTimerElapsedTime( ) >= RtcTimerContext.Delay )
        {
            printf("ALARM RINGING BULUBULU\n");
            RtcTimerContext.AlarmState = ALARM_STOPPED;

            // Because of one shot the task will be removed after the callback
            RtcTimeoutPendingPolling = false;
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
            GpioWrite( &DbgRtcPin0, 0 );
            GpioWrite( &DbgRtcPin1, 1 );
#endif
            // NOTE: The handler should take less then 1 ms otherwise the clock shifts
            TimerIrqHandler( );
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
            GpioWrite( &DbgRtcPin1, 0 );
#endif
        }
    }
    CRITICAL_SECTION_END( );
}

TimerTime_t RtcTempCompensation( TimerTime_t period, float temperature )//ok
{
    return period;
}

static void RtcAlarmIrq( void )//ok
{
    RtcProcess();
    /*
    RtcTimerContext.AlarmState = ALARM_STOPPED;
    // Because of one shot the task will be removed after the callback
    RtcTimeoutPendingInterrupt = false;
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
    GpioWrite( &DbgRtcPin1, 1 );
#endif
    // NOTE: The handler should take less then 1 ms otherwise the clock shifts
    TimerIrqHandler( );
#if( RTC_DEBUG_GPIO_STATE == RTC_DEBUG_ENABLE )
    GpioWrite( &DbgRtcPin1, 0 );
#endif*/
}

static void RtcOverflowIrq( void )//ok
{
    //RtcTimerContext.Time += ( uint64_t )( 1 << 32 );
}
