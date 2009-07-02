#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {UP, DOWN} dir_t;

static uint8_t pos=0;
static dir_t dir=UP;
static uint8_t delay=0;

void bump(void) {
	PORTA = ~_BV(pos);

	switch (dir) {
	case UP:
		if (pos == 6) dir = DOWN;
		else pos++;
		break;
	case DOWN:
		if (pos == 0) dir = UP;
		else pos--;
		break;
	}
}

ISR(TIMER0_COMP_vect) {
	PORTB = _BV(PB0) & ~PORTB;

	delay += 4;
	delay %= 128;

	OCR0 = 100+delay;
}


int main(void) {
	cli();
	
	//set pins as output
	DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA2) | _BV(DDA3) | _BV(DDA4) | _BV(DDA5) | _BV(DDA6);
	DDRB = _BV(DDB0);

	//light dev board led
	PORTB = _BV(PB0);

	//setup timer 0
	TCCR0 = _BV(WGM01) | _BV(CS02) |_BV(CS00);
	OCR0 = 128;
	TIMSK = _BV(OCIE0);

	sei();

	uint32_t counter;
	while (1) {
		for (counter=0; counter<64000; counter++) {}
		bump();
	}

	return 0;
}

