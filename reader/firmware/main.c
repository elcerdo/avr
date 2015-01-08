#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "status.h"

static uint8_t cycle = 0;
static int16_t position_current = 0;
static int16_t position_target = 0;

void motor_callback(void) {

  if (position_current == position_target )
  {
    PORTA = 0xff;
    return;
  }

  switch (cycle)
  {
    case 0:
      PORTA = ~( _BV(PA0) );
      break;
    case 1:
      PORTA = ~( _BV(PA0) | _BV(PA1) );
      break;
    case 2:
      PORTA = ~( _BV(PA1) );
      break;
    case 3:
      PORTA = ~( _BV(PA1) | _BV(PA4) );
      break;
    case 4:
      PORTA = ~( _BV(PA4) );
      break;
    case 5:
      PORTA = ~( _BV(PA4) | _BV(PA5) );
      break;
    case 6:
      PORTA = ~( _BV(PA5) );
      break;
    case 7:
      PORTA = ~( _BV(PA5) | _BV(PA0) );
      break;
    default:
      PORTA = 0xff;
      break;
  }

  if (position_current > position_target)
  {
    position_current--;
    cycle--;
  }
  else
  {
    position_current++;
    cycle++;
  }

  cycle %= 8;

}

static uint8_t delay=0;

ISR(TIMER0_COMP_vect) {
  delay++;
  if (delay>127 || bit_is_clear(PINB, PINB1)) status_set(0);
  else status_toggle();
  motor_callback();
}


int main(void) {
  cli();

  //set pins as output
  DDRA = _BV(DDA0) | _BV(DDA1) | _BV(DDA4) | _BV(DDA5);
  DDRB = _BV(DDB0);

  //set bit to clear dev board led
  PORTA = 0xff;
  PORTB = _BV(PB1);

  //setup timer 0
  TCCR0 = _BV(WGM01) | _BV(CS02);
  OCR0 = 0xff;
  TIMSK = _BV(OCIE0);

  sei();


  uint32_t counter;
  while (1) {
    loop_until_bit_is_clear(PINB, PINB1);

    cli();
    position_target = 8*32;
    sei();
    do {} while(position_target != position_current);

    cli();
    position_target = 0;
    sei();
    do {} while(position_target != position_current);
  }

  return 0;
}

