/**********************************************************************
 * INCLUDES
 */
#include "zcl_include.h"
#include "app_main.h"

#ifdef ZCL_ON_OFF_SWITCH_CFG
_CODE_ZCL_ status_t zcl_onOffSwitchCfg_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
//	return zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb);
	u8 status = ZCL_STA_SUCCESS;
//printf("switchcfg register start \r\n");
    status = zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb);

    if(status == ZCL_STA_SUCCESS){
//printf("switchcfg register status  0x%x\r\n", status);
//    	zcl_level_startUpCurrentLevel(endpoint);
    }
//printf("switchcfg register status 2  0x%x\r\n", status);
    return status;

}

#endif /* ZCL_ON_OFF_SWITCH_CFG */


nv_sts_t zcl_onOffCfgAttr_save(void) {
    nv_sts_t st = NV_SUCC;

//    printf("zcl_onOffCfgAttr_save()\r\n");

#ifdef ZCL_ON_OFF_SWITCH_CFG
#if NV_ENABLE
    zcl_onOffSwitchCfgAttr_t zcl_nv_onOffCfg;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&zcl_nv_onOffCfg);

    if(st == NV_SUCC){
        if(zcl_nv_onOffCfg.switchActions != g_zcl_onOffSwitchCfgAttrs.switchActions){
            zcl_nv_onOffCfg.switchActions = g_zcl_onOffSwitchCfgAttrs.switchActions;

            st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&zcl_nv_onOffCfg);
        }

    }else if(st == NV_ITEM_NOT_FOUND){
        zcl_nv_onOffCfg.switchActions = g_zcl_onOffSwitchCfgAttrs.switchActions;

        st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&zcl_nv_onOffCfg);
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

nv_sts_t zcl_onOffCfgAttr_restore(void) {
    nv_sts_t st = NV_SUCC;

//    printf("zcl_onOffCfgAttr_restore()\r\n");

#ifdef ZCL_ON_OFF_SWITCH_CFG
#if NV_ENABLE
    zcl_onOffSwitchCfgAttr_t zcl_nv_onOffCfg;

    st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_ON_OFF, sizeof(zcl_onOffSwitchCfgAttr_t), (u8*)&zcl_nv_onOffCfg);

    if(st == NV_SUCC){
        g_zcl_onOffSwitchCfgAttrs.switchActions = zcl_nv_onOffCfg.switchActions;
    } else {
        g_zcl_onOffSwitchCfgAttrs.switchActions = ZCL_SWITCH_ACTION_ON_OFF;
    }
#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif
#endif

    return st;
}

