#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

PGM_P string = "pierre";

int main(void) {
	cli();
	
	char* string_ram = (char*) malloc(10);
	strlcpy_P(string_ram,string,10);
	free((void*) string_ram);

	DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA2) | _BV(DDA3);
	PORTA = ~( _BV(PA0) | _BV(PA2) );

	DDRB = _BV(DDB0);
	PORTB = _BV(PB0);
	PORTB = 0x00;

	sei();
	return 0;
}

