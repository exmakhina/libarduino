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

#ifdef ENABLE_PWMSERVO
/*************************************************************************
* pwmservo_init(pwmno)
* pwmno 1 & 2 are preferred b/c they provide higher resolution
* pwmno:
* 1 - OC1A (avr pin 15 PB1) - arduino digital pin 9
* 2 - OC1B (avr pin 16 PB2) - arduino digital pin 10
* 3 - OC2A (avr pin 17 PB3) - arduino digital pin 11
* 4 - OC2B (avr pin  5 PD3) - arduino digital pin 3
* 5 - OC0A (avr pin 12 PD6) - arduino digital pin 6  (conflicts with IR) FIXME
* 6 - OC0B (avr pin 11 PD5) - arduino digital pin 5  (conflicts with IR) FIXME
*************************************************************************/
void pwmservo_init(uint8_t pwmno)
{
	/* FIXME: need to reserve pins and counters at compile time */
	/* FIXME: conflict w/ IR if it runs at 8MHz */
	if (!pwmno || (pwmno > 6)) /* invalid */
		return;

	if ((pwmno == 1) || (pwmno == 2)) /* TCNT1 */
	{
		if (pwmno == 1) {
			setpin_out(DDRB, 1);
			OCR1A = SERVO_MID_POS16; /* initial value */
			TCCR1A |= _BV(COM1A1); /* turn on PWM 1 */
		}
		if (pwmno == 2) {
			setpin_out(DDRB, 2);
			OCR1B = SERVO_MID_POS16;
			TCCR1A |= _BV(COM1B1); /* turn on PWM 2 */
		}
		TCCR1A |= _BV(WGM11); /* PWM phase correct */
		TCCR1B |= _BV(WGM13) | _BV(WGM12); /* PWM phase correct */
		TCCR1B |= _BV(CS11);
#if (F_CPU == 16000000)
		ICR1 = 0x9C3F; /* 16bit */
#endif
#if (F_CPU == 8000000)
		ICR1 = 0x4E1F; /* 16bit */
#endif
		TCNT1H = 0;/* initial value */
		TCNT1L = 0;/* initial value */
	}

	if ((pwmno == 3) || (pwmno == 4)) /* TCNT1 */
	{
		if (pwmno == 3) {
			setpin_out(DDRB, 3);
			OCR2A = SERVO_MID_POS8; /* initial value */
			TCCR2A |= _BV(COM2A1); /* turn on PWM 1 */
		}
		if (pwmno == 4) {
			setpin_out(DDRD, 3);
			OCR2B = SERVO_MID_POS8;
			TCCR2A |= _BV(COM2B1); /* turn on PWM 2 */
		}
#if (F_CPU == 16000000)
		TCCR2A |= _BV(WGM20) | _BV(WGM21); /* fast PWM */
#endif
#if (F_CPU == 8000000)
		TCCR2A |= _BV(WGM20); /* fast PWM */
#endif
		TCCR2B |= _BV(CS20) | _BV(CS21) | _BV(CS22);
		TCNT2 = 0;/* initial value */
	}
	
}

void __pwmservo_set(uint8_t servo, uint16_t pwmval)
{
	//printf("__pwmservo_set setting %d\n\r", pwmval);
	if (servo == 1)
		OCR1A = pwmval;
	else if (servo == 2)
		OCR1B = pwmval;
	else if (servo == 3)
		OCR2A = pwmval;
	else if (servo == 4)
		OCR2B = pwmval;
}

/***************************************************************************
* pwmservo_set
* servo - 1-6 (servo number that we already called init on)
* pwmval - from 0-255 - provides the position for the servo
*          0 makes a 1.5ms pulse
*          255 makes a 2.5ms pulse
***************************************************************************/
void pwmservo_set(uint8_t servo, uint8_t pwmval)
{
	//printf("pwmservo_set setting %d to %d\n\r", servo, pwmval);

	if ((servo == 1) || (servo == 2))
		__pwmservo_set(
			servo,
			(((uint32_t)pwmval*((uint32_t)SERVO_MAX_POS16-(uint32_t)SERVO_MIN_POS16))/(uint32_t)256) + (uint32_t)SERVO_MIN_POS16
			);

	if ((servo == 3) || (servo == 4))
		__pwmservo_set(
			servo,
			(((uint32_t)pwmval*((uint32_t)SERVO_MAX_POS8-(uint32_t)SERVO_MIN_POS8))/(uint32_t)256) + (uint32_t)SERVO_MIN_POS8
			);
}

