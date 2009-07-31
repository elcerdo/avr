#ifndef __SATEL_STATUS__
#define __SATEL_STATUS__

#include <avr/io.h>

#define PSTATUS PB0

void status_init(void);
void status_toggle(void);
void status_set(uint8_t k);

#endif
