/*
 * peripheralManager.h
 *
 *  Created on: Feb 11, 2016
 *      Author: michaelblouin
 */

#ifndef APPLICATION_PERIPHERALMANAGER_H_
#define APPLICATION_PERIPHERALMANAGER_H_

#include "Board.h"
#include "Devices/tca9554a.h"

#define PERIPHERAL_MAX_READ_ATTEMPTS 3

#define HDC1050_READ_WAIT_TICKS ((uint16)(HDC1050_CONV_TIME_HRES_14BIT + HDC1050_CONV_TIME_TRES_14BIT)) * NTICKS_PER_MILLSECOND + (1 * NTICKS_PER_MILLSECOND)

#define IOEXP_I2CSTATUS_PIN_BLE IOPORT4
#define IOEXP_I2CSTATUS_PIN_TEMP0 IOPORT2
#define IOEXP_I2CSTATUS_PIN_HUMIDITY IOPORT5
#define IOEXP_I2CSTATUS_PIN_TEMP(index) \
	(index < 2 ? \
		(TCA9554A_IO_PORT)(IOEXP_I2CSTATUS_PIN_TEMP0 + (TCA9554A_IO_PORT)(index % SB_NUM_MCP9808_SENSORS)) \
		: (TCA9554A_IO_PORT)(IOPORT7 - (TCA9554A_IO_PORT)((index-2) % SB_NUM_MCP9808_SENSORS)))

#ifdef SB_DEBUG
# define PMANAGER_TASK_YIELD_HIGHERPRI() Task_sleep(100)
#else
# define PMANAGER_TASK_YIELD_HIGHERPRI() Task_sleep(1)
#endif

#if IOEXP_I2CSTATIS_PIN_HUMIDITY > 7
#error "Too many MCP9808 sensor for debug LEDs"
#endif

typedef enum {
	PState_Unknown,
	PState_OK,
	PState_Intermittent,
	PState_FailedConfig,
	PState_Failed,
} SB_PeripheralFunctionalState;

typedef struct {
	SB_Error lastError;
	SB_PeripheralFunctionalState currentState;
	uint8_t numReadAttempts;
} SB_PeripheralState;

typedef struct {
	MUX_OUTPUT pwrmuxOutput;
	MUX_OUTPUT_ENABLE pwrmuxOutputEnable;
	MUX_OUTPUT iomuxOutput;
} SB_MUXState;

/*********************************************************************
 * @fn      SB_peripheralInit
 *
 * @brief   Initializes the peripheral manager
 *
 * @return  NoError if properly initialized, otherwise the error that occured
 */
SB_Error SB_peripheralInit();

/*********************************************************************
 * @fn      SB_setPeripheralsEnable
 *
 * @brief   Enables or disables peripherals
 *
 * @return  NoError if properly enabled/disabled, otherwise the error that occured
 */
SB_Error SB_setPeripheralsEnable(bool enable);

/*********************************************************************
 * @fn      SB_sysDisableRefresh
 *
 * @brief   Refreshes the sys disable output waits at most semaphoreTimeout to access resources
 *
 * @return  NoError if properly refreshed, otherwise the error that occured
 */
SB_Error SB_sysDisableRefresh(uint32 semaphoreTimeout);

/*********************************************************************
 * @fn      SB_sysDisableShutdown
 *
 * @brief   Shuts down the MCU through the sys disable output. This function doesn't return.
 *
 * @return  Doesn't return if success, otherwise the error that occured
 */
SB_Error SB_sysDisableShutdown();

/*********************************************************************
 * @fn      SB_selectMoistureSensorInput
 *
 * @brief   Selects the current moisture sensor input
 *
 * @return  NoError if properly selected, otherwise the error that occured
 */
SB_Error SB_selectMoistureSensorInput(SB_MoistureSensorLine line, SB_MoistureSensorVoltage voltage, uint32_t timeout);

#endif /* APPLICATION_PERIPHERALMANAGER_H_ */
