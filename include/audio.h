#ifndef __SATEL_AUDIO__
#define __SATEL_AUDIO__

#include <inttypes.h>

void audio_on(uint32_t time);
void audio_off(void);

typedef enum { SIN, TRIANGLE, RECT, NOISE, NONE } wave_t;

void audio_set_table(wave_t wave);
wave_t audio_get_table(void);

void audio_set_speed(uint8_t speed);
uint8_t audio_get_speed(void);

void audio_init(void);
void audio_free(void);

#endif
