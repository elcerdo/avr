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

	sei();
	return 0;
}

