/*
 * avr.h
 *
 * Created: 15.02.2020 19:56:42
 *  Author: Admin
 */


#ifndef AVR_H_
#define AVR_H_

#ifndef F_CPU
#define F_CPU 8000000L						//CPU frequency
#endif

#define AVRDEF_VER "0.0.1-bleeding"
#define AVRDEF_TIMESTAMP __TIMESTAMP__

#include <types.h>
#include <avr/io.h>
#include <avr/iom128.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/common.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <kernel.h>
#include <platform/avr/timers.h>

#define lo8(x) ((x)&0xff)
#define hi8(x) ((x)>>8)

#define platform_DISABLE_INTERRUPTS() asm volatile ("cli"::)
#define platform_ENABLE_INTERRUPTS() asm volatile ("sei"::)

#define platform_STATUS_REG SREG
#define platform_NOP() asm volatile ("nop"::)
#define platform_DELAY_MS(x) _delay_ms(x)

#define platform_RET() asm volatile ("ret \n\t" ::)
#define platform_RETI() asm volatile ("reti \n\t" ::)

#if CFG_USE_RAMP_REGISTERS == 0
#define platform_RESTORE_CONTEXT() asm volatile ( \
	";------Context-Restore------\n\t"\
	"in r0, %[_SPL_]			\n\t" \
	"sts kStackPointer, r0		\n\t" \
	"in r0, %[_SPH_]			\n\t" \
	"sts kStackPointer+1, r0	\n\t" \
	"lds r26, kCurrentTask		\n\t" \
	"lds r27, kCurrentTask+1	\n\t" \
	"ld r0, X+					\n\t" \
	"out %[_SPL_], r0			\n\t" \
	"ld r0, X+					\n\t" \
	"out %[_SPH_], r0			\n\t" \
	"pop r31					\n\t" \
	"pop r30					\n\t" \
	"pop r29					\n\t" \
	"pop r28					\n\t" \
	"pop r27					\n\t" \
	"pop r26					\n\t" \
	"pop r25					\n\t" \
	"pop r24					\n\t" \
	"pop r23					\n\t" \
	"pop r22					\n\t" \
	"pop r21					\n\t" \
	"pop r20					\n\t" \
	"pop r19					\n\t" \
	"pop r18					\n\t" \
	"pop r17					\n\t" \
	"pop r16					\n\t" \
	"pop r15					\n\t" \
	"pop r14					\n\t" \
	"pop r13					\n\t" \
	"pop r12					\n\t" \
	"pop r11					\n\t" \
	"pop r10					\n\t" \
	"pop r9						\n\t" \
	"pop r8						\n\t" \
	"pop r7						\n\t" \
	"pop r6						\n\t" \
	"pop r5						\n\t" \
	"pop r4						\n\t" \
	"pop r3						\n\t" \
	"pop r2						\n\t" \
	"pop r1						\n\t" \
	"sei						\n\t" \
	"pop r0						\n\t" \
	"out %[_SREG_], r0			\n\t" \
	"pop r0						\n\t" \
	";---------------------------\n\t" \
	: \
	:	[_SREG_] "i" _SFR_IO_ADDR(platform_STATUS_REG), \
	[_SPL_] "i" _SFR_IO_ADDR(SPL), \
	[_SPH_] "i" _SFR_IO_ADDR(SPH) \
)

