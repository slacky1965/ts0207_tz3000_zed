#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PB1
#define DEBUG_BAUDRATE          115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN      GPIO_PC5

/***************************** Configure LED  ***************************************/

#define MAX_LED_NUM             2

#define LED_ON                  1
#define LED_OFF                 0

/* board zg_222za and snzb_05 */
#define LED1                    GPIO_PB4
#define PB4_FUNC                AS_GPIO
#define PB4_OUTPUT_ENABLE       ON
#define PB4_INPUT_ENABLE        OFF
#define PB4_DATA_OUT            OFF         // led off 0

/* board zg_222z */
#define LED2                    GPIO_PC4
#define PC4_FUNC                AS_GPIO
#define PC4_OUTPUT_ENABLE       ON
#define PC4_INPUT_ENABLE        OFF
#define PC4_DATA_OUT            ON          // led off 1

/************************* Configure LEAK GPIO **************************************/

/* board zg_222za and snzb_05 */
#define WLEAK1                  GPIO_PD7
#define PD7_INPUT_ENABLE        ON
#define PD7_DATA_OUT            OFF
#define PD7_OUTPUT_ENABLE       OFF
#define PD7_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PD7     PM_PIN_PULLDOWN_100K //PM_PIN_PULLUP_1M

/* board zg_222z */
#define WLEAK2                  GPIO_PB6
#define PB6_INPUT_ENABLE        ON
#define PB6_DATA_OUT            OFF
#define PB6_OUTPUT_ENABLE       OFF
#define PB6_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PB6     PM_PIN_PULLUP_10K //PM_PIN_PULLDOWN_100K //PM_PIN_PULLUP_1M

/************************* Configure KEY GPIO ***************************************/

#define MAX_BUTTON_NUM  3

/* board zg_222za */
#define BUTTON1                 GPIO_PC0
#define PC0_FUNC                AS_GPIO
#define PC0_OUTPUT_ENABLE       OFF
#define PC0_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PC0     PM_PIN_PULLUP_1M

/* board zg_222z */
#define BUTTON2                 GPIO_PC2
#define PC2_FUNC                AS_GPIO
#define PC2_OUTPUT_ENABLE       OFF
#define PC2_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_1M

/* board snzb_05 */
#define BUTTON3                 GPIO_PD3
#define PD3_FUNC                AS_GPIO
#define PD3_OUTPUT_ENABLE       OFF
#define PD3_INPUT_ENABLE        ON
#define PULL_WAKEUP_SRC_PD3     PM_PIN_PULLUP_1M

#define PM_WAKEUP_LEVEL         PM_WAKEUP_LEVEL_LOW // only for KEY

enum {
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03
};

#define KB_MAP_NORMAL   {\
        {VK_SW1,}, \
        {VK_SW2,}, \
        {VK_SW3,}}

#define KB_MAP_NUM      KB_MAP_NORMAL
#define KB_MAP_FN       KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1, BUTTON2, BUTTON3}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
