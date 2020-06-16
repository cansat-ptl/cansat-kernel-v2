/*
 * listutils.c
 *
 * Created: 03.06.2020 23:13:03
 *  Author: Admin
 */ 

#include "../kerneldefs.h"
#include "listutils.h"

void taskmgr_listAddBack(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task)
{
	if (list != NULL && task != NULL) {
		task->taskList.next = NULL;
		task->taskList.prev = list->tail;
		
		if (list->tail != NULL) {
			list->tail->taskList.next = task;
		}
		
		list->tail = task;
		
		if (list->head == NULL) {
			list->head = task;
		}
		
		task->taskList.list = list;
	}
	return;
}

void taskmgr_listDropBack(volatile struct kLinkedListStruct_t* list) 
{
	kTaskHandle_t next;
	
	if (list != NULL) {
		if (list->tail != NULL) {
			next = list->tail;
			list->tail = list->tail->taskList.prev;
			
			if (list->tail != NULL) {
				list->tail->taskList.next = NULL;
			}
			
			if (next == list->head) {
				list->head = NULL;
			}
			
			next->taskList.list = NULL;
		}
	}
	return;
}

void taskmgr_listAddFront(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task)
{
	if (list != NULL && task != NULL) {
		task->taskList.next = list->head;
		task->taskList.prev = NULL;
	
		if (list->head != NULL) {
			list->head->taskList.prev = task;
		}
	
		list->head = task;
	
		if (list->tail == NULL) {
			list->tail = task;
		}
	
		task->taskList.list = list;
	}
	return;
}

void taskmgr_listDropFront(volatile struct kLinkedListStruct_t* list)
{
	kTaskHandle_t prev;
	
	if (list != NULL) {
		if (list->head != NULL) {
			prev = list->head;
			list->head = list->head->taskList.next;
			
			if (list->head != NULL) {
				list->head->taskList.prev = NULL;
			}
			
			if (prev == list->tail) {
				list->tail = NULL;
			}
			
			prev->taskList.list = NULL;
		}
	}
	return;
}

void taskmgr_listDeleteAny(volatile struct kLinkedListStruct_t* list, kTaskHandle_t task)
{
	if (list != NULL && task != NULL) {
		if (list->head == task) {
			taskmgr_listDropFront(list);
		}
		else {
			if (list->tail == task) {
				taskmgr_listDropBack(list);
			}
			else {
				if (task->taskList.next != NULL) {
					task->taskList.next->taskList.prev = task->taskList.prev;
				}

				if (task->taskList.prev != NULL) {
					task->taskList.prev->taskList.next = task->taskList.next;
				}
				task->taskList.list = NULL;
			}
		}
	}
	
	return;
}