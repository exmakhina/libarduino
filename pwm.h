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

#ifndef _PWM_H_
#define _PWM_H_

#include "libarduino.h"

#ifdef ENABLE_PWMSERVO
void pwmservo_init(uint8_t pwmno);
void pwmservo_set(uint8_t servo, uint8_t pwmval); /* pwmval 0-255 */
/* 16bit timer counter values for PWM */
#define SERVO_MIN_POS16		1400
#define SERVO_MID_POS16		3000
#define SERVO_MAX_POS16		4600
/* 8bit timer counter values for PWM */
#define SERVO_MIN_POS8		10
#define SERVO_MID_POS8		25
#define SERVO_MAX_POS8		40
#endif

#ifdef ENABLE_PWM
void pwm_init(uint8_t pwmnum);
void pwm_set(uint8_t pwmnum, uint8_t pwmval); /* pwmval 0-255 */
#endif

#endif 	// _PWM_H_
