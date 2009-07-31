#ifndef __SATEL_LED_H__
#define __SATEL_LED_H__

#include <avr/io.h>

#define PLCLOCK PC0
#define PLLOAD  PC1
#define PLDATA  PC6

#define LED_SHUTDOWN     0x0c
#define LED_SHUTDOWN_ON  0x01
#define LED_SHUTDOWN_OFF 0x00
#define LED_TEST         0x0f
#define LED_TEST_ON      0x01
#define LED_TEST_OFF     0x00
#define LED_LIMIT        0x0b

#define LED_DECODE       0x09
#define LED_DIGIT0       0x01
#define LED_DIGIT1       0x02

void led_init(void);

void led_send_command(uint8_t address,uint8_t data);

#endif
