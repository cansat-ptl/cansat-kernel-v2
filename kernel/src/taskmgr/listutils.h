/*
 * listutils.h
 *
 * Created: 04.06.2020 17:09:08
 *  Author: Admin
 */ 


#ifndef LISTUTILS_H_
#define LISTUTILS_H_

#include <kernel/kernel.h>

void taskmgr_listAddBack(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task);
void taskmgr_listDropBack(volatile struct kLinkedListStruct_t* list);
void taskmgr_listAddFront(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task);
void taskmgr_listDropFront(volatile struct kLinkedListStruct_t* list);
void taskmgr_listDeleteAny(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task);

#endif /* LISTUTILS_H_ */