#include "libtimer.h"

#define TICKS_UNIT 1000// unit of tick in (ms)
#define COUNTER_TIMER 10000
#define REG_CNTR_TIMER 0xFFFF-COUNTER_TIMER



/**
  * @brief  Initialize the RTC peripheral
  * @param  hrtc: RTC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_Init(RTC_HandleTypeDef *hrtc)

/**
  * @brief  Sets RTC current date.
  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sDate: Pointer to date structure
  * @param  Format: specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format 
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)

/**
  * @brief  Sets RTC current time.
  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sTime: Pointer to Time structure
  * @param  Format: Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format 
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)

/**
  * @brief  Enable the Bypass Shadow feature.
  * @note   When the Bypass Shadow is enabled the calendar value are taken
  *         directly from the Calendar counter.
  * @param  hrtc RTC handle
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTCEx_EnableBypassShadow(RTC_HandleTypeDef* hrtc)

/**
  * @brief  Set the priority of an interrupt.
  * @param  IRQn: External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to the appropriate CMSIS device file (stm32l4xxxx.h))
  * @param  PreemptPriority: The pre-emption priority for the IRQn channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority
  * @param  SubPriority: the subpriority level for the IRQ channel.
  *         This parameter can be a value between 0 and 15
  *         A lower priority value indicates a higher priority.
  * @retval None
  */
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority)

/**
  * @brief  Enables a device specific interrupt in the NVIC interrupt controller.
  * @note   To configure interrupts priority correctly, the NVIC_PriorityGroupConfig()
  *         function should be called before. 
  * @param  IRQn External interrupt number .
  *         This parameter can be an enumerator of  IRQn_Type enumeration
  *         (For the complete STM32 Devices IRQ Channels list, please refer to stm32l0xx.h file)  
  * @retval None
  */
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn)

/**
  * @brief  Deactive the specified RTC Alarm 
  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  Alarm: Specifies the Alarm.
  *          This parameter can be one of the following values:
  *            @arg RTC_ALARM_A:  AlarmA
  *            @arg RTC_ALARM_B:  AlarmB
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_DeactivateAlarm(RTC_HandleTypeDef *hrtc, uint32_t Alarm)


/**
  * @brief  Sets the specified RTC Alarm with Interrupt 
  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sAlarm: Pointer to Alarm structure
  * @param  Format: Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *             @arg RTC_FORMAT_BIN: Binary data format 
  *             @arg RTC_FORMAT_BCD: BCD data format
  * @note   The Alarm register can only be written when the corresponding Alarm
  *         is disabled (Use the HAL_RTC_DeactivateAlarm()).   
  * @note   The HAL_RTC_SetTime() must be called before enabling the Alarm feature.   
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_SetAlarm_IT(RTC_HandleTypeDef *hrtc, RTC_AlarmTypeDef *sAlarm, uint32_t Format)

/**
  * @brief  Gets RTC current date.
  * @param  hrtc: pointer to a RTC_HandleTypeDef structure that contains
  *                the configuration information for RTC.
  * @param  sDate: Pointer to Date structure
  * @param  Format: Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN:  Binary data format 
  *            @arg RTC_FORMAT_BCD:  BCD data format
  * @note You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values 
  * in the higher-order calendar shadow registers to ensure consistency between the time and date values.
  * Reading RTC current time locks the values in calendar shadow registers until Current date is read.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)

/**
  * @brief  Get RTC current time.
  * @param  hrtc: RTC handle
  * @param  sTime: Pointer to Time structure with Hours, Minutes and Seconds fields returned 
  *                with input format (BIN or BCD), also SubSeconds field returning the
  *                RTC_SSR register content and SecondFraction field the Synchronous pre-scaler
  *                factor to be used for second fraction ratio computation.
  * @param  Format: Specifies the format of the entered parameters.
  *          This parameter can be one of the following values:
  *            @arg RTC_FORMAT_BIN: Binary data format 
  *            @arg RTC_FORMAT_BCD: BCD data format
  * @note  You can use SubSeconds and SecondFraction (sTime structure fields returned) to convert SubSeconds
  *        value in second fraction ratio with time unit following generic formula:
  *        Second fraction ratio * time_unit= [(SecondFraction-SubSeconds)/(SecondFraction+1)] * time_unit
  *        This conversion can be performed only if no shift operation is pending (ie. SHFP=0) when PREDIV_S >= SS
  * @note  You must call HAL_RTC_GetDate() after HAL_RTC_GetTime() to unlock the values 
  *        in the higher-order calendar shadow registers to ensure consistency between the time and date values.
  *        Reading RTC current time locks the values in calendar shadow registers until Current date is read
  *        to ensure consistency between the time and date values.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)

/*!
 * \brief  Alarm A callback.
 *
 * \param [IN] hrtc RTC handle
 */
void HAL_RTC_AlarmAEventCallback( RTC_HandleTypeDef *hrtc )

/**
  * @brief  Write a data in a specified RTC Backup data register.
  * @param  hrtc: RTC handle 
  * @param  BackupRegister: RTC Backup data Register number.
  *          This parameter can be: RTC_BKP_DRx where x can be from 0 to 19 to 
  *                                 specify the register.
  * @param  Data: Data to be written in the specified RTC Backup data register.
  * @retval None
  */
void HAL_RTCEx_BKUPWrite(RTC_HandleTypeDef *hrtc, uint32_t BackupRegister, uint32_t Data)

