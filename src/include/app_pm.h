#ifndef SRC_INCLUDE_APP_PM_H_
#define SRC_INCLUDE_APP_PM_H_

#define APP_ANALOG_REG          DEEP_ANA_REG1   // analog register
#define TIME_LONG_DEEP_SLEEP    3600             // Time of long deep sleep in sec. 3600

#if PM_ENABLE

void app_wakeupPinConfig();
void app_wakeupPinLevelChange();
void app_lowPowerEnter();

#endif

void app_set_analog_reg(uint8_t *reg_data);
uint8_t app_get_analog_reg();

#endif /* SRC_INCLUDE_APP_PM_H_ */
