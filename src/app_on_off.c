#include "app_main.h"

//static bool sw_onoff = false;

void cmdOnOff(uint8_t command) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.profileId = HA_PROFILE_ID;

    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

    /* command 0x00 - off, 0x01 - on, 0x02 - toggle */

    switch(command) {
        case ZCL_CMD_ONOFF_OFF:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: off\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_offCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_ON:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: on\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_onCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_TOGGLE:
#if UART_PRINTF_MODE && DEBUG_ONOFF
            printf("OnOff command: toggle\r\n");
#endif /* UART_PRINTF_MODE */
            zcl_onOff_toggleCmd(APP_ENDPOINT1, &dstEpInfo, FALSE);
            break;
        default:
            break;
    }
}

