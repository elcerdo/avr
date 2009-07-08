#include "status.h"

void status_init(void) {
	DDRB = _BV(PSTATUS);
    PORTB |= _BV(PSTATUS);
}

void status_toogle(void) {
	if (bit_is_set(PORTB,PSTATUS)) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}

void status_set(uint8_t k) {
	if (k) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}

