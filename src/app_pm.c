#include "app_main.h"

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */

static drv_pm_pinCfg_t pin_PmCfg[] = {
    {
        WLEAK_GPIO,
#if (BOARD == BOARD_ZG_222Z)
        PM_WAKEUP_LEVEL_LOW
#elif (BOARD == BOARD_ZG_222ZA || BOARD == BOARD_SNZB_05)
        PM_WAKEUP_LEVEL_HIGH
#else
#error BOARD must be defined
#endif
    },
    {
        BUTTON1,
        PM_WAKEUP_LEVEL
    },
};

void app_wakeupPinConfig() {
    drv_pm_wakeupPinConfig(pin_PmCfg, sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t));
}


void app_wakeupPinLevelChange() {
    drv_pm_wakeupPinLevelChange(pin_PmCfg, 1);
}

void app_lowPowerEnter() {

//    printf("app_lowPowerEnter(). g_appCtx.not_sleep: %d\r\n", g_appCtx.not_sleep);

    uint32_t durationMs = 0;

    app_wakeupPinLevelChange();

    if (g_appCtx.not_sleep) {
        /* SDK deep sleep with SRAM retention */
        drv_pm_lowPowerEnter();
    } else /*if (zb_isDeviceJoinedNwk())*/{
        /* app deep sleep with SRAM retention */
        if (tl_stackBusy() || !zb_isTaskDone()) {
#if UART_PRINTF_MODE && DEBUG_PM
            printf("Stack or Task busy. Return from deep sleep start\r\n");
#endif
            return;
        }

        apsCleanToStopSecondClock();

        uint32_t r = drv_disable_irq();
        rf_paShutDown();

        if (g_appCtx.timerOnOffRepeatEvt) {
            durationMs = g_appCtx.timerOnOffRepeatEvt->timeout;
        } else {
            durationMs = TIME_LONG_DEEP_SLEEP * 1000;
        }

#if UART_PRINTF_MODE && DEBUG_PM
        printf("Long deep sleep start with time: %d sec\r\n", durationMs / 1000);
#endif

        drv_pm_longSleep(PM_SLEEP_MODE_DEEP_WITH_RETENTION, PM_WAKEUP_SRC_PAD | PM_WAKEUP_SRC_TIMER, durationMs);

        drv_restore_irq(r);

    }
}

#endif


//int32_t check_sleepCb(void *args) {
//
//    if (zb_getLocalShortAddr() < 0xFFF8) {
//
//        if (g_appCtx.ota) {
//            printf("check_sleepCb - OTA\r\n");
//            g_appCtx.timerCheckSleepEvt = NULL;
//            return -1;
//        }
//
//        printf("check_sleepCb - reset\r\n");
//        sleep_ms(250);
//
//        zb_resetDevice();
//        return -1;
//    }
//
////    if (zb_isDeviceJoinedNwk()) {
////
////        printf("check_sleepCb - reset\r\n");
////        sleep_ms(250);
////
////        zb_resetDevice();
////        return -1;
////    }
//
//    printf("check_sleepCb - no joined\r\n");
//    return 0;
//}
