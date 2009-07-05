#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"

#define PSTATUS PB0

void toggle_led(void) {
	if (bit_is_set(PORTB,PSTATUS)) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}

#define PAUDIO PB3
#define DELAY 0xffff

static uint8_t time = 0;
static uint8_t speed = 1;
static uint32_t delay = 0;

static uint8_t *sinus = NULL;
static uint8_t *saw = NULL;
static uint8_t *rect = NULL;
static uint8_t *noise = NULL;
static uint8_t *table = NULL;

ISR(TIMER0_OVF_vect) {
	if (delay > 0) delay--;
	else DDRB &= ~_BV(PAUDIO);
	OCR0 = table[time];
	time += speed;
}

int main(void) {
	uint32_t l;

	cli();
	
	//set pins as output
	DDRA = _BV(PSDATA) | _BV(PSCLEAR) | _BV(PRLOAD) | _BV(PRCLOCK) | _BV(PSCLOCK);
	DDRB = _BV(PSTATUS);

	//light dev board led
	PORTA |= _BV(PRLOAD);
	PORTB &= ~_BV(PB0);

	//setup timer 0
	TCCR0 = _BV(WGM01) | _BV(WGM00) | _BV(CS00) | _BV(CS00) | _BV(COM01);
	TIMSK = _BV(TOIE0);

	keypad = malloc(2);
	sinus = malloc(256);
	saw = malloc(256);
	rect = malloc(256);
	noise = malloc(256);

	for (l=0; l<256; l++) {
		sinus[l] = (uint8_t)( 127.5 + 127.5 * sin( M_PI * (float)(l) / 128. ) );
		saw[l] = (uint8_t)( l );
		noise[l] = (uint8_t)( rand() );
		if (l < 72) rect[l] = 255;
		else rect[l] = 0;
	}

	table = sinus;

	sei();

	while (1) {
		update_keypad();

		cli();
		//select table
		if (bit_is_set(keypad[0],0)) { table = sinus; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[0],1)) { table = saw; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[0],2)) { table = rect; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[0],3)) { table = noise; delay = DELAY; DDRB |= _BV(PAUDIO); }

		//select scale
		if (bit_is_set(keypad[1],0)) speed = 1;
		if (bit_is_set(keypad[1],1)) speed = 2;
		if (bit_is_set(keypad[1],2)) speed = 4;
		if (bit_is_set(keypad[1],3)) speed = 8;
		sei();
	}

	cli();

	free(keypad);
	free(sinus);
	free(saw);
	free(rect);
	free(noise);

	return 0;
}

