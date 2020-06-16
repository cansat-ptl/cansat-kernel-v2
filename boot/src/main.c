/*
 * yktsat-boot.c
 *
 * Created: 05.03.2020 19:43:09
 * Author : Admin
 */

#define SIGRD 5

#include <boot/boot.h>
#include <avr/signature.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <dev/basic/uart.h>
#include <dev/basic/gpio.h>

const char boot_msg_test[] PROGMEM = "boot: Test string %d %d\r\n";
const char boot_msg_start[] PROGMEM = "boot: Starting up yktsat-boot version %s built %s\r\n\r\n";
//const char boot_msg_sign[] PROGMEM = "boot: MCU signature: 0x%02X 0x%02X 0x%02X\r\n";
const char boot_msg_sign1[] PROGMEM = "boot: MCU signature: 0x%02X 0x%02X 0x%02X\r\n";
const char boot_msg_fuses[] PROGMEM = "boot: Fuse bits: 0x%02X 0x%02X 0x%02X\r\n";
const char boot_msg_locks[] PROGMEM = "boot: Lock bits: 0x%02X\r\n\r\n";
const char boot_msg_ready[] PROGMEM = "boot: Ready to accept instructions\r\n";
const char boot_msg_waiting[] PROGMEM = "boot: Waiting 10 seconds before OS startup\r\n";


int main(void)
{
	basicUart_init(12);
	boot_logMessage_p(0, pgm_get_far_address(boot_msg_start), BOOT_VER, BOOT_TIMESTAMP);
//	__signature[3];
	uint32_t sign0, sign1, sign2;
	sign0 = boot_signature_byte_get(0x00);
	sign1 = boot_signature_byte_get(0x02);
	sign2 = boot_signature_byte_get(0x04);

	boot_logMessage_p(0, pgm_get_far_address(boot_msg_sign1), __signature[2], __signature[1], __signature[0]);
	//boot_logMessage_p(0, pgm_get_far_address(boot_msg_sign), sign0, sign1, sign2);
	boot_checkSignature(__signature[2], __signature[1], __signature[0]);

	uint8_t fuse0, fuse1, fuse2, lock;
	fuse0 = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
	lock = boot_lock_fuse_bits_get(GET_LOCK_BITS);
	fuse1 = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
	fuse2 = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);

	boot_logMessage_p(0, pgm_get_far_address(boot_msg_fuses), fuse0, fuse1, fuse2);
	boot_logMessage_p(0, pgm_get_far_address(boot_msg_locks), lock);
	boot_logMessage_p(0, pgm_get_far_address(boot_msg_ready));
	boot_logMessage_p(0, pgm_get_far_address(boot_msg_waiting));

	uint8_t cnt = 0;
	
    //while(cnt != 100) {
	//	_delay_ms(100);
	//	cnt++;
	//}
	
	/* Enable change of interrupt vectors */
	MCUCR = (1 << IVCE);
	/* Move interrupts to boot flash section */
	MCUCR = (1 << IVSEL);
	
	basicUart_enableInterruptsRX();
	sei();
	
	boot_runStateMachine();

    ((void(*)(void))0)();
}

