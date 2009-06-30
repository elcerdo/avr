#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {UP, DOWN} dir_t;

static uint8_t pos=0;
static dir_t dir=UP;

void bump(void) {
	PORTA = ~_BV(pos);

	switch (dir) {
	case UP:
		if (pos == 3) dir = DOWN;
		else pos++;
		break;
	case DOWN:
		if (pos == 0) dir = UP;
		else pos--;
		break;
	}
}
	

int main(void) {
	cli();
	
	//set pins as output
	DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA2) | _BV(DDA3);
	DDRB = _BV(DDB0);

	//light dev board led
	PORTB = ~_BV(PB0);

	sei();

	uint32_t counter;
	while (1) {
		for (counter=0; counter<800; counter++) {}
		bump();
	}

	return 0;
}

