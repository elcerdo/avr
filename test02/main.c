#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//
// SELECTION
//

#define PSDATA  PA0
#define PSCLEAR PA1
#define PSCLOCK PA6

void sel_valid_clock(void) {
	PORTA |= _BV(PSCLOCK);
	PORTA &= ~_BV(PSCLOCK);
}

void sel_clear_value(void) {
	PORTA &= ~_BV(PSCLEAR);
	PORTA |= _BV(PSCLEAR);
}

void sel_load_value(const uint8_t value) {
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

#define PRDATA  PA2
#define PRLOAD  PA3
#define PRCLOCK PA5

void read_valid_clock(void) {
	PORTA |= _BV(PRCLOCK);
	PORTA &= ~_BV(PRCLOCK);
}

void read_load_value(void) {
	PORTA &= ~_BV(PRLOAD);
	PORTA |= _BV(PRLOAD);
}

uint8_t read_line(void) {
	uint8_t keys=0xff;
	uint8_t k;

	read_load_value();
	for (k=0; k<8; k++) {
		if (bit_is_set(PINA,PRDATA)) keys &= ~ _BV(k);
		read_valid_clock();

	}
	return keys;
}

static uint8_t *keypad = NULL;

void update_keypad(void) {
	uint8_t k;

	sel_load_value(0x70);
	PORTA |= _BV(PSDATA);

	for (k=0; k<2; k++) {
		keypad[k] = read_line();
		sel_valid_clock();
	}
}

#define PSTATUS PB0

void toggle_led(void) {
	if (bit_is_set(PORTB,PSTATUS)) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}
	
ISR(TIMER0_COMP_vect) {
	OCR0 += 4;
}

int main(void) {
	uint32_t k,l;
	uint8_t i;

	cli();
	
	//set pins as output
	DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA3) | _BV(DDA4) | _BV(DDA5) | _BV(DDA6);
	DDRB = _BV(DDB0) | _BV(DDB3);

	//light dev board led
	PORTA |= _BV(PRLOAD);
	PORTA &= _BV(PA4);
	PORTB &= ~_BV(PB0);

	//setup timer 0
	TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00) | _BV(COM00);
	OCR0 = 0x70;
	TIMSK = _BV(OCIE0);

	keypad = malloc(2);

	sei();

	while (1) {
		update_keypad();
		i = (keypad[0] & 0x0f) | ((keypad[1] & 0x0f) << 4);
		sel_load_value(~i);
		toggle_led();
		for (l=0; l<10000; l++) { }
	}

	cli();

	free(keypad);

	return 0;
}

