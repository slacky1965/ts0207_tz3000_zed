#ifndef SRC_COMMON_BOARD_SNZB_05_H_
#define SRC_COMMON_BOARD_SNZB_05_H_

#if (BOARD == BOARD_SNZB_05)

#define ZCL_BASIC_MFG_NAME     {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID     {13,'S','N','Z','B','-','0','5','-','z','-','S','l','D'}

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PB1
#define DEBUG_BAUDRATE          115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN      GPIO_PC5

/***************************** Configure LED  ***************************************/

#define LED_ON                  1
#define LED_OFF                 0
#define LED_STATUS              GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       ON
#define PB4_INPUT_ENABLE        OFF
#define PB4_DATA_OUT            OFF

//#define LED_POWER               GPIO_PA0
//#define PA0_FUNC                AS_GPIO
//#define PA0_OUTPUT_ENABLE       ON
//#define PA0_INPUT_ENABLE        OFF
//
//#define LED_PERMIT              LED_STATUS
#define LED1                    LED_STATUS


/************************* Configure LEAK GPIO **************************************/
#define WLEAK_GPIO              GPIO_PD7
#define PD7_INPUT_ENABLE        ON
#define PD7_DATA_OUT            OFF
#define PD7_OUTPUT_ENABLE       OFF
#define PD7_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PD7     PM_PIN_PULLDOWN_100K //PM_PIN_PULLUP_1M //PM_PIN_PULLUP_10K //

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON1                 GPIO_PD3
#define PD3_FUNC                AS_GPIO
#define PD3_OUTPUT_ENABLE       OFF
#define PD3_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PD3     PM_PIN_PULLUP_1M

#define PM_WAKEUP_LEVEL         PM_WAKEUP_LEVEL_LOW // only for KEY

enum {
    VK_SW1 = 0x01,
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1}


#endif /* (BOARD == BOARD_SNZB_05) */

#endif /* SRC_COMMON_BOARD_SNZB_05_H_ */
