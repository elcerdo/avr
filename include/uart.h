#ifndef __SATEL_UART_H__
#define __SATEL_UART_H__

#include <inttypes.h>
#include <string.h>

void uart_send_string(const char *string,size_t size);
uint8_t uart_read_line(char *string,size_t size);

void uart_init(void);
void uart_free(void);

#endif
