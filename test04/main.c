#include <stdio.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "status.h"
#include "audio.h"
#include "uart.h"

int main(void) {
	uint8_t *keypad = NULL;
	wave_t old_table;
	uint8_t old_speed;

	cli();
	
    keypad_init();
	status_init();
	uart_init();
	audio_init();

	keypad = keypad_get();
	old_table = audio_get_table();
	old_speed = audio_get_speed();

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
		if (bit_is_set(keypad[0],0)) { audio_set_table(SIN); audio_on(0X8000); }
		if (bit_is_set(keypad[0],1)) { audio_set_table(TRIANGLE); audio_on(0X8000); }
		if (bit_is_set(keypad[0],2)) { audio_set_table(RECT); audio_on(0x8000); }
		if (bit_is_set(keypad[0],3)) { audio_set_table(NOISE); audio_on(0x8000); }

		//select scale
		if (bit_is_set(keypad[1],0)) { audio_set_speed(1); audio_on(0x8000); }
		if (bit_is_set(keypad[1],1)) { audio_set_speed(2); audio_on(0x8000); }
		if (bit_is_set(keypad[1],2)) { audio_set_speed(4); audio_on(0x8000); }
		if (bit_is_set(keypad[1],3)) { audio_set_speed(8); audio_on(0x8000); }
		sei();

		if (uart_read_line(status_text,256)) {
			size_t length = strnlen(status_text,255);
			status_text[length]='\n'; 
			status_text[length+1]=0;
		}
		if ( old_table != audio_get_table() || old_speed != audio_get_speed() ) uart_send_string(status_text,256);
		old_table = audio_get_table();
		old_speed = audio_get_speed();
	}

	cli();

	keypad_free();
	uart_free();
	audio_free();

	free(status_text);

	return 0;
}