#define platform_SAVE_CONTEXT() asm volatile ( \
	";--------Context-Save-------\n\t"\
	"push r0					\n\t" \
	"in r0, %[_SREG_]			\n\t" \
	"cli						\n\t" \
	"push r0					\n\t" \
	"push r1					\n\t" \
	"clr r1						\n\t" \
	"push r2					\n\t" \
	"push r3					\n\t" \
	"push r4					\n\t" \
	"push r5					\n\t" \
	"push r6					\n\t" \
	"push r7					\n\t" \
	"push r8					\n\t" \
	"push r9					\n\t" \
	"push r10					\n\t" \
	"push r11					\n\t" \
	"push r12					\n\t" \
	"push r13					\n\t" \
	"push r14					\n\t" \
	"push r15					\n\t" \
	"push r16					\n\t" \
	"push r17					\n\t" \
	"push r18					\n\t" \
	"push r19					\n\t" \
	"push r20					\n\t" \
	"push r21					\n\t" \
	"push r22					\n\t" \
	"push r23					\n\t" \
	"push r24					\n\t" \
	"push r25					\n\t" \
	"push r26					\n\t" \
	"push r27					\n\t" \
	"push r28					\n\t" \
	"push r29					\n\t" \
	"push r30					\n\t" \
	"push r31					\n\t" \
	"lds r26, kCurrentTask		\n\t" \
	"lds r27, kCurrentTask+1	\n\t" \
	"in r0, %[_SPL_]			\n\t" \
	"st X+, r0					\n\t" \
	"in r0, %[_SPH_]			\n\t" \
	"st X+, r0					\n\t" \
	"lds r0, kStackPointer		\n\t" \
	"out %[_SPL_], r0			\n\t" \
	"lds r0, kStackPointer+1	\n\t" \
	"out %[_SPH_], r0			\n\t" \
	";---------------------------\n\t" \
	: \
	:	[_SREG_] "i" _SFR_IO_ADDR(platform_STATUS_REG), \
	[_SPL_] "i" _SFR_IO_ADDR(SPL), \
	[_SPH_] "i" _SFR_IO_ADDR(SPH) \
)
#else
#define platform_RESTORE_CONTEXT() asm volatile ( \
	";------Context-Restore------\n\t"\
	"in r0, %[_SPL_]			\n\t" \
	"sts kStackPointer, r0		\n\t" \
	"in r0, %[_SPH_]			\n\t" \
	"sts kStackPointer+1, r0	\n\t" \
	"lds r26, kCurrentTask		\n\t" \
	"lds r27, kCurrentTask+1	\n\t" \
	"ld r0, X+					\n\t" \
	"out %[_SPL_], r0			\n\t" \
	"ld r0, X+					\n\t" \
	"out %[_SPH_], r0			\n\t" \
	"pop r31					\n\t" \
	"pop r30					\n\t" \
	"pop r29					\n\t" \
	"pop r28					\n\t" \
	"pop r27					\n\t" \
	"pop r26					\n\t" \
	"pop r25					\n\t" \
	"pop r24					\n\t" \
	"pop r23					\n\t" \
	"pop r22					\n\t" \
	"pop r21					\n\t" \
	"pop r20					\n\t" \
	"pop r19					\n\t" \
	"pop r18					\n\t" \
	"pop r17					\n\t" \
	"pop r16					\n\t" \
	"pop r15					\n\t" \
	"pop r14					\n\t" \
	"pop r13					\n\t" \
	"pop r12					\n\t" \
	"pop r11					\n\t" \
	"pop r10					\n\t" \
	"pop r9						\n\t" \
	"pop r8						\n\t" \
	"pop r7						\n\t" \
	"pop r6						\n\t" \
	"pop r5						\n\t" \
	"pop r4						\n\t" \
	"pop r3						\n\t" \
	"pop r2						\n\t" \
	"pop r1						\n\t" \
	"pop r0						\n\t" \
	"sts %[_RAMPD_], r0			\n\t" \
	"pop r0						\n\t" \
	"sts %[_RAMPZ_], r0			\n\t" \
	"pop r0						\n\t" \
	"sts %[_RAMPY_], r0			\n\t" \
	"pop r0						\n\t" \
	"sts %[_RAMPX_], r0			\n\t" \
	"pop r0						\n\t" \
	"sts %[_EIND_], r0			\n\t" \
	"sei						\n\t" \
	"pop r0						\n\t" \
	"sts %[_SREG_], r0			\n\t" \
	"pop r0						\n\t" \
	";---------------------------\n\t" \
	: \
	:[_SREG_] "i" _SFR_IO_ADDR(platform_STATUS_REG), \
	[_SPL_] "i" _SFR_IO_ADDR(SPL), \
	[_SPH_] "i" _SFR_IO_ADDR(SPH), \
	[_EIND_] "i" _SFR_IO_ADDR(EIND), \
	[_RAMPX_] "i" _SFR_IO_ADDR(RAMPX), \
	[_RAMPY_] "i" _SFR_IO_ADDR(RAMPY), \
	[_RAMPZ_] "i" _SFR_IO_ADDR(RAMPZ), \
	[_RAMPD_] "i" _SFR_IO_ADDR(RAMPD)
)

