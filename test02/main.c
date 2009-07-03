#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//
// SELECTION
//

#define PSDATA  PA0
#define PSCLEAR PA1
#define PSCLOCK PA6

#define SEL_CLEAR ( 0 )
#define SEL_LOW   ( _BV(PSCLEAR) )
#define SEL_HIGH  ( _BV(PSCLEAR) | _BV(PSDATA) )

ISR(TIMER0_COMP_vect) {
	OCR0 += 4;
}

void sel_valid_clock(void) {
	PORTA |= _BV(PSCLOCK);
	PORTA &= ~_BV(PSCLOCK);
}

void sel_load_value(const uint8_t value) {
	uint8_t k;

	PORTA = SEL_CLEAR;
	sel_valid_clock();

	for (k=0; k<8; k++) {
		if ( value & _BV(k) ) PORTA = SEL_HIGH;
		else PORTA = SEL_LOW;
		sel_valid_clock();
	}

}

//
// READ
//

#define PRDATA  PA2
#define PRLOAD  PA3
#define PRCLOCK PA5


int main(void) {
	uint32_t k;
	uint8_t i;

	cli();
	
	//set pins as output
	DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA2) | _BV(DDA3) | _BV(DDA5) | _BV(DDA6);
	DDRB = _BV(DDB0) | _BV(DDB3);

	//light dev board led
	PORTB = ~ _BV(PB0);

	//setup timer 0
	TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00) | _BV(COM00);
	OCR0 = 0x70;
	TIMSK = _BV(OCIE0);

	sei();

	i=0;
	while (1) {
		sel_load_value(~i);
		for (k=0; k<65000; k++) { }
		i++;
	}

	return 0;
}

