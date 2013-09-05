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

#ifdef ENABLE_ADC
/***************************************************************************
* adc_init()
*  
* gets our ADC ready to take 10bit samples 
* compares analog input against voltage on AREF pin
* you probably want to connect AREF to VCC
***************************************************************************/
void adc_init(void)
{
	/* initialize the ADC - 10bit mode */
	//ADMUX |= _BV(ADLAR); /* uncomment for 8-bit mode */
	ADCSRA |= _BV(ADEN); // for now we don't do this in the ISR | _BV(ADIE);
	ADCSRA |= _BV(ADPS2) | _BV(ADPS1); 
	PRR &= ~_BV(PRADC);
	
	/* set up all of our analog pins to be inputs */
	/* FIXME: perhaps this should be dynamic */
	setpin_in(DDRC, 0);
	setpin_in(DDRC, 1);
	setpin_in(DDRC, 2);
	setpin_in(DDRC, 3);
	setpin_in(DDRC, 4);
	setpin_in(DDRC, 5);
}

/***************************************************************************
* adc_get()
* 
* adcnum - specifies which ADC pin you want to read from
*        - 0 through 5 corresponds to ADC0-ADC5 (avr pins 14-19) and
*          Arduino Analog pins 0-5.
*
* returns 16 bit unsigned value between 0 and 1024 with 0 meaning 0v and
*          1024 meaning at or above voltage on AREF pin
***************************************************************************/
uint16_t adc_get(uint8_t adcnum)
{
	static uint8_t current_adcnum = 9;

	if (adcnum > 5)
		return 0; /* invalid */
	
	if (adcnum != current_adcnum)
		ADMUX = (ADMUX & 0xF0) | adcnum; /* set up mux */
	current_adcnum = adcnum; /* cache for next time */

	ADCSRA |= (1 << ADSC); /* start ADC conversion */
	while (ADCSRA & (1 << ADSC)) {;} /* block for the result */
	return ADC;
}
#endif

