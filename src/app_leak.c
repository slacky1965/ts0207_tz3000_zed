#include "app_main.h"

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

static void app_leakOn(void *args) {

    zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();

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

}

static int32_t leak_onTimerCb(void *args) {

    TL_SCHEDULE_TASK(app_leakOn, NULL);

    return -1;
}

void waterleak_handler() {

    uint16_t len;
    epInfo_t dstEpInfo;
    zoneStatusChangeNoti_t statusChangeNotification;

#if (BOARD == BOARD_ZG_222Z)
    if (!drv_gpio_read(WLEAK_GPIO)) {
#elif (BOARD == BOARD_ZG_222ZA)
    if (drv_gpio_read(WLEAK_GPIO)) {
#else
#error BOARD must be defined
#endif
        if (!g_appCtx.leak) {
            g_appCtx.leak = true;

#if UART_PRINTF_MODE && DEBUG_LEAK
            printf("There is a water leak\r\n");
#endif /* DEBUG_LEAK */

            fillIASAddress(&dstEpInfo);

            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*)&statusChangeNotification.zoneStatus);
            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

            statusChangeNotification.zoneStatus |= ZONE_STATUS_BIT_ALARM1;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&statusChangeNotification.zoneStatus);
            statusChangeNotification.extStatus = 0;
            statusChangeNotification.delay = 0;

            zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);

            TL_ZB_TIMER_SCHEDULE(leak_onTimerCb, NULL, TIMEOUT_250MS);
            app_setPollRate(TIMEOUT_20SEC);
        }
    } else {
        if (g_appCtx.leak) {
            g_appCtx.leak = false;
            app_setPollRate(TIMEOUT_20SEC);

#if UART_PRINTF_MODE && DEBUG_LEAK
            zcl_onOffSwitchCfgAttr_t *onoffCfgAttrs = zcl_onOffSwitchCfgAttrGet();
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

            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*)&statusChangeNotification.zoneStatus);
            zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

            statusChangeNotification.zoneStatus &= ~ZONE_STATUS_BIT_ALARM1;
            zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*)&statusChangeNotification.zoneStatus);
            statusChangeNotification.extStatus = 0;
            statusChangeNotification.delay = 0;

            zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, &statusChangeNotification);
        }
    }
}
