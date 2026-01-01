#include "app_main.h"

void forceReportBattery(void *args) {

#if UART_PRINTF_MODE && DEBUG_BATTERY
    printf("forceReportBattery()\r\n");
#endif

    struct report_t {
        u8 numAttr;
        zclReport_t attr[2];
    };

    struct report_t report;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    report.numAttr = 0;

    zclAttrInfo_t *pAttrEntry;

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);

    if (pAttrEntry) {
        report.attr[report.numAttr].attrID = pAttrEntry->id;
        report.attr[report.numAttr].dataType = pAttrEntry->type;
        report.attr[report.numAttr].attrData = pAttrEntry->data;
        report.numAttr++;
    }

    if (report.numAttr) {
        zcl_sendReportAttrsCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, ZCL_CLUSTER_GEN_POWER_CFG, (zclReportCmd_t* )&report);
    }
}
