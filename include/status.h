#ifndef __SATEL_STATUS__
#define __SATEL_STATUS__

#define PSTATUS PB0

#include <inttypes.h>
#include <avr/io.h>

void status_init(void);
void status_toggle(void);
void status_set(uint8_t k);

#endif
