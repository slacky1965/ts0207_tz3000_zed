#include "app_main.h"

#define WATERLEAK_COUNT_MAX 8
#define WATERLEAK_OPER      5

static uint8_t waterleak_count = 0;
static uint8_t no_waterleak_count = 0;
//static uint32_t check_rejoin = 0;
//static uint8_t first_start = 1;
//static bool reset_leak = false;
static uint8_t waterleak_oper = 0;
static ev_timer_event_t *timerResetLeakEvt = NULL;

static void leak_cmd(uint8_t action) {

    switch (action) {
    case ZCL_SWITCH_ACTION_ON_OFF:
        cmdOnOff(ZCL_CMD_ONOFF_OFF);
        break;
    case ZCL_SWITCH_ACTION_OFF_ON:
        cmdOnOff(ZCL_CMD_ONOFF_ON);
        break;
//            case ZCL_SWITCH_ACTION_TOGGLE:
//                cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                break;
    default:
        break;
    }
}

static int32_t onoff_cmd_repeatCb(void *args) {

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

    app_setPollRate(TIMEOUT_5SEC);

#if (BOARD == BOARD_ZG_222Z)
    if (!drv_gpio_read(WLEAK_GPIO)) {
#elif (BOARD == BOARD_ZG_222ZA || BOARD == BOARD_SNZB_05)
    if (drv_gpio_read(WLEAK_GPIO)) {
#else
#error BOARD must be defined
#endif
        g_appCtx.leak = true;
        waterleak_ias(1);
        leak_cmd(onoffCfgAttrs->switchActions);
        return 0;
    } else {
        g_appCtx.leak = false;
    }

    waterleak_ias(0);
    g_appCtx.timerOnOffRepeatEvt = NULL;
    return -1;
}

static int32_t reset_leakTimerCb() {

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

//    reset_leak = true;

    if (waterleak_oper < WATERLEAK_OPER) {
        if (waterleak_oper == 0) {
            g_appCtx.leak = false;
        } else {
            leak_cmd(onoffCfgAttrs->switchActions);
        }
        waterleak_oper++;
        return TIMEOUT_1SEC;
    }

    waterleak_oper = 0;
    timerResetLeakEvt = NULL;

    if (!g_appCtx.timerSetPollRateEvt) {
        app_setPollRate(TIMEOUT_20SEC);
    }

    if (!g_appCtx.timerOnOffRepeatEvt) g_appCtx.timerOnOffRepeatEvt = TL_ZB_TIMER_SCHEDULE(onoff_cmd_repeatCb, NULL, TIME_REPEAT_ONOFF);

    return -1;
}

void waterleak_handler() {

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

    if (zb_getLocalShortAddr() >= 0xFFF8) {
        return;
    }

#if (BOARD == BOARD_ZG_222Z)
    if (!drv_gpio_read(WLEAK_GPIO)) {
#elif (BOARD == BOARD_ZG_222ZA || BOARD == BOARD_SNZB_05)
    if (drv_gpio_read(WLEAK_GPIO)) {
#else
#error BOARD must be defined
#endif

        no_waterleak_count = 0;

        if (!g_appCtx.leak) {

            if (waterleak_count++ < WATERLEAK_COUNT_MAX) {
                sleep_ms(5);
                return;
            }

#if UART_PRINTF_MODE //&& DEBUG_LEAK
            printf("There is a water leak.\r\n");
#endif /* DEBUG_LEAK */

            waterleak_ias(1);

#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* DEBUG_ONOFF */

            leak_cmd(onoffCfgAttrs->switchActions);

            app_setPollRate(TIMEOUT_20SEC);

            if (!timerResetLeakEvt) timerResetLeakEvt = TL_ZB_TIMER_SCHEDULE(reset_leakTimerCb, NULL, TIMEOUT_700MS);

            g_appCtx.leak = true;
            waterleak_count = 0;
        }
    } else {

        waterleak_count = 0;

        if (g_appCtx.leak) {

            if (no_waterleak_count++ < WATERLEAK_COUNT_MAX) {
                sleep_ms(5);
                return;
            }

            g_appCtx.leak = false;
            no_waterleak_count = 0;
            app_setPollRate(TIMEOUT_20SEC);

#if UART_PRINTF_MODE && DEBUG_LEAK
            printf("No water leakage occurs\r\n");
#if UART_PRINTF_MODE && DEBUG_ONOFF
                    printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* DEBUG_ONOFF */

            switch(onoffCfgAttrs->switchActions) {
                case ZCL_SWITCH_ACTION_ON_OFF:
                    cmdOnOff(ZCL_CMD_ONOFF_ON);
                    break;
                case ZCL_SWITCH_ACTION_OFF_ON:
                    cmdOnOff(ZCL_CMD_ONOFF_OFF);
                    break;
//                case ZCL_SWITCH_ACTION_TOGGLE:
//                    cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                    break;
                default:
                    break;
            }
#endif /* DEBUG_LEAK */

            waterleak_ias(0);
        }
    }
}

bool waterleak_idle() {
    if (waterleak_count) return true;
    if (no_waterleak_count) return true;
    return false;
}
