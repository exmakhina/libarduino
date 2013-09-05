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
#ifndef LIBARDUINO_H
#define LIBARDUINO_H

#include <inttypes.h>

#define ENABLE_PWMSERVO /* servo control (conflicts with regular pwm) */
#define ENABLE_PWM /* motor or led control (conflicts with pwmservo) */
#define ENABLE_IR /* infrared receiver */
#define ENABLE_ADC /* analog to digital convertor */
#define ENABLE_SERIAL /* uart0 interface */

#define ENABLE_ARDUINO_COMPAT /* subset of arduino functions */

#define IR_DEBOUNCE /* uncomment to debounce IR with a delay */

#if !((F_CPU == 16000000) || (F_CPU == 8000000))
#error "Processor speed not supported in libarduino.c !"
#endif

#if (F_CPU == 8000000)
#error "Processor speed only partically supported by libarduino.c.  Some things may not work !"
#endif

#include "uart.h"
#include "gpio.h"
#include "ir.h"
#include "pwm.h"
#include "adc.h"

#endif
