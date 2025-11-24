/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#include "app_types.h"

#define ON                              1
#define OFF                             0

/* for reporting */
#define REPORTING_MIN                   10              /* 10 sec                   */
#define REPORTING_MAX                   3600            /* 60 min                   */
#define REPORTING_BATTERY_MIN           3600            /* 1 hour                   */
#define REPORTING_BATTERY_MAX           0               /* reporting on the change  */

/* for polling */
#define LONG_POLL                       REPORTING_MIN
#define TIMEOUT_NET                     TIMEOUT_30MIN

/* for OTA */
#define APP_OTA_PERIODIC_QUERY_INTERVAL (1080 * 60)     /* start the OTA request after the set seconds */

/* Debug mode config */
#define UART_PRINTF_MODE                ON
#define USB_PRINTF_MODE                 OFF

#define DEBUG_BUTTON                    ON
#define DEBUG_REPORTING                 OFF
#define DEBUG_BATTERY                   OFF
#define DEBUG_PM                        ON
#define DEBUG_OTA                       OFF
#define DEBUG_LEAK                      OFF
#define DEBUG_STA_STATUS                OFF
#define DEBUG_ONOFF                     ON

/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"

/* PM */
#define PM_ENABLE						ON

/* PA */
#define PA_ENABLE						OFF

/* BDB */
#define TOUCHLINK_SUPPORT				ON
#define FIND_AND_BIND_SUPPORT			OFF


/* Board define */
#if defined(MCU_CORE_826x)
#if !PA_ENABLE
    #define BOARD                       BOARD_826x_DONGLE
#else
    #define BOARD                       BOARD_826x_DONGLE_PA
#endif
    #define CLOCK_SYS_CLOCK_HZ          32000000
#elif defined(MCU_CORE_8258)
#if (CHIP_TYPE == TLSR_8258_1M)
    #define FLASH_CAP_SIZE_1M           1
#endif
//    #define BOARD                       BOARD_ZG_222ZA
    #define CLOCK_SYS_CLOCK_HZ          48000000
    #define NV_ITEM_APP_USER_CFG        (NV_ITEM_APP_GP_TRANS_TABLE + 1)    // see sdk/proj/drivers/drv_nv.h
#elif defined(MCU_CORE_8278)
    #define FLASH_CAP_SIZE_1M           1
    #define BOARD                       BOARD_8278_DONGLE//BOARD_8278_EVK
    #define CLOCK_SYS_CLOCK_HZ          48000000
#elif defined(MCU_CORE_B91)
    #define FLASH_CAP_SIZE_1M           1
    #define BOARD                       BOARD_B91_DONGLE//BOARD_B91_EVK
    #define CLOCK_SYS_CLOCK_HZ          48000000
#else
    #error "MCU is undefined!"
#endif

/* Board include */
#if (BOARD == BOARD_ZG_222Z)
#include "board_zg_222z.h"
#elif (BOARD == BOARD_ZG_222ZA)
#include "board_zg_222za.h"
#elif (BOARD == BOARD_SNZB_05)
#include "board_snzb_05.h"
#else
#error BOARD must be defined
#endif

/* Voltage detect module */
/* If VOLTAGE_DETECT_ENABLE is set,
 * 1) if MCU_CORE_826x is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * and there is no need to configure the detection IO port;
 * 2) if MCU_CORE_8258 or MCU_CORE_8278 is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be in a floating state.
 * 3) if MCU_CORE_B91 is defined, the DRV_ADC_BASE_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be connected to the target under test,
 * such as VCC.
 */
#define VOLTAGE_DETECT_ENABLE                       ON
#define VOLTAGE_DETECT_ADC_PIN                      VOLTAGE_DETECT_PIN

/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE						OFF

/* UART module */
#define	MODULE_UART_ENABLE							OFF

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
	#define ZBHCI_EN								1
#endif

/**********************************************************************
 * ZCL cluster support setting
 */
#define ZCL_GROUP_SUPPORT                           ON
#define ZCL_SCENE_SUPPORT                           ON
#define ZCL_IAS_ZONE_SUPPORT                        ON
#define ZCL_POWER_CFG_SUPPORT                       ON
#define ZCL_OTA_SUPPORT                             ON
#if TOUCHLINK_SUPPORT
#define ZCL_ZLL_COMMISSIONING_SUPPORT               ON
#endif
#define ZCL_ON_OFF_SUPPORT                          ON
#define ZCL_ON_OFF_SWITCH_CFG_SUPPORT               ON

/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"


/**********************************************************************
 * EV configuration
 */
typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
