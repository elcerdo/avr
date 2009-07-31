#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "usbdrv/oddebug.h"
#include "keypad.h"
#include "status.h"
#include "audio.h"
#include "led.h"

int main(void) {
	uint8_t wave = 0x10;
	uint8_t speed = 0x01;
	uint8_t kk = 0x01;
	uint8_t ll = 0;

	wdt_disable();

	cli();

	uint8_t *keypad = NULL;

	odDebugInit();
	keypad_init();
	status_init();
	audio_init();
	led_init();

	keypad = keypad_get();

	sei();

	DBG1(0,NULL,0);

	status_set(1);
	led_send_command(LED_DIGIT0,wave | speed);
	led_send_command(LED_DIGIT1,kk);


	wdt_enable(WDTO_1S);
	while (1) {
		keypad_update();

		cli();
		//select table
		if (bit_is_set(keypad[0],0)) { wave = 0x10; audio_set_table(SIN); audio_on(0X8000); }
		if (bit_is_set(keypad[0],1)) { wave = 0x20; audio_set_table(TRIANGLE); audio_on(0X8000); }
		if (bit_is_set(keypad[0],2)) { wave = 0x40; audio_set_table(RECT); audio_on(0x8000); }
		if (bit_is_set(keypad[0],3)) { wave = 0x80; audio_set_table(NOISE); audio_on(0x8000); }

		//select scale
		if (bit_is_set(keypad[1],0)) { speed = 0x01; audio_set_speed(1); audio_on(0x8000); }
		if (bit_is_set(keypad[1],1)) { speed = 0x02; audio_set_speed(2); audio_on(0x8000); }
		if (bit_is_set(keypad[1],2)) { speed = 0x04; audio_set_speed(4); audio_on(0x8000); }
		if (bit_is_set(keypad[1],3)) { speed = 0x08; audio_set_speed(8); audio_on(0x8000); }
		sei();

		if (++ll == 0xff) {
			if (kk < 0x80) kk <<= 1;
			else kk = 0x01;
			led_send_command(LED_DIGIT1,kk);
		}
		led_send_command(LED_DIGIT0,wave | speed);

		wdt_reset();
	}

	cli();

	keypad_free();
	audio_free();

	return 0;
}

