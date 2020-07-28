/*
 * flash.c
 *
 * Created: 18.03.2020 16:00:36
 *  Author: Admin
 */
#include <boot/boot.h>
#include <avr/io.h>
#include <avr/boot.h>

void boot_programPage(uint32_t page, uint8_t *buf)
{
	uint8_t sreg = SREG;
	cli();

	eeprom_busy_wait();
	boot_page_erase(page);
	boot_spm_busy_wait();

	for (uint16_t i = 0; i < SPM_PAGESIZE; i += 2) {
		uint16_t w = *buf++;
		w += (*buf++) << 8;
		boot_page_fill(page + i, w);
	}

	boot_page_write(page);
	boot_spm_busy_wait();

	boot_rww_enable();
	sei();
	SREG = sreg;
}