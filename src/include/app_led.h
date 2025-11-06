#ifndef SRC_INCLUDE_APP_LED_H_
#define SRC_INCLUDE_APP_LED_H_

typedef struct {
    uint32_t gpio;
    uint8_t  on;
    uint8_t  off;
} led_t;

typedef struct {
    led_t    led[MAX_LED_NUM];
    uint8_t  led_num;
} led_config_t;

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime);
void light_blink_stop(void);

void light_init(void);
//void light_on(void);
//void light_off(void);

#endif /* SRC_INCLUDE_APP_LED_H_ */
