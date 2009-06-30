#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void) {
	cli();
	
	DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA2) | _BV(DDA3);
	PORTA = ~( _BV(PA0) | _BV(PA2) );

	DDRB = _BV(DDB0);
	PORTB = _BV(PB0);
	PORTB = 0x00;

	sei();
	return 0;
}

