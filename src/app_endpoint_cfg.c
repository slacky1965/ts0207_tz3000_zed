#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#ifndef ZCL_BASIC_MFG_NAME
#define ZCL_BASIC_MFG_NAME          {6,'T','E','L','I','N','K'}
#endif
#ifndef ZCL_BASIC_MODEL_ID
#define ZCL_BASIC_MODEL_ID          {8,'T','L','S','R','8','2','x','x'}
#endif
#ifndef ZCL_BASIC_SW_BUILD_ID
#define ZCL_BASIC_SW_BUILD_ID       {10,'0','1','2','2','0','5','2','0','1','7'}
#endif

#define R               ACCESS_CONTROL_READ
#define RW              ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE
#define RR              ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE
#define RWR             ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE | ACCESS_CONTROL_REPORTABLE

#define ZCL_UINT8       ZCL_DATA_TYPE_UINT8
#define ZCL_INT8        ZCL_DATA_TYPE_INT8
#define ZCL_UINT16      ZCL_DATA_TYPE_UINT16
#define ZCL_INT16       ZCL_DATA_TYPE_INT16
#define ZCL_UINT32      ZCL_DATA_TYPE_UINT32
#define ZCL_ENUM8       ZCL_DATA_TYPE_ENUM8
#define ZCL_ENUM16      ZCL_DATA_TYPE_ENUM16
#define ZCL_BOOLEAN     ZCL_DATA_TYPE_BOOLEAN
#define ZCL_BITMAP8     ZCL_DATA_TYPE_BITMAP8
#define ZCL_BITMAP16    ZCL_DATA_TYPE_BITMAP16
#define ZCL_CHAR_STR    ZCL_DATA_TYPE_CHAR_STR
#define ZCL_UTC         ZCL_DATA_TYPE_UTC
#define ZCL_SINGLE      ZCL_DATA_TYPE_SINGLE_PREC
#define ZCL_IEEE_ADDR   ZCL_DATA_TYPE_IEEE_ADDR

