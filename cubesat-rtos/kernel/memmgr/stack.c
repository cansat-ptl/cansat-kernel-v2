/*
 * stack.c
 *
 * Created: 13.02.2020 22:16:38
 *  Author: Admin
 */

#include <kernel/kernel.h>

static volatile kStackSize_t kUserTaskStackUsage = 0;
static volatile kStackSize_t kSystemStackUsage = 0;

void platform_handleStackCorruption();

//TODO: refactor

kStackPtr_t kernel_getStackPtr()
{
	return NULL; //TODO: remove
}

kStackSize_t kernel_getUserTaskStackUsage()
{
	return kUserTaskStackUsage;
}

kStackSize_t kernel_getSystemTaskStackUsage()
{
	return kSystemStackUsage;
}

void kernel_prepareMemoryBarrier(kStackPtr_t sptr, uint8_t size, uint8_t filler)
{
	for (int16_t i = 0; i > -size; i--)
		sptr[i] = filler;
}

kStackPtr_t kernel_setupTaskStack(kTask_t startupPointer, kStackSize_t taskStackSize, kTaskType_t taskType, void* args)
{
	return NULL;
}

uint8_t kernel_checkStackProtectionRegion(kTaskHandle_t checkedTask)
{
	if (checkedTask == NULL) return 1;

	kStatusRegister_t sreg = threads_startAtomicOperation();

	if (checkedTask -> type != KTASK_SYSTEM) {
		kStackPtr_t stackEnd = checkedTask -> stackBegin - checkedTask -> stackSize - CFG_TASK_STACK_SAFETY_MARGIN;

		for (int16_t i = 1; i < CFG_TASK_STACK_SAFETY_MARGIN+1; i++) {
			if (stackEnd[i] != 0xFE) {
				debug_puts(L_INFO, PSTR("kernel: Stack corruption detected\r\n"));
				platform_handleStackCorruption();
			}
		}
	}

	threads_endAtomicOperation(sreg);
	return 0;
}

