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
#include "irkeys.h"

#ifdef ENABLE_IR
volatile uint8_t ir_readptr = 0;
volatile uint8_t ir_writeptr = 0;
volatile uint8_t ir_buffer[IR_BUFFER_SIZE];

/*************************************************************************
 *  receive from IR Receiver - external interrupt 0 on pin PD2 (arduino pin 2)
 *
 * Use the external interrupt to decode the Sony IR protocol.  We use
 * both rising and falling edges to measure pulse widths.  The timer/counter
 * is needed to take these measurements.  If the address matches our device,
 * we store the command in a circular buffer for retreval outside of
 * interrupt context.
 *
 * tested with Vishay TSOP392 IR receiver and universal remote
 *************************************************************************/
ISR(INT0_vect)
{
	static uint8_t value;
	static uint8_t address;
	static uint8_t sigcount = 0;

	if (PIND & _BV(2)) { // rising edge
		unsigned short count;
		count = TCNT0;

		onboard_led_off();

#if (F_CPU == 16000000)
		if (count < 14) { // 0
#endif
#if (F_CPU == 8000000)
		if (count < 7) { // 0
#endif
			sigcount++;
			if (sigcount < 8) {
				value = value & ~(1 << (sigcount-1));
			} else {
				address = address & ~(1 << (sigcount-8));
			}
#if (F_CPU == 16000000)
		} else if (count < 30) { // 1
#endif
#if (F_CPU == 8000000)
		} else if (count < 15) { // 1
#endif
			sigcount++;
			if (sigcount < 8) {
				value = value | (1 << (sigcount-1));
			} else {
				address = address | (1 << (sigcount-8));
			}
		} else { // this starts a new one
			sigcount = 0;
			value = 0;
			address = 0;
		}

		/* we have a command and it is for us */
		if ((sigcount == 12) && ((address == 26) || (address == 17)
			|| (address == REMOTE_DEVICE_SONY_TV000)))
		{
			ir_buffer[ir_writeptr] = value;
			ir_writeptr = (ir_writeptr + 1) % IR_BUFFER_SIZE;
#ifdef IR_DEBOUNCE
			_delay_ms(200); /* debounce */
#endif
		}
		
	} else { // falling edge
		TCNT0 = 0; // reset
		onboard_led_on();
	}
}

/**************************************************************************
* ir_init
* AVR PD3 pin 5 - arduino digital pin 3
**************************************************************************/
void ir_init(void)
{
	setpin_in(DDRD, 2);

	/* 8-bit timer to count IR stuff */
	TCCR0B |= _BV(CS02) | _BV(CS00); //CLK / 64 for 8mhz
	TCNT0 = 0; //reset the timer

	/* we use external interrupt INT1 for the IR receiver */
	EICRA |= _BV(ISC00); // interrupt on rising and falling edge of INT1
	EIMSK |= _BV(INT0);  // enable int1 interrupts

	ir_readptr = 0;
	ir_writeptr = 0;
		
	onboard_led_enable();

	/* dont forget sei() */
}

/*************************************************************************
 * ir_get() 
 * 
 * blocks until we get the next IR value out of our buffer
 * returns the value of the command
 *************************************************************************/
uint8_t ir_get(void)
{
	uint8_t value;
	while( ir_writeptr == ir_readptr); /* block waiting for a value */
	value = ir_buffer[ir_readptr]; /* pull out a sampe*/
	ir_readptr = (ir_readptr + 1) % IR_BUFFER_SIZE;
	return value;
}

/*************************************************************************
 * ir_get_nonblock() 
 * 
 * returns the value of the command
 * returns 255 if there is not a new value for us
 *************************************************************************/
uint8_t ir_get_nonblock(void)
{
	if (ir_writeptr == ir_readptr) /* don't block waiting for a value */
		return 255;

	uint8_t value;
	value = ir_buffer[ir_readptr]; /* pull out a sample */
	ir_readptr = (ir_readptr + 1) % IR_BUFFER_SIZE;
	return value;
}

#endif	// ENABLE_IR
