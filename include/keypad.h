#ifndef __SATEL_KEYPAD__
#define __SATEL_KEYPAD__

#include <avr/io.h>

#define PSDATA  PA0
#define PSCLEAR PA1
#define PSCLOCK PA6

#define PRDATA  PA2
#define PRLOAD  PA3
#define PRCLOCK PA5

void keypad_init(void);
void keypad_free(void);

uint8_t *keypad_get(void);

void keypad_load_value(const uint8_t value);
void keypad_update(void);

#endif
