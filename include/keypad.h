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

