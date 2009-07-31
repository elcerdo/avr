#include "keypad.h"

#include <stdlib.h>

static uint8_t *keypad = NULL;

void keypad_init(void) {
	keypad = calloc(8,sizeof(uint8_t));
	DDRA |= _BV(PSDATA) | _BV(PSCLEAR) | _BV(PRLOAD) | _BV(PRCLOCK) | _BV(PSCLOCK);
	PORTA |= _BV(PRLOAD);
}

uint8_t *keypad_get(void) {
    return keypad;
}

void keypad_free(void) {
	free(keypad);
	keypad = NULL;
}

//
// SELECTION
//

static void sel_valid_clock(void) {
	PORTA |= _BV(PSCLOCK);
	PORTA &= ~_BV(PSCLOCK);
}

static void sel_clear_value(void) {
	PORTA &= ~_BV(PSCLEAR);
	PORTA |= _BV(PSCLEAR);
}

void keypad_load_value(const uint8_t value) {
	uint8_t k;

	sel_clear_value();
	for (k=0; k<8; k++) {
		if ( value & _BV(k) ) PORTA |= _BV(PSDATA);
		else PORTA &= ~_BV(PSDATA);
		sel_valid_clock();
	}

}

//
// READ
//

static void read_valid_clock(void) {
	PORTA |= _BV(PRCLOCK);
	PORTA &= ~_BV(PRCLOCK);
}

static void read_load_value(void) {
	PORTA &= ~_BV(PRLOAD);
	PORTA |= _BV(PRLOAD);
}

static uint8_t read_line(void) {
	uint8_t keys=0xff;
	uint8_t k;

	read_load_value();
	for (k=0; k<8; k++) {
		if (bit_is_set(PINA,PRDATA)) keys &= ~ _BV(k);
		read_valid_clock();

	}
	return keys;
}


void keypad_update(void) {
	uint8_t k;

	keypad_load_value(0x70);
	PORTA |= _BV(PSDATA);

	for (k=0; k<8; k++) {
		keypad[k] = read_line();
		sel_valid_clock();
	}
}

