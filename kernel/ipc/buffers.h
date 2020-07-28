/*
 * buffers.h
 *
 * Created: 21.06.2020 19:55:32
 *  Author: Admin
 */


#ifndef BUFFERS_H_
#define BUFFERS_H_

struct kIPCStruct_t
{
	char* pointer;
	size_t itemSize;
	size_t size;
	size_t inputPosition;
	size_t outputPosition;
	size_t currentPosition;
};

#endif /* BUFFERS_H_ */