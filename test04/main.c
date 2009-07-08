#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "status.h"

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

//
// UART stuff
//

const static char *current_string = NULL;
static size_t current_size = 0;

void send_string_uart(const char *string,size_t size) {
	if (size == 0) return;
	do {} while (bit_is_set(UCSRA,UDRE) && current_size !=0);

	current_string = string;
	current_size = size;

	status_set(1);
	UDR = *current_string;
	current_string++;
	current_size--;
}

ISR(USART_TXC_vect) {
	if (current_size > 0 && *current_string != 0) {
		UDR = *current_string;
		current_string++;
		current_size--;
	} else {
		current_string = NULL;
		current_size = 0;
		status_set(0);
	}
}

int main(void) {
	uint32_t l;
	uint8_t *keypad = NULL;
	uint8_t *old_table;
	uint8_t old_speed;

	cli();
	
    keypad_init();
	status_init();
	keypad = keypad_get();

	//set pins as output
	DDRD = _BV(PD1);

	//light dev board led

	//setup timer 0
	TCCR0 = _BV(WGM00) | _BV(CS00) | _BV(CS00) | _BV(COM01);
	TIMSK = _BV(TOIE0);

	//setup UART
	UBRRL = 103; //see page 168
	UCSRB = _BV(TXEN) | _BV(TXCIE);

	//compute signals
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

	//uart echo setup
	char *status_text = malloc(256);
	snprintf(status_text,256,"coucou %d\n",10);

	sei();

	send_string_uart("\n\nstartup\n",10);

	while (1) {
		keypad_update();

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

		if (old_table!=table || old_speed!=speed) send_string_uart(status_text,10);
		old_table = table;
		old_speed = speed;
	}

	cli();

	keypad_free();
	free(status_text);
	free(sinus);
	free(saw);
	free(rect);
	free(noise);

	return 0;
}

