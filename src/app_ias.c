#include "app_main.h"

void sendIasNotification(zoneStatusChangeNoti_t *noti) {
    u16 len;
    u8 zoneState;
    epInfo_t dstEp;

    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATE, &len, &zoneState);

    if (!(zoneState & ZONE_STATE_ENROLLED)) {
        return;
    }

    TL_SETSTRUCTCONTENT(dstEp, 0);
    dstEp.profileId = HA_PROFILE_ID;
    dstEp.dstEp = APP_ENDPOINT1;
    dstEp.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_IAS_CIE_ADDR, &len, (u8*)&dstEp.dstAddr.extAddr);
    zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &dstEp, TRUE, noti);

    if (zb_bindingTblSearched(ZCL_CLUSTER_SS_IAS_ZONE, APP_ENDPOINT1)) {
        epInfo_t bindEp;
        TL_SETSTRUCTCONTENT(bindEp, 0);
        bindEp.profileId = HA_PROFILE_ID;
        bindEp.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        zcl_iasZone_statusChangeNotificationCmd(APP_ENDPOINT1, &bindEp, TRUE, noti);
    }
}

void waterleak_ias(bool leak) {

    uint16_t len;
    zoneStatusChangeNoti_t statusChangeNotification;

    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, &len, (u8*) &statusChangeNotification.zoneStatus);
    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_ID, &len, &statusChangeNotification.zoneId);

    if (leak) {
        statusChangeNotification.zoneStatus |= ZONE_STATUS_BIT_ALARM1;
    } else {
        statusChangeNotification.zoneStatus &= ~ZONE_STATUS_BIT_ALARM1;
    }

    zcl_setAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATUS, (u8*) &statusChangeNotification.zoneStatus);
    statusChangeNotification.extStatus = 0;
    statusChangeNotification.delay = 0;

    sendIasNotification(&statusChangeNotification);
}

void requestIasEnrollment(void) {
    u16 len;
    u8 zoneState;
    epInfo_t dstEp;

    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_ZONE_STATE, &len, &zoneState);

    if (zoneState & ZONE_STATE_ENROLLED) {
        return;
    }

    TL_SETSTRUCTCONTENT(dstEp, 0);
    dstEp.profileId = HA_PROFILE_ID;
    dstEp.dstEp = APP_ENDPOINT1;
    dstEp.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
    zcl_getAttrVal(APP_ENDPOINT1, ZCL_CLUSTER_SS_IAS_ZONE, ZCL_ATTRID_IAS_CIE_ADDR, &len, (u8*)&dstEp.dstAddr.extAddr);

    zoneEnrollReq_t enrollReq;
    enrollReq.zoneType = ZONE_TYPE_WATER_SENSOR;
    enrollReq.manufacturerCode = MANUFACTURER_CODE_TELINK;

    zcl_iasZone_enrollReqCmd(APP_ENDPOINT1, &dstEp, TRUE, &enrollReq);
}