/* not tested yet
void pwmservo_setf(uint8_t servo, float pwmval)
{
	if (servo == 1)
		OCR1A = pwmval * 255.0;
	else if (servo == 2)
		OCR1B = pwmval * 255.0;
} */
#endif	// ENABLE_PWMSERVO

#ifdef ENABLE_PWM
/*************************************************************************
* pwm_init(pwmno)
* pwmno:
* 1 - OC1A (avr pin 15 PB1) - arduino digital pin 9
* 2 - OC1B (avr pin 16 PB2) - arduino digital pin 10
* 3 - OC2A (avr pin 17 PB3) - arduino digital pin 11
* 4 - OC2B (avr pin  5 PD3) - arduino digital pin 3
* 5 - OC0A (avr pin 12 PD6) - arduino digital pin 6  (conflicts with IR) FIXME
* 6 - OC0B (avr pin 11 PD5) - arduino digital pin 5  (conflicts with IR) FIXME
*************************************************************************/
void pwm_init(uint8_t pwmno)
{
	/* FIXME: need to reserve pins and counters at compile time */
	if (!pwmno || (pwmno > 6)) /* invalid */
		return;

	if ((pwmno == 1) || (pwmno == 2)) /* TCNT1 */
	{
		if (pwmno == 1) {
			setpin_out(DDRB, 1);
			OCR1A = 0; /* initial value */
			TCCR1A |= _BV(COM1A1); /* turn on PWM 1 */
		}
		if (pwmno == 2) {
			setpin_out(DDRB, 2);
			OCR1B = 0;
			TCCR1A |= _BV(COM1B1); /* turn on PWM 2 */
		}
		TCCR1A |= _BV(WGM10); /* PWM 8bit */
		//TCCR1B |= _BV(WGM13) | _BV(WGM12); /* PWM phase correct */
		TCCR1B |= _BV(CS12); /* div by 256 */
		TCNT1H = 0;/* initial value */
		TCNT1L = 0;/* initial value */
	}

	if ((pwmno == 3) || (pwmno == 4)) /* TCNT1 */
	{
		if (pwmno == 3) {
			setpin_out(DDRB, 3);
			OCR2A = 0; /* initial value */
			TCCR2A |= _BV(COM2A1); /* turn on PWM 1 */
		}
		if (pwmno == 4) {
			setpin_out(DDRD, 3);
			OCR2B = 0;
			TCCR2A |= _BV(COM2B1); /* turn on PWM 2 */
		}
		TCCR2A |= _BV(WGM20); /* PWM 8bit */
		TCCR2B |= _BV(CS21) | _BV(CS22); /* div by 256 for ovfl 244hz*/
		TCNT2 = 0;/* initial value */
	}
	
}

/***************************************************************************
* pwm_set
* pwmchan - 1-6 (servo number that we already called init on)
* pwmval - from 0-255 - provides the position for the servo
*          0 makes a 1.5ms pulse
*          255 makes a 2.5ms pulse
***************************************************************************/
void pwm_set(uint8_t pwmchan, uint8_t pwmval)
{
	//printf("__pwm_set setting %d\n\r", pwmval);
	if (pwmchan == 1)
		OCR1A = pwmval;
	else if (pwmchan == 2)
		OCR1B = pwmval;
	else if (pwmchan == 3)
		OCR2A = pwmval;
	else if (pwmchan == 4)
		OCR2B = pwmval;
}

/* not tested yet
void pwm_setf(uint8_t pwmchan, float pwmval)
{
	if (pwmchan == 1)
		OCR1A = pwmval * 255.0;
	else if (servo == 2)
		OCR1B = pwmval * 255.0;
} */
#endif	// ENABLE_PWM
