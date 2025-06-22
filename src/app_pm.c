#include "app_main.h"

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */

static drv_pm_pinCfg_t pin_PmCfg[] = {
    {
        WLEAK_GPIO,
        PM_WAKEUP_LEVEL_HIGH //PM_WAKEUP_LEVEL
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

    app_wakeupPinLevelChange();

    if (g_appCtx.not_sleep) {
        /* Deep sleep with SRAM retention */
        drv_pm_lowPowerEnter();
    } else if (zb_isDeviceJoinedNwk()){
        /* Deep sleep without SRAM retention */
#if UART_PRINTF_MODE && DEBUG_PM
        printf("Deep sleep start\r\n");
#endif

        if(tl_stackBusy() || !zb_isTaskDone()){
#if UART_PRINTF_MODE && DEBUG_PM
            printf("Stack or Task busy. Return from deep sleep start\r\n");
#endif
            return;
        }


//    drv_pm_wakeupPinLevelChange(pin_PmCfg, sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t));

    apsCleanToStopSecondClock();

    uint32_t r = drv_disable_irq();
    rf_paShutDown();
    drv_pm_deepSleep_frameCnt_set(ss_outgoingFrameCntGet());

    g_appCtx.analog_reg.leak = g_appCtx.leak;
    g_appCtx.analog_reg.deep_sleep = 1;

    app_set_analog_reg((uint8_t*)&g_appCtx.analog_reg);

    drv_pm_longSleep(PM_SLEEP_MODE_DEEPSLEEP, PM_WAKEUP_SRC_PAD | PM_WAKEUP_SRC_TIMER, (1000 * TIME_LONG_DEEP_SLEEP));

    drv_restore_irq(r);

    }
}

#endif

void app_set_analog_reg(uint8_t *reg_data) {
    analog_write(APP_ANALOG_REG, *reg_data);
}

uint8_t app_get_analog_reg() {
    return analog_read(APP_ANALOG_REG);
}
