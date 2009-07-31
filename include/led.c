#include "led.h"

//
// LEDS
//

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

