#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"

#define PSTATUS PB0

void toggle_led(void) {
	if (bit_is_set(PORTB,PSTATUS)) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}

void set_status(uint8_t k) {
	if (k) PORTB &= ~_BV(PSTATUS);
	else PORTB |= _BV(PSTATUS);
}

#define PAUDIO PB3
#define DELAY 0x8000

static uint8_t time = 0;
static uint8_t speed = 1;
static uint32_t delay = 0;

static uint8_t *sinus = NULL;
static uint8_t *saw = NULL;
static uint8_t *rect = NULL;
static uint8_t *noise = NULL;
static uint8_t *table = NULL;

ISR(TIMER0_OVF_vect) {
	if (delay > 0) delay--;
	else DDRB &= ~_BV(PAUDIO);
	OCR0 = table[time];
	time += speed;
}

const static char *current_string = NULL;
static size_t current_size = 0;

void send_uart(const char *string,size_t size) {
	if (size == 0) return;
	do {} while (bit_is_set(UCSRA,UDRE) && current_size !=0);

	current_string = string;
	current_size = size;

	set_status(1);
	UDR = *current_string;
	current_string++;
	current_size--;
}

ISR(USART_TXC_vect) {
	if (current_size > 0) {
		UDR = *current_string;
		current_string++;
		current_size--;
	} else {
		current_string = NULL;
		set_status(0);
	}
}

const static char *text="le chat est super cool!!!\n";

int main(void) {
	uint32_t l;
	uint8_t *old_table;
	uint8_t old_speed;

	cli();
	
	//set pins as output
	DDRA = _BV(PSDATA) | _BV(PSCLEAR) | _BV(PRLOAD) | _BV(PRCLOCK) | _BV(PSCLOCK);
	DDRB = _BV(PSTATUS);
	DDRD = _BV(PD1);

	//light dev board led
	PORTA |= _BV(PRLOAD);

	//setup timer 0
	TCCR0 = _BV(WGM00) | _BV(CS00) | _BV(CS00) | _BV(COM01);
	TIMSK = _BV(TOIE0);

	//setup UART
	UBRRL = 103; //see page 168
	UCSRB = _BV(TXEN) | _BV(TXCIE);

	//compute signals
	keypad = malloc(2);
	sinus = malloc(256);
	saw = malloc(256);
	rect = malloc(256);
	noise = malloc(256);

	for (l=0; l<256; l++) {
		sinus[l] = (uint8_t)( 127.5 + 127.5 * sin( M_PI * (float)(l) / 128. ) );

		if (l < 64) saw[l] = (uint8_t)( 128 + 2*l );
		else if (l < 192) saw[l] = (uint8_t)( 256 - 2*(l-64) );
		else saw[l] = (uint8_t)( 2*(l-192) );

		noise[l] = (uint8_t)( rand() );

		if (l < 128) rect[l] = 255;
		else rect[l] = 0;
	}

	table = sinus;

	old_table = table;
	old_speed = speed;

	sei();

	set_status(0);
	send_uart(text,26);

	while (1) {
		update_keypad();

		cli();
		//select table
		if (bit_is_set(keypad[0],0)) { table = sinus; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[0],1)) { table = saw; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[0],2)) { table = rect; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[0],3)) { table = noise; delay = DELAY; DDRB |= _BV(PAUDIO); }

		//select scale
		if (bit_is_set(keypad[1],0)) { speed = 1; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[1],1)) { speed = 2; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[1],2)) { speed = 4; delay = DELAY; DDRB |= _BV(PAUDIO); }
		if (bit_is_set(keypad[1],3)) { speed = 8; delay = DELAY; DDRB |= _BV(PAUDIO); }
		sei();

		if (old_table!=table || old_speed!=speed) send_uart("top\n",4);
		old_table = table;
		old_speed = speed;
	}

	cli();

	free(keypad);
	free(sinus);
	free(saw);
	free(rect);
	free(noise);

	return 0;
}