/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const uint16_t app_ep1_inClusterList[] =
{
    ZCL_CLUSTER_GEN_BASIC,
    ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_POLL_CTRL
    ZCL_CLUSTER_GEN_POLL_CONTROL,
#endif
    ZCL_CLUSTER_GEN_POWER_CFG,
#ifdef ZCL_ON_OFF_SWITCH_CFG
    ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,
#endif
#ifdef ZCL_IAS_ZONE
    ZCL_CLUSTER_SS_IAS_ZONE,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const uint16_t app_ep1_outClusterList[] =
{
#ifdef ZCL_GROUP
    ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
    ZCL_CLUSTER_GEN_SCENES,
#endif
#ifdef ZCL_OTA
    ZCL_CLUSTER_OTA,
#endif
#ifdef ZCL_ZLL_COMMISSIONING
    ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
#ifdef ZCL_ON_OFF
    ZCL_CLUSTER_GEN_ON_OFF,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define APP_EP1_IN_CLUSTER_NUM   (sizeof(app_ep1_inClusterList)/sizeof(app_ep1_inClusterList[0]))
#define APP_EP1_OUT_CLUSTER_NUM  (sizeof(app_ep1_outClusterList)/sizeof(app_ep1_outClusterList[0]))


/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t app_ep1Desc = {
    HA_PROFILE_ID,                          /* Application profile identifier */
    HA_DEV_IAS_ZONE,                        /* Application device identifier */
    APP_ENDPOINT1,                          /* Endpoint */
    2,                                      /* Application device version */
    0,                                      /* Reserved */
    APP_EP1_IN_CLUSTER_NUM,                 /* Application input cluster count */
    APP_EP1_OUT_CLUSTER_NUM,                /* Application output cluster count */
    (uint16_t *)app_ep1_inClusterList,      /* Application input cluster list */
    (uint16_t *)app_ep1_outClusterList,     /* Application output cluster list */
};


/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
{
    .zclVersion     = 0x03,
    .appVersion     = APP_RELEASE,
    .stackVersion   = (STACK_RELEASE|STACK_BUILD),
    .hwVersion      = HW_VERSION,
    .manuName       = ZCL_BASIC_MFG_NAME,
    .modelId        = ZCL_BASIC_MODEL_ID,
    .dateCode       = ZCL_BASIC_DATE_CODE,
    .powerSource    = POWER_SOURCE_BATTERY,
    .swBuildId      = ZCL_BASIC_SW_BUILD_ID,
    .deviceEnable   = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
{
    { ZCL_ATTRID_BASIC_ZCL_VER,             ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.zclVersion},
    { ZCL_ATTRID_BASIC_APP_VER,             ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.appVersion},
    { ZCL_ATTRID_BASIC_STACK_VER,           ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.stackVersion},
    { ZCL_ATTRID_BASIC_HW_VER,              ZCL_UINT8,    R,  (uint8_t*)&g_zcl_basicAttrs.hwVersion},
    { ZCL_ATTRID_BASIC_MFR_NAME,            ZCL_CHAR_STR, R,  (uint8_t*)g_zcl_basicAttrs.manuName},
    { ZCL_ATTRID_BASIC_MODEL_ID,            ZCL_CHAR_STR, R,  (uint8_t*)g_zcl_basicAttrs.modelId},
    { ZCL_ATTRID_BASIC_DATE_CODE,           ZCL_CHAR_STR, R,  (uint8_t*)g_zcl_basicAttrs.dateCode},
    { ZCL_ATTRID_BASIC_POWER_SOURCE,        ZCL_ENUM8,    R,  (uint8_t*)&g_zcl_basicAttrs.powerSource},
    { ZCL_ATTRID_BASIC_DEV_ENABLED,         ZCL_BOOLEAN,  RW, (uint8_t*)&g_zcl_basicAttrs.deviceEnable},
    { ZCL_ATTRID_BASIC_SW_BUILD_ID,         ZCL_CHAR_STR, R,  (uint8_t*)&g_zcl_basicAttrs.swBuildId},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,  (uint8_t*)&zcl_attr_global_clusterRevision},

};

#define ZCL_BASIC_ATTR_NUM       sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)


/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
{
    .identifyTime   = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
{
    { ZCL_ATTRID_IDENTIFY_TIME,             ZCL_UINT16,   RW,   (uint8_t*)&g_zcl_identifyAttrs.identifyTime },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,   R,    (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM           sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

/* power */
zcl_powerAttr_t g_zcl_powerAttrs =
{
    .batteryVoltage    = 30,   //in 100 mV units, 0xff - unknown
    .batteryPercentage = 0x64, //in 0,5% units, 0xff - unknown
};

const zclAttrInfo_t powerCfg_attrTbl[] =
{
    { ZCL_ATTRID_BATTERY_VOLTAGE,               ZCL_UINT8,  RR, (uint8_t*)&g_zcl_powerAttrs.batteryVoltage},
    { ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING,  ZCL_UINT8,  RR, (uint8_t*)&g_zcl_powerAttrs.batteryPercentage},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,       ZCL_UINT16, R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_POWER_CFG_ATTR_NUM       sizeof(powerCfg_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs =
{
    .nameSupport    = 0,
};

const zclAttrInfo_t group_attrTbl[] =
{
    { ZCL_ATTRID_GROUP_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_groupAttrs.nameSupport},

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_GROUP_ATTR_NUM    sizeof(group_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs = {
    .sceneCount     = 0,
    .currentScene   = 0,
    .currentGroup   = 0x0000,
    .sceneValid     = FALSE,
    .nameSupport    = 0,
};

const zclAttrInfo_t scene_attrTbl[] = {
    { ZCL_ATTRID_SCENE_SCENE_COUNT,         ZCL_UINT8,      R,  (uint8_t*)&g_zcl_sceneAttrs.sceneCount   },
    { ZCL_ATTRID_SCENE_CURRENT_SCENE,       ZCL_UINT8,      R,  (uint8_t*)&g_zcl_sceneAttrs.currentScene },
    { ZCL_ATTRID_SCENE_CURRENT_GROUP,       ZCL_UINT16,     R,  (uint8_t*)&g_zcl_sceneAttrs.currentGroup },
    { ZCL_ATTRID_SCENE_SCENE_VALID,         ZCL_BOOLEAN,    R,  (uint8_t*)&g_zcl_sceneAttrs.sceneValid   },
    { ZCL_ATTRID_SCENE_NAME_SUPPORT,        ZCL_BITMAP8,    R,  (uint8_t*)&g_zcl_sceneAttrs.nameSupport  },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (uint8_t*)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE_ATTR_NUM   sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_IAS_ZONE
/* IAS Zone */
zcl_iasZoneAttr_t g_zcl_iasZoneAttrs =
{
    .zoneState      = ZONE_STATE_NOT_ENROLLED,
    .zoneType       = ZONE_TYPE_WATER_SENSOR,
    .zoneStatus     = ZONE_STATUS_BIT_RESTORE_NOTIFY,
    .iasCieAddr     = {0x00},
    .zoneId         = ZCL_ZONE_ID_INVALID,
};

const zclAttrInfo_t iasZone_attrTbl[] =
{
    { ZCL_ATTRID_ZONE_STATE,                ZCL_ENUM8,      R,  (u8*)&g_zcl_iasZoneAttrs.zoneState },
    { ZCL_ATTRID_ZONE_TYPE,                 ZCL_ENUM16,     R,  (u8*)&g_zcl_iasZoneAttrs.zoneType },
    { ZCL_ATTRID_ZONE_STATUS,               ZCL_BITMAP16,   R,  (u8*)&g_zcl_iasZoneAttrs.zoneStatus },
    { ZCL_ATTRID_IAS_CIE_ADDR,              ZCL_IEEE_ADDR,  RW, (u8*)g_zcl_iasZoneAttrs.iasCieAddr },
    { ZCL_ATTRID_ZONE_ID,                   ZCL_UINT8,      R,  (u8*)&g_zcl_iasZoneAttrs.zoneId},
    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16,     R,  (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_IASZONE_ATTR_NUM         sizeof(iasZone_attrTbl) / sizeof(zclAttrInfo_t)

#endif

#ifdef ZCL_ON_OFF_SWITCH_CFG
/* On/Off Config */

zcl_onOffSwitchCfgAttr_t g_zcl_onOffSwitchCfgAttrs = {
    .switchType       = ZCL_SWITCH_TYPE_TOGGLE,
    .switchActions    = ZCL_SWITCH_ACTION_ON_OFF,
};

const zclAttrInfo_t onoff_switch_cfg_attrTbl[] =
{
    { ZCL_ATTRID_SWITCH_TYPE,               ZCL_ENUM8,  R,  (u8*)&g_zcl_onOffSwitchCfgAttrs.switchType    },
    { ZCL_ATTRID_SWITCH_ACTION,             ZCL_ENUM8,  RW, (u8*)&g_zcl_onOffSwitchCfgAttrs.switchActions },

    { ZCL_ATTRID_GLOBAL_CLUSTER_REVISION,   ZCL_UINT16, R,  (u8*)&zcl_attr_global_clusterRevision},
};

#define ZCL_ON_OFF_SWITCH_CFG_ATTR_NUM       sizeof(onoff_switch_cfg_attrTbl) / sizeof(zclAttrInfo_t)

#endif //ZCL_ON_OFF_SWITCH_CFG

/**
 *  @brief Definition for simple switch ZCL specific cluster
 */
const zcl_specClusterInfo_t g_appEp1ClusterList[] = {
    {ZCL_CLUSTER_GEN_BASIC,                  MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM,             basic_attrTbl,             zcl_basic_register,                     app_basicCb         },
    {ZCL_CLUSTER_GEN_IDENTIFY,               MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM,          identify_attrTbl,          zcl_identify_register,                  app_identifyCb      },
    {ZCL_CLUSTER_GEN_POWER_CFG,              MANUFACTURER_CODE_NONE, ZCL_POWER_CFG_ATTR_NUM,         powerCfg_attrTbl,          zcl_powerCfg_register,                  app_powerCfgCb      },
#ifdef ZCL_GROUP
    {ZCL_CLUSTER_GEN_GROUPS,                 MANUFACTURER_CODE_NONE, ZCL_GROUP_ATTR_NUM,             group_attrTbl,             zcl_group_register,                     NULL                },
#endif
#ifdef ZCL_SCENE
    {ZCL_CLUSTER_GEN_SCENES,                 MANUFACTURER_CODE_NONE, ZCL_SCENE_ATTR_NUM,             scene_attrTbl,             zcl_scene_register,                     app_sceneCb         },
#endif
#ifdef ZCL_POLL_CTRL
    {ZCL_CLUSTER_GEN_POLL_CONTROL,           MANUFACTURER_CODE_NONE, ZCL_POLLCTRL_ATTR_NUM,          pollCtrl_attrTbl,          zcl_pollCtrl_register,                  app_pollCtrlCb      },
#endif
#ifdef ZCL_ON_OFF_SWITCH_CFG
    {ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG,   MANUFACTURER_CODE_NONE, ZCL_ON_OFF_SWITCH_CFG_ATTR_NUM, onoff_switch_cfg_attrTbl,  zcl_onOffSwitchCfg_register,            NULL                },
#endif
    {ZCL_CLUSTER_SS_IAS_ZONE,   MANUFACTURER_CODE_NONE, ZCL_IASZONE_ATTR_NUM,   iasZone_attrTbl,    zcl_iasZone_register,   leak_iasZoneCb},
};

uint8_t APP_EP1_CB_CLUSTER_NUM = (sizeof(g_appEp1ClusterList)/sizeof(g_appEp1ClusterList[0]));

