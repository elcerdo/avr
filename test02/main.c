#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "status.h"

#define PAUDIO PB3

static uint8_t tone = 0x80;

ISR(TIMER0_COMP_vect) {
	if (OCR0 < tone) OCR0++;
	if (OCR0 > tone) OCR0--;
}

int main(void) {
	uint32_t l;
	uint8_t *keypad = NULL;

	cli();
	
    keypad_init();
	status_init();
	keypad = keypad_get();

	//set pins as output
	DDRB |= _BV(PAUDIO);


	//setup timer 0
	TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00) | _BV(COM00);
	OCR0 = tone;
	TIMSK = _BV(OCIE0);

	sei();

	while (1) {
		keypad_update();

		//display keypad
		uint8_t i = (keypad[0] & 0x0f) | ((keypad[1] & 0x0f) << 4);
		keypad_load_value(~i);

		//select tone
		if (bit_is_set(keypad[0],0)) tone=0xff;
		if (bit_is_set(keypad[0],1)) tone=0x80;
		if (bit_is_set(keypad[0],2)) tone=0x40;
		if (bit_is_set(keypad[0],3)) tone=0x20;

		//select scale
		if (bit_is_set(keypad[1],0)) { TCCR0 |= _BV(CS02); TCCR0 &= ~_BV(CS01); TCCR0 |= _BV(CS00); }
		if (bit_is_set(keypad[1],1)) { TCCR0 |= _BV(CS02); TCCR0 &= ~_BV(CS01); TCCR0 &= ~_BV(CS00); }
		if (bit_is_set(keypad[1],2)) { TCCR0 &= ~_BV(CS02); TCCR0 |= _BV(CS01); TCCR0 |= _BV(CS00); }
		if (bit_is_set(keypad[1],3)) { TCCR0 &= ~_BV(CS02); TCCR0 |= _BV(CS01); TCCR0 &= ~_BV(CS00); }

		status_toogle();
		for (l=0; l<10000; l++) { }
	}

	cli();

	keypad_free();

	return 0;
}

