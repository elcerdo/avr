#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "usbdrv.h"
#include "oddebug.h"
#include "requests.h"
#include "keypad.h"
#include "led.h"
#include "status.h"

#define MAXLAYER 4
static uint8_t current_layer = 0;
static uint8_t *flat_layers = NULL;
static uint8_t **layers = NULL;

void layers_init(void) {
    uint8_t k;
    flat_layers = calloc(8*MAXLAYER,sizeof(uint8_t));
    layers = calloc(MAXLAYER,sizeof(uint8_t*));
    for (k=0; k<MAXLAYER; k++) layers[k] = &flat_layers[8*k];
}

void layers_free(void) {
    free(layers);
    free(flat_layers);
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (void *)data;

    if (rq->bRequest == CUSTOM_RQ_LED_SET_STATUS) {
        status_set(rq->wValue.word & 1);
    } else if (rq->bRequest == CUSTOM_RQ_KEY_GET_STATUS) {
        keypad_update();
        DBG1(0x02, keypad_get(), 8);
        usbMsgPtr = keypad_get();
        return 8;
    } else if (rq->bRequest == CUSTOM_RQ_LEDS_SET_STATUS) {
        return USB_NO_MSG;
    } else if (rq->bRequest == CUSTOM_RQ_LEDS_SET_LAYER) {
        if (rq->wValue.word >= 0 && rq->wValue.word < MAXLAYER) current_layer = rq->wValue.word;
    }
    return 0;
}

/* ------------------------------------------------------------------------- */

uchar usbFunctionWrite(uchar *data, uchar len) {
    static uint8_t k,l,current;
    memcpy(layers[current_layer],data,8);
    for (k=0; k<8; k++) {
        current = 0;
        for (l=0; l<MAXLAYER; l++) current |= layers[l][k];
        led_send_command(k+1,current);
    }
    return 1;
}

int main(void) {
    uint8_t i;

    keypad_init();
    led_init();
    status_init();
    layers_init();

    for (i=0; i<8; i++) led_send_command(i+1,0x00);

    wdt_enable(WDTO_1S);
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
     */
    /* RESET status: all port bits are inputs without pull-up.
     * That's the way we need D+ and D-. Therefore we don't need any
     * additional hardware initialization.
     */
    odDebugInit();
    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    i = 0;
    while(--i){             /* fake USB disconnect for > 250 ms */
        wdt_reset();
        _delay_ms(1);
    }
    usbDeviceConnect();

    sei();
    DBG1(0x01, 0, 0);       /* debug output: main loop starts */
    for(;;){                /* main event loop */
        wdt_reset();
        usbPoll();
    }

    keypad_free();
    layers_free();

    return 0;
}

/* ------------------------------------------------------------------------- */
