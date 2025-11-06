#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

led_config_t led_config;

static void led_on() {
    for (uint8_t i = 0; i < led_config.led_num; i++) {
        drv_gpio_write(led_config.led[i].gpio, led_config.led[i].on);
    }
}

static void led_off() {
    for (uint8_t i = 0; i < led_config.led_num ; i++) {
        drv_gpio_write(led_config.led[i].gpio, led_config.led[i].off);
    }
}

void light_init(void) {
    led_config.led_num = MAX_LED_NUM;
    led_config.led[0].gpio = LED1;
    led_config.led[0].on   = ON;
    led_config.led[0].off  = OFF;
    led_config.led[1].gpio = LED2;
    led_config.led[1].on   = OFF;
    led_config.led[1].off  = ON;
    led_off();

}

int32_t zclLightTimerCb(void *arg) {
    uint32_t interval = 0;

    if(g_appCtx.sta == g_appCtx.oriSta){
        g_appCtx.times--;
        if(g_appCtx.times <= 0){
            g_appCtx.timerLedEvt = NULL;
            return -1;
        }
    }

    g_appCtx.sta = !g_appCtx.sta;
    if(g_appCtx.sta){
        led_on();
        interval = g_appCtx.ledOnTime;
    }else{
        led_off();
        interval = g_appCtx.ledOffTime;
    }

    return interval;
}

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime) {
    uint32_t interval = 0;
    g_appCtx.times = times;

    if(!g_appCtx.timerLedEvt){
        if(g_appCtx.oriSta){
            led_off();
            g_appCtx.sta = 0;
            interval = ledOffTime;
        }else{
            led_on();
            g_appCtx.sta = 1;
            interval = ledOnTime;
        }
        g_appCtx.ledOnTime = ledOnTime;
        g_appCtx.ledOffTime = ledOffTime;

        g_appCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
    }
}

void light_blink_stop(void)
{
    if(g_appCtx.timerLedEvt){
        TL_ZB_TIMER_CANCEL(&g_appCtx.timerLedEvt);

        g_appCtx.times = 0;
        if(g_appCtx.oriSta){
            led_on();
        }else{
            led_off();
        }
    }
}
