/*
 * sysio.h
 *
 * Created: 28.04.2020 21:05:48
 *  Author: Admin
 */ 


#ifndef SYSIO_H_
#define SYSIO_H_

#include <kernel/types.h>
#include <kernel/kernel.h>
#include <stdio.h>

struct kIODevStruct_t;
{
	FILE* dev_stdin;
	FILE* dev_stdout;
	FILE* dev_stderr;
};

int sysio_fopen(kSystemIOHandle_t handle, uint8_t flags);

int sysio_read(int fd, void* rbuf, size_t amount);
int sysio_write(int fd, void* wbuf, size_t amount);


#endif /* SYSIO_H_ */