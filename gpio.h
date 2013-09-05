/* ***********************************************************************
**  libarduino - a GNU library for using GNU toolchain with Arduino
**  Copyright (C) 2009 Michael Spiceland
*************************************************************************
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software Foundation, 
**  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
**
*************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "libarduino.h"

/**************************************************************************
* no user configurable things to edit below this
**************************************************************************/

#define setpin_in(port, pin) 	port &= ~_BV(pin)
#define setpin_out(port, pin)	port |= _BV(pin)

#define setpin(port, pin) 	port |= _BV(pin)
#define clearpin(port, pin)	port &= ~_BV(pin)

void onboard_led_enable(void);
void onboard_led_on(void);
void onboard_led_off(void);

#ifdef ENABLE_ARDUINO_COMPAT /* subset of arduino functions */
enum pinmode {
	INPUT  = 0,
	OUTPUT = 1
};

enum pinstate {
	LOW = 0,
	HIGH = 1
};

void pinMode(uint8_t pin, enum pinmode mode);
void digitalWrite(uint8_t pin, enum pinstate value);
uint8_t digitalRead(uint8_t pin);
#ifdef ENABLE_PWM /* using arduino compat and pwm */
void analogWrite(uint8_t pin, uint8_t value);
#endif	// ENABLE_PWM

#endif	// ENABLE_ARDUINO_COMPAT

#endif 	// _GPIO_H_
