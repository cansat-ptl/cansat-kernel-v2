/*
 * listutils.c
 *
 * Created: 03.06.2020 23:13:03
 *  Author: Admin
 */

#include "linkedlists.h"

void utils_listAddBack(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item)
{
	if (list != NULL && item != NULL) {
		item->next = NULL;
		item->prev = list->tail;

		if (list->tail != NULL) {
			list->tail->next = item;
		}

		list->tail = item;

		if (list->head == NULL) {
			list->head = item;
		}

		item->list = list;
	}
	return;
}

void utils_listDropBack(volatile struct kLinkedListStruct_t* list)
{
	volatile struct kListItemStruct_t* next;

	if (list != NULL) {
		if (list->tail != NULL) {
			next = list->tail;
			list->tail = list->tail->prev;

			if (list->tail != NULL) {
				list->tail->next = NULL;
			}

			if (next == list->head) {
				list->head = NULL;
			}

			next->list = NULL;
		}
	}
	return;
}

void utils_listAddFront(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item)
{
	if (list != NULL && item != NULL) {
		item->next = list->head;
		item->prev = NULL;

		if (list->head != NULL) {
			list->head->prev = item;
		}

		list->head = item;

		if (list->tail == NULL) {
			list->tail = item;
		}

		item->list = list;
	}
	return;
}

void utils_listDropFront(volatile struct kLinkedListStruct_t* list)
{
	volatile struct kListItemStruct_t* prev;

	if (list != NULL) {
		if (list->head != NULL) {
			prev = list->head;
			list->head = list->head->next;

			if (list->head != NULL) {
				list->head->prev = NULL;
			}

			if (prev == list->tail) {
				list->tail = NULL;
			}

			prev->list = NULL;
		}
	}
	return;
}

void utils_listDeleteAny(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item)
{
	if (list != NULL && item != NULL) {
		if (list->head == item) {
			utils_listDropFront(list);
		}
		else {
			if (list->tail == item) {
				utils_listDropBack(list);
			}
			else {
				if (item->next != NULL) {
					item->next->prev = item->prev;
				}

				if (item->prev != NULL) {
					item->prev->next = item->next;
				}
				item->list = NULL;
			}
		}
	}

	return;
}