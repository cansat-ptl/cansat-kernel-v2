/*
 * notifications.h
 *
 * Created: 16.06.2020 16:36:09
 *  Author: Admin
 */ 


#ifndef NOTIFICATIONS_H_
#define NOTIFICATIONS_H_

#include <stdint.h>
#include <tasks.h>

uint16_t threads_notificationWait();
kReturnValue_t threads_notificationSend(kTaskHandle_t taskToNotify, uint16_t flags);

#endif /* NOTIFICATIONS_H_ */