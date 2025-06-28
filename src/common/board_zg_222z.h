#ifndef SRC_COMMON_BOARD_ZG_222Z_H_
#define SRC_COMMON_BOARD_ZG_222Z_H_

#define CHIP_FLASH_SIZE 512

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
#define LED_STATUS              GPIO_PD7
#define PD7_FUNC                AS_GPIO
#define PD7_OUTPUT_ENABLE       LED_ON
#define PD7_INPUT_ENABLE        LED_OFF
#define PD7_DATA_OUT            LED_OFF

//#define LED_POWER               GPIO_PA0
//#define PA0_FUNC                AS_GPIO
//#define PA0_OUTPUT_ENABLE       ON
//#define PA0_INPUT_ENABLE        OFF
//
//#define LED_PERMIT              LED_STATUS
#define LED1                    LED_STATUS


/************************* Configure LEAK GPIO **************************************/
#define WLEAK_GPIO              GPIO_PD3
#define PD3_INPUT_ENABLE        ON
#define PD3_DATA_OUT            OFF
#define PD3_OUTPUT_ENABLE       OFF
#define PD3_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PD3     PM_PIN_PULLDOWN_100K //PM_PIN_PULLUP_1M

/************************* Configure KEY GPIO ***************************************/
#define MAX_BUTTON_NUM  1

#define BUTTON1                 GPIO_PC2
#define PC2_FUNC                AS_GPIO
#define PC2_OUTPUT_ENABLE       OFF
#define PC2_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_1M

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

#endif /* SRC_COMMON_BOARD_ZG_222Z_H_ */
