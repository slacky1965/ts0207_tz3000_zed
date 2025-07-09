#include "app_main.h"

#define WATERLEAK_COUNT_MAX  16

static uint8_t waterleak_count = 0;
static uint8_t no_waterleak_count = 0;
static uint32_t check_rejoin = 0;
static uint8_t first_start = 1;
//static uint8_t start_leak = 0;
static bool reset_leak = false;
static ev_timer_event_t *timerResetLeakEvt = NULL;

void fillIASAddress(epInfo_t* pdstEpInfo) {
    u16 len;
    u8 zoneState;

    memset((u8 *)pdstEpInfo, 0, sizeof(epInfo_t));

    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATE, &len, &zoneState);

    pdstEpInfo->dstEp = APP_ENDPOINT1;
    pdstEpInfo->profileId = HA_PROFILE_ID;

    if (zoneState&ZONE_STATE_ENROLLED) { //device enrolled
        pdstEpInfo->dstAddrMode = APS_LONG_DSTADDR_WITHEP;
        zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_IAS_CIE_ADDR, &len, (u8*)&pdstEpInfo->dstAddr.extAddr);
    }
    else {
        pdstEpInfo->dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
        pdstEpInfo->dstAddr.shortAddr = 0x0000;
    }
}

//static void app_leakOn(void *args) {
//
//    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
//
//#if UART_PRINTF_MODE && DEBUG_ONOFF
//    printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
//#endif /* DEBUG_ONOFF */
//
//    switch (onoffCfgAttrs->switchActions) {
//        case ZCL_SWITCH_ACTION_ON_OFF:
//            cmdOnOff(ZCL_CMD_ONOFF_OFF);
//            break;
//        case ZCL_SWITCH_ACTION_OFF_ON:
//            cmdOnOff(ZCL_CMD_ONOFF_ON);
//            break;
//        case ZCL_SWITCH_ACTION_TOGGLE:
//            cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//            break;
//        default:
//            break;
//    }
//
//}
//
//static int32_t leak_onTimerCb(void *args) {
//
//    TL_SCHEDULE_TASK(app_leakOn, NULL);
//
//    return -1;
//}

//static int32_t start_leakTimerCb(void *args) {
//
//    start_leak++;
//
//    return -1;
//}

static int32_t reset_leakTimerCb() {

    g_appCtx.leak = false;

    reset_leak = true;

    timerResetLeakEvt = NULL;
    return -1;
}

void waterleak_handler() {

    uint16_t len;
    epInfo_t dstEpInfo;
    zoneStatusChangeNoti_t statusChangeNotification;
    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

    if (zb_getLocalShortAddr() < 0xFFF8) {

        if (first_start) {
            check_rejoin = clock_time();
            first_start = 0;
        }

        if (!zb_isDeviceJoinedNwk()) {
            if (clock_time_exceed(check_rejoin, TIMEOUT_TICK_5SEC)) {
                check_rejoin = clock_time();
                zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                return;
            }
        }
    } else {
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

            fillIASAddress(&dstEpInfo);

            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*) &statusChangeNotification.zoneStatus);
            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

            statusChangeNotification.zoneStatus |= ZONE_STATUS_BIT_ALARM1;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*) &statusChangeNotification.zoneStatus);
            statusChangeNotification.extStatus = 0;
            statusChangeNotification.delay = 0;

            zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);

//            app_leakOn(NULL);
//            app_leakOn(NULL);
//            app_leakOn(NULL);

#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
#endif /* DEBUG_ONOFF */

            switch (onoffCfgAttrs->switchActions) {
            case ZCL_SWITCH_ACTION_ON_OFF:
                cmdOnOff(ZCL_CMD_ONOFF_OFF);
                break;
            case ZCL_SWITCH_ACTION_OFF_ON:
                cmdOnOff(ZCL_CMD_ONOFF_ON);
                break;
            case ZCL_SWITCH_ACTION_TOGGLE:
                cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
                break;
            default:
                break;
            }

            app_setPollRate(TIMEOUT_20SEC);

            if (!reset_leak)
                timerResetLeakEvt = TL_ZB_TIMER_SCHEDULE(reset_leakTimerCb, NULL, TIMEOUT_250MS);

            g_appCtx.leak = true;

//            if (start_leak == 0 || start_leak == 2 || start_leak == 4) {
//#if UART_PRINTF_MODE //&& DEBUG_LEAK
//                printf("There is a water leak. start_leak: %d\r\n", start_leak);
//#endif /* DEBUG_LEAK */
//
//                fillIASAddress(&dstEpInfo);
//
//                zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*) &statusChangeNotification.zoneStatus);
//                zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);
//
//                statusChangeNotification.zoneStatus |= ZONE_STATUS_BIT_ALARM1;
//                zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*) &statusChangeNotification.zoneStatus);
//                statusChangeNotification.extStatus = 0;
//                statusChangeNotification.delay = 0;
//
//                zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);
//
//#if UART_PRINTF_MODE && DEBUG_ONOFF
//                printf("Switch action: 0x0%x\r\n", onoffCfgAttrs->switchActions);
//#endif /* DEBUG_ONOFF */
//
//                switch (onoffCfgAttrs->switchActions) {
//                case ZCL_SWITCH_ACTION_ON_OFF:
//                    cmdOnOff(ZCL_CMD_ONOFF_OFF);
//                    break;
//                case ZCL_SWITCH_ACTION_OFF_ON:
//                    cmdOnOff(ZCL_CMD_ONOFF_ON);
//                    break;
//                case ZCL_SWITCH_ACTION_TOGGLE:
//                    cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
//                    break;
//                default:
//                    break;
//                }
//
////                TL_ZB_TIMER_SCHEDULE(leak_onTimerCb, NULL, TIMEOUT_250MS);
//                app_setPollRate(TIMEOUT_20SEC);
//
//                if (start_leak == 4) {
//                    g_appCtx.leak = true;
//                    start_leak = 0;
//                    if (!reset_leak)
//                        timerResetLeakEvt = TL_ZB_TIMER_SCHEDULE(reset_leakTimerCb, NULL, TIMEOUT_3SEC);
//
//                } else {
//                    sleep_ms(100);
//                    fillIASAddress(&dstEpInfo);
//
//                    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*) &statusChangeNotification.zoneStatus);
//                    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);
//
//                    statusChangeNotification.zoneStatus &= ~ZONE_STATUS_BIT_ALARM1;
//                    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*) &statusChangeNotification.zoneStatus);
//                    statusChangeNotification.extStatus = 0;
//                    statusChangeNotification.delay = 0;
//
//                    zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);
//
//                    start_leak++;
//                    TL_ZB_TIMER_SCHEDULE(start_leakTimerCb, NULL, TIMEOUT_100MS);
//                }
//            }
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
            reset_leak = false;
            if (timerResetLeakEvt)
                TL_ZB_TIMER_CANCEL(&timerResetLeakEvt);

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
                case ZCL_SWITCH_ACTION_TOGGLE:
                    cmdOnOff(ZCL_CMD_ONOFF_TOGGLE);
                    break;
                default:
                    break;
            }
#endif /* DEBUG_LEAK */

            fillIASAddress(&dstEpInfo);

            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*) &statusChangeNotification.zoneStatus);
            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

            statusChangeNotification.zoneStatus &= ~ZONE_STATUS_BIT_ALARM1;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*) &statusChangeNotification.zoneStatus);
            statusChangeNotification.extStatus = 0;
            statusChangeNotification.delay = 0;

            zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);
        }
    }
}

bool waterleak_idle() {
    if (waterleak_count) return true;
    if (no_waterleak_count) return true;
    return false;
}
