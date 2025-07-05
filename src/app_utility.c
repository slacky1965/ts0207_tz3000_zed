#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

uint32_t mcuBootAddrGet(void);

void start_message() {
#ifdef ZCL_OTA
#if UART_PRINTF_MODE
        printf("OTA mode enabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#else
#if UART_PRINTF_MODE
    printf("OTA mode desabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif /* UART_PRINTF_MODE */
#endif

#if UART_PRINTF_MODE
    const uint8_t version[] = ZCL_BASIC_SW_BUILD_ID;
    printf("Firmware version: %s\r\n", version+1);
#endif
}

int32_t delayedMcuResetCb(void *arg) {

    //printf("mcu reset\r\n");
    zb_resetDevice();
    return -1;
}

int32_t delayedFactoryResetCb(void *arg) {

    zb_resetDevice2FN();
    zb_deviceFactoryNewSet(true);

    factory_reset = true;
    g_appCtx.timerFactoryReset = NULL;

//    printf("Cb Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");


    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}

static int32_t set_pollRateCb(void *args) {

    uint32_t sec = (u32)args;

//    printf("set_pollRateCb\r\n");

//    zb_setPollRate(POLL_RATE * 60);

    if (zb_getLocalShortAddr() < 0xFFF8) {
//        printf("zb_getLocalShortAddr() < 0xFFF8\r\n");
        if (zb_isDeviceJoinedNwk()) {
            g_appCtx.not_sleep = false;
        } else {
            zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
            if (g_appCtx.timerCheckSleepEvt) {
                printf("2. checkSleepTimer restart\r\n");
                TL_ZB_TIMER_CANCEL(&g_appCtx.timerCheckSleepEvt);
            }
            g_appCtx.timerCheckSleepEvt = TL_ZB_TIMER_SCHEDULE(check_sleepCb, NULL, (sec + TIMEOUT_10SEC));

            return 0;
        }
    }

    g_appCtx.timerSetPollRateEvt = NULL;
    return -1;
}

void app_setPollRate(uint32_t sec) {

    g_appCtx.not_sleep = true;

    zb_setPollRate(POLL_RATE * 3);
    if (g_appCtx.timerSetPollRateEvt) {
        TL_ZB_TIMER_CANCEL(&g_appCtx.timerSetPollRateEvt);
    }
    g_appCtx.timerSetPollRateEvt = TL_ZB_TIMER_SCHEDULE(set_pollRateCb, (void *)sec, sec /*TIMEOUT_20SEC*/);

    if (g_appCtx.timerCheckSleepEvt) {
        printf("1. checkSleepTimer restart\r\n");
        TL_ZB_TIMER_CANCEL(&g_appCtx.timerCheckSleepEvt);
    }
    g_appCtx.timerCheckSleepEvt = TL_ZB_TIMER_SCHEDULE(check_sleepCb, NULL, (sec + TIMEOUT_10SEC));
}

