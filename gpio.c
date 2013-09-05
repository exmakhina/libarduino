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
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libarduino.h"

/* stuff used in all modes */
inline void onboard_led_enable(void) {
	setpin_out(DDRB, 5);
}

inline void onboard_led_on(void) {
	setpin(PORTB, 5);
}

inline void onboard_led_off(void) {
	clearpin(PORTB, 5);
}

/* arduino compatibility functions */
#ifdef ENABLE_ARDUINO_COMPAT /* subset of arduino functions */
void pinMode(uint8_t pin, enum pinmode mode)
{
	if (pin < 8)
	{	
		if (mode == INPUT)
			setpin_in(DDRD, pin);
		else
			setpin_out(DDRD, pin);
	} else {
		pin = pin - 8;
		if (mode == INPUT)
			setpin_in(DDRB, pin);
		else
			setpin_out(DDRB, pin);
	}
}

void digitalWrite(uint8_t pin, enum pinstate value)
{
	if (pin < 8)
	{	
		if (value == HIGH)
			setpin(PORTD, pin);
		else
			clearpin(PORTD, pin);
	} else {
		pin = pin - 8;
		if (value == HIGH)
			setpin(PORTB, pin);
		else
			clearpin(PORTB, pin);
	}
}

uint8_t digitalRead(uint8_t pin) 
{
	if (pin < 8)
		return (PIND & (1<<pin));
	else {
		pin = pin - 8;
		return (PINB & (1<<pin));
	}
}

#ifdef ENABLE_PWM
/* we have arduino api compat and pwm enabled */
void analogWrite(uint8_t pin, uint8_t value)
{
	switch(pin) {
	case 9:
		pwm_set(1, value);
		break;
	case 10:
		pwm_set(2, value);
		break;
	case 11:
		pwm_set(3, value);
		break;
	case 3:
		pwm_set(4, value);
		break;
	default:
		return;
		break;
	}
}
#endif 

#endif
