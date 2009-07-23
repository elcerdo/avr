#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "usbdrv/oddebug.h"
#include "keypad.h"
#include "status.h"
#include "audio.h"

//
// LEDS
//

#define PLCLOCK PC0
#define PLLOAD  PC1
#define PLDATA  PC6

#define LED_SHUTDOWN     0x0c
#define LED_SHUTDOWN_ON  0x01
#define LED_SHUTDOWN_OFF 0x00
#define LED_TEST         0x0f
#define LED_TEST_ON      0x01
#define LED_TEST_OFF     0x00
#define LED_LIMIT        0x0b

#define LED_DECODE       0x09
#define LED_DIGIT0       0x01
#define LED_DIGIT1       0x02

void led_valid_clock(void) {
	PORTC |= _BV(PLCLOCK);
	PORTC &= ~_BV(PLCLOCK);
}

void led_send_command(uint8_t address,uint8_t data) {
	uint8_t k,l;
	PORTC &= ~_BV(PLLOAD);

	l = 0;
	k = 8;
	do {
		k--;
		l++;

		if ( address & _BV(k) ) PORTC |= _BV(PLDATA);
		else PORTC &= ~_BV(PLDATA);

		led_valid_clock();
	} while ( k > 0);

	k = 8;
	do {
		k--;
		l++;

		if ( data & _BV(k) ) PORTC |= _BV(PLDATA);
		else PORTC &= ~_BV(PLDATA);

		led_valid_clock();
	} while ( k > 0);

	PORTC |= _BV(PLLOAD);
}

void led_init(void) {
	DDRC |= _BV(PLCLOCK) | _BV(PLDATA) | _BV(PLLOAD);
	PORTC |= _BV(PLLOAD);

	led_send_command(LED_SHUTDOWN,LED_SHUTDOWN_OFF);
	led_send_command(LED_TEST,LED_TEST_OFF);
	led_send_command(LED_LIMIT,2);
	led_send_command(LED_DECODE,0);
	led_send_command(LED_SHUTDOWN,LED_SHUTDOWN_ON);
}

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

