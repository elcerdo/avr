#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"

#define PSTATUS PB0

void toggle_led(void) {
	if (bit_is_set(PORTB,PSTATUS)) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}

#define PAUDIO PB3

static uint8_t tone = 0x80;

ISR(TIMER0_COMP_vect) {
	if (OCR0 < tone) OCR0++;
	if (OCR0 > tone) OCR0--;
}

int main(void) {
	uint32_t l;
	uint8_t i;

	cli();
	
	//set pins as output
	DDRA = _BV(PSDATA) | _BV(PSCLEAR) | _BV(PRLOAD) | _BV(PRCLOCK) | _BV(PSCLOCK);
	DDRB = _BV(PSTATUS) | _BV(PAUDIO);

	//light dev board led
	PORTA |= _BV(PRLOAD);
	PORTB &= ~_BV(PB0);

	//setup timer 0
	TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00) | _BV(COM00);
	OCR0 = tone;
	TIMSK = _BV(OCIE0);

	keypad = malloc(2);

	sei();

	while (1) {
		update_keypad();

		//display keypad
		i = (keypad[0] & 0x0f) | ((keypad[1] & 0x0f) << 4);
		sel_load_value(~i);

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

		toggle_led();
		for (l=0; l<5000; l++) { }
	}

	cli();

	free(keypad);

	return 0;
}

