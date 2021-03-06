/* Name: requests.h
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-09
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id: requests.h 692 2008-11-07 15:07:40Z cs $
 */

/* This header is shared between the firmware and the host software. It
 * defines the USB request numbers (and optionally data types) used to
 * communicate between the host and the device.
 */

#ifndef __REQUESTS_H_INCLUDED__
#define __REQUESTS_H_INCLUDED__

#define CUSTOM_RQ_LED_SET_STATUS     1
/* Set the LED status. Control-OUT */

#define CUSTOM_RQ_KEY_GET_STATUS     3
/* Send keys to computer */

#define CUSTOM_RQ_LEDS_SET_STATUS    4
/* Set current layers keypad leds */

#define CUSTOM_RQ_LEDS_SET_LAYER     5
/* Select active layer */

#define CUSTOM_RQ_LEDS_SET_INTENSITY 6

#endif /* __REQUESTS_H_INCLUDED__ */
