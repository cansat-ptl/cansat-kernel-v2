/*
 * notifications.h
 *
 * Created: 18.06.2020 0:30:15
 *  Author: Admin
 */ 


#ifndef NOTIFICATIONS_H_
#define NOTIFICATIONS_H_

uint16_t threads_notificationWait();
kReturnValue_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags);

#endif /* NOTIFICATIONS_H_ */