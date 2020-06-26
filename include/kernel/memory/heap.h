/*
 * heap.h
 *
 * Created: 21.06.2020 21:06:46
 *  Author: Admin
 */


#ifndef HEAP_H_
#define HEAP_H_

void* memory_heapAlloc(size_t size);
void memory_heapFree(void* pointer);

size_t memory_getFreeHeap();
size_t memory_getFreeHeapMin();

#endif /* HEAP_H_ */