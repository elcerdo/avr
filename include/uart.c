#include "uart.h"

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

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

	return 1;
}

void uart_free(void) {
	free(tx_buffer);
	free(rx_buffer);
}

