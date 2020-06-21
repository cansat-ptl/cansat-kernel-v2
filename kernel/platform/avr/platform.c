/*
 * port.c
 *
 * Created: 07.06.2020 21:16:38
 *  Author: Admin
 */

#include <kernel/platform/platform.h>
#include <kernel/hal/hal.h>

extern kTaskHandle_t kCurrentTask;
volatile byte kReservedMemory[CFG_KERNEL_RESERVED_MEMORY];
kStackPtr_t kStackPointer = &kReservedMemory[CFG_KERNEL_RESERVED_MEMORY-1];

void tasks_tick();
void tasks_switchTask();

void __attribute__ (( naked, noinline )) platform_yield(void)
{
	platform_SAVE_CONTEXT();
	tasks_switchTask();
	platform_RESTORE_CONTEXT();
	platform_RET();
}

void __attribute__ (( naked, noinline )) platform_tick()
{
	platform_SAVE_CONTEXT();

	tasks_tick();

	platform_RESTORE_CONTEXT();
	platform_RET();
}

kStatusRegister_t platform_startAtomicOperation()
{
	#if CFG_ALLOW_INTERRUPTS_IN_ATOMIC_SECTIONS == 0
		kStatusRegister_t sreg = platform_STATUS_REG;
		platform_DISABLE_INTERRUPTS();
		return sreg;
	#else
		return 0;
	#endif
}

void platform_endAtomicOperation(kStatusRegister_t sreg)
{
	#if CFG_ALLOW_INTERRUPTS_IN_ATOMIC_SECTIONS == 0
		platform_ENABLE_INTERRUPTS();
		platform_STATUS_REG = sreg;
	#endif
}

void platform_spinlockAcquire(kSpinlock_t* spinlock)
{
	while(1) {
		asm volatile("": : :"memory");
		if(*spinlock == 0) {
			kStatusRegister_t sreg = platform_startAtomicOperation();
			if(*spinlock == 0) {
				*spinlock = 1;
				platform_endAtomicOperation(sreg);
				return;
			}
			platform_endAtomicOperation(sreg);
		}
	}
}

void platform_spinlockRelease(kSpinlock_t* spinlock)
{
	kStatusRegister_t sreg = platform_startAtomicOperation();
	*spinlock = 0;
	platform_endAtomicOperation(sreg);
}