/*
 * listutils.h
 *
 * Created: 04.06.2020 17:09:08
 *  Author: Admin
 */


#ifndef LISTUTILS_H_
#define LISTUTILS_H_

#include <stddef.h>
#include <kernel/ktypes.h>

struct kLinkedListStruct_t
{
	volatile struct kListItemStruct_t* head;
	volatile struct kListItemStruct_t* tail;
};

struct kListItemStruct_t
{
	volatile struct kLinkedListStruct_t* list;
	volatile struct kListItemStruct_t* next;
	volatile struct kListItemStruct_t* prev;
};

#define utils_LISTITEM_STRUCT_SIZE ((sizeof(struct kListItemStruct_t) + ((size_t)(CFG_PLATFORM_BYTE_ALIGNMENT - 1))) & ~((size_t)CFG_PLATFORM_BYTE_ALIGNMENT_MASK))

void utils_listAddBack(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item);
void utils_listDropBack(volatile struct kLinkedListStruct_t* list);
void utils_listAddFront(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item);
void utils_listDropFront(volatile struct kLinkedListStruct_t* list);
void utils_listDeleteAny(volatile struct kLinkedListStruct_t* list, volatile struct kListItemStruct_t* item);

#endif /* LISTUTILS_H_ */