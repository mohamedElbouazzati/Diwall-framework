/*! -----------------------------------------------------------------------------------------------------
 * \file      libtimer.h libtimer.c
 *
 * \brief     RTC TIMER implementation for Litex 
 *
 * \code                                                                                                                                                                          
 *          ___  _ __   ___ _ __                  
 *         / _ \| '_ \ / _ \ '_ \                 
 *        | (_) | |_) |  __/ | | |                
 *         \___/| .__/_\___|_| |_| _ _ __ ___ ___ 
 *              | | / __|/ _ \| | | | '__/ __/ _ \
 *              |_| \__ \ (_) | |_| | | | (_|  __/
 *                  |___/\___/ \__,_|_|  \___\___|
 *                                                                                                                      
 * \endcode
 *
 * \author    TERRINE Christophe ( www.ct-sylv.com )
 * \author    Mohamed El Bouazzati ()  
 *-------------------------------------------------------------------------------------------------------*/
#ifndef LIBTIMER_H
#define LIBTIMER_H

#include "defint.h"
#define NBTIMER 2

#define FREQ 1000 //1ms
#define COUNTER_TIMER CONFIG_CLOCK_FREQUENCY/FREQ

typedef struct TIMERs_control TIMERs_control;
typedef enum TimerSelect TimerSelect;
typedef struct Time Time;
/*********************************************************************
TIMER HARDWARE FUNCTION
**********************************************************************/  
/**
 * @brief Control structure of the TIMERS 
 * 
 */
struct TIMERs_control
{
    uint32_t (*load_read)(void);        //ONE SHOT
    void (*load_write)(uint32_t v);

    uint32_t (*reload_read)(void);      //RELOAD
    void (*reload_write)(uint32_t v);

    uint32_t (*en_read)(void);          // ENABLE TIMER
    void (*en_write)(uint32_t v);

    uint32_t (*update_value_read)(void);    // ENABLE UPDATE VALUE TIMER
    void (*update_value_write)(uint32_t v);

    uint32_t (*value_read)(void);           //TIMER VALUE

    uint32_t (*ev_status_read)(void);           // FLAG STATYS

    uint32_t (*ev_pending_read)(void);          // PENDING FLAG TIMER FINISH
    void (*ev_pending_write)(uint32_t v);       // <-- ERASE FLAG

    uint32_t (*ev_enable_read)(void);           // ENABLE INTERRUPTION
    void (*ev_enable_write)(uint32_t v);

};

/**
 * @brief Selection structure of TIMERS 
 * 
 */

enum TimerSelect 
{
    TIMER0 = 0,
    TIMER1 = 1,
    TIMER2 = 2,
};


void time0_init(void);
void time1_init(void);
void timer1_isr(void);   
void timer0_isr(void);   
void RunTimerWithConfig(
 uint32_t loadReloadValue,bool enableReaload,
 bool enableTimer, bool enableUptadate,
 bool enableInterrupt, TimerSelect timerSelect);

/*********************************************************************
TIMER SOFTWARE FUNCTION
**********************************************************************/  
struct Time
{
    uint16_t milliseconds;
    uint64_t seconds;
};


void updateSoftTimerInterrupt(void);

void HwTimerInit(void);

void HwTimerAlarmSetCallback(void (*f)(void));

void HwTimerOverflowSetCallback(void (* f)(void) );

uint64_t HwTimerGetTime(void);
/**
* \brief Loads the timeout in terms of ticks into the hardware
* \ticks Time value in terms of timer ticks
*/
bool HwTimerLoadAbsoluteTicks(uint32_t ticks);

#endif