#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define BUFFER_SIZE 256

static char *tx_buffer = NULL;
const static char *tx_current = NULL;
static size_t tx_size = 0;

static char *rx_buffer = NULL;
static char *rx_current = NULL;
static size_t rx_size = 0;
static size_t rx_lines = 0;

void uart_send_string(const char *string,size_t size) {
	if (size == 0) return;
	do {} while (bit_is_set(UCSRA,UDRE) && tx_size !=0);

	strlcpy(tx_buffer,string,BUFFER_SIZE);

	tx_current = tx_buffer;
	tx_size = size;
	if (tx_size > BUFFER_SIZE) tx_size = BUFFER_SIZE;

	UDR = *tx_current;
	tx_current++;
	tx_size--;
}

ISR(USART_TXC_vect) {
	if (tx_size > 0 && *tx_current != 0) {
		UDR = *tx_current;
		tx_current++;
		tx_size--;
	} else {
		tx_current = NULL;
		tx_size = 0;
	}
}

ISR(USART_RXC_vect) {
	if (rx_size < BUFFER_SIZE-1 && rx_lines == 0) {
		*rx_current = UDR;
		if (*rx_current == 0 || *rx_current == '\n' || rx_size == BUFFER_SIZE-2) {
			status_set(1);
			*rx_current = 0;
			rx_lines++;
		}
		rx_size++;
		rx_current++;
	} else {
		uint8_t dummy;
		dummy = UDR;
	}
}

void uart_init(void) {
	DDRD |= _BV(PD1);
	UBRRL = 103; //see page 168
	UCSRB = _BV(TXEN) | _BV(TXCIE) | _BV(RXEN) | _BV(RXCIE);
	tx_buffer = malloc(BUFFER_SIZE);
	rx_buffer = malloc(BUFFER_SIZE);
	rx_current = rx_buffer;
}

uint8_t uart_read_line(char *string,size_t size) {
	if ( rx_lines == 0 ) return 0;

	strlcpy(string,rx_buffer,size);
	rx_current = rx_buffer;
	rx_size = 0;
	rx_lines--;
	status_set(0);

	return 1;
}

void uart_free(void) {
	free(tx_buffer);
	free(rx_buffer);
}

int main(void) {
	uint32_t l;
	uint8_t *keypad = NULL;
	uint8_t *old_table;
	uint8_t old_speed;

	cli();
	
    keypad_init();
	status_init();
	uart_init();
	keypad = keypad_get();

	//setup timer 0
	TCCR0 = _BV(WGM00) | _BV(CS00) | _BV(CS00) | _BV(COM01);
	TIMSK = _BV(TOIE0);


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

	uart_send_string("\n\nstartup\n",10);
	status_set(1);

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

		if (uart_read_line(status_text,256)) {
			size_t length = strnlen(status_text,255);
			status_text[length]='\n'; 
			status_text[length+1]=0;
		}
		if (old_table!=table || old_speed!=speed) uart_send_string(status_text,256);
		old_table = table;
		old_speed = speed;
	}

	cli();

	keypad_free();
	uart_free();

	free(status_text);
	free(sinus);
	free(saw);
	free(rect);
	free(noise);

	return 0;
}

