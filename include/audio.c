#include "audio.h"

#include <stdio.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/io.h>

static uint8_t time = 0;
static uint8_t speed = 1;
static uint32_t delay = 0;

static uint8_t *sinus = NULL;
static uint8_t *saw = NULL;
static uint8_t *rect = NULL;
static uint8_t *noise = NULL;
static const uint8_t *table = NULL;

//
// audio stuff
//

void audio_on(uint32_t time)  { delay = time; DDRD |= _BV(PD7);  }
void audio_off(void) { delay = 0;      DDRD &= ~_BV(PD7); }

ISR(TIMER2_OVF_vect) {
	if (delay > 0) delay--;
	else audio_off();
	OCR2 = table[time];
	time += speed;
}

void audio_set_speed(uint8_t sspeed) {
    speed = sspeed;
}
    
uint8_t audio_get_speed(void) {
    return speed;
}

void audio_set_table(wave_t wave) {
    switch (wave) {
    case SIN: table = sinus; break;
    case TRIANGLE: table = saw; break;
    case RECT: table = rect; break;
    case NOISE: table = noise; break;
    case NONE: break;
    }
}

wave_t audio_get_table(void) {
    if (table == sinus) return SIN;
    if (table == saw) return TRIANGLE;
    if (table == rect) return RECT;
    if (table == noise) return NOISE;
    return NONE;
}

void audio_init(void) {
	uint32_t l;

	//init timer
	TCCR2 = _BV(WGM20) | _BV(CS20) | _BV(COM21);
	TIMSK = _BV(TOIE2);

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
}

void audio_free(void) {
	free(sinus);
	free(saw);
	free(rect);
	free(noise);
}

