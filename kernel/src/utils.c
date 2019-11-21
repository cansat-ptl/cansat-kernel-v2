/*
 * utils.c
 *
 * Created: 23.06.2019 10:27:23
 *  Author: WorldSkills-2019
 */ 
#include "../kernel.h"

uint8_t util_strCompare(char * a, char * b, uint8_t len)
{
	unsigned char i;
	for(i=0;i<len;i++){
		if(a[i]!=b[i]) return 0;
	}
	return 1;
}