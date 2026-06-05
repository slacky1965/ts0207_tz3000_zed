#ifndef SRC_INCLUDE_APP_IAS_H_
#define SRC_INCLUDE_APP_IAS_H_

void sendIasNotification(zoneStatusChangeNoti_t *noti);
void waterleak_ias(bool leak);
void requestIasEnrollment(void);

#endif /* SRC_INCLUDE_APP_IAS_H_ */