#define platform_SAVE_CONTEXT() asm volatile ( \
	";--------Context-Save-------\n\t"\
	"push r0					\n\t" \
	"lds r0, %[_SREG_]			\n\t" \
	"cli						\n\t" \
	"push r0					\n\t" \
	"lds r0, %[_EIND_]			\n\t" \
	"push r0					\n\t" \
	"lds r0, %[_RAMPX_]			\n\t" \
	"push r0					\n\t" \
	"lds r0, %[_RAMPY_]			\n\t" \
	"push r0					\n\t" \
	"lds r0, %[_RAMPZ_]			\n\t" \
	"push r0					\n\t" \
	"lds r0, %[_RAMPD_]			\n\t" \
	"push r0					\n\t" \
	"push r1					\n\t" \
	"clr r1						\n\t" \
	"push r2					\n\t" \
	"push r3					\n\t" \
	"push r4					\n\t" \
	"push r5					\n\t" \
	"push r6					\n\t" \
	"push r7					\n\t" \
	"push r8					\n\t" \
	"push r9					\n\t" \
	"push r10					\n\t" \
	"push r11					\n\t" \
	"push r12					\n\t" \
	"push r13					\n\t" \
	"push r14					\n\t" \
	"push r15					\n\t" \
	"push r16					\n\t" \
	"push r17					\n\t" \
	"push r18					\n\t" \
	"push r19					\n\t" \
	"push r20					\n\t" \
	"push r21					\n\t" \
	"push r22					\n\t" \
	"push r23					\n\t" \
	"push r24					\n\t" \
	"push r25					\n\t" \
	"push r26					\n\t" \
	"push r27					\n\t" \
	"push r28					\n\t" \
	"push r29					\n\t" \
	"push r30					\n\t" \
	"push r31					\n\t" \
	"lds r26, kCurrentTask		\n\t" \
	"lds r27, kCurrentTask+1	\n\t" \
	"in r0, %[_SPL_]			\n\t" \
	"st X+, r0					\n\t" \
	"in r0, %[_SPH_]			\n\t" \
	"st X+, r0					\n\t" \
	"lds r0, kStackPointer		\n\t" \
	"out %[_SPL_], r0			\n\t" \
	"lds r0, kStackPointer+1	\n\t" \
	"out %[_SPH_], r0			\n\t" \
	";---------------------------\n\t" \
	: \
	:[_SREG_] "i" _SFR_IO_ADDR(platform_STATUS_REG), \
	[_SPL_] "i" _SFR_IO_ADDR(SPL), \
	[_SPH_] "i" _SFR_IO_ADDR(SPH), \
	[_EIND_] "i" _SFR_IO_ADDR(EIND), \
	[_RAMPX_] "i" _SFR_IO_ADDR(RAMPX), \
	[_RAMPY_] "i" _SFR_IO_ADDR(RAMPY), \
	[_RAMPZ_] "i" _SFR_IO_ADDR(RAMPZ), \
	[_RAMPD_] "i" _SFR_IO_ADDR(RAMPD)
)
#endif


#define platform_setupSystemTimer() platform_setupTimer0(CFG_KERNEL_TIMER_PRESCALER);
#define platform_startSystemTimer() platform_startTimer0();
#define platform_stopSystemTimer() platform_stopTimer0();

uint8_t platform_prepareStackFrame(kStackPtr_t regionPointer, kTask_t taskPointer, void* args);

#endif /* AVR_H_ */