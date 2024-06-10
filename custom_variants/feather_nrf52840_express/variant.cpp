/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  Copyright (c) 2018, Adafruit Industries (adafruit.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"
#include "wiring_constants.h"
#include "wiring_digital.h"
#include "nrf.h"

// custom pin mapping for nice!nano

#define PINNUM(port, pin)    ((port)*32 + (pin))

const uint32_t g_ADigitalPinMap[] =
{
  // just map them to the same pin numbers, for simplicity
  [0] = PINNUM(0,  2),
  [1] = PINNUM(0,  4),
  [2] = PINNUM(0,  6),
  [3] = PINNUM(0,  8),
  [4] = PINNUM(0,  9),
  [5] = PINNUM(0, 10),
  [6] = PINNUM(0, 11),
  [7] = PINNUM(0, 12),
  [8] = PINNUM(0, 13),
  [9] = PINNUM(0, 15),
  [10] = PINNUM(0, 17),
  [11] = PINNUM(0, 20),
  [12] = PINNUM(0, 22),
  [13] = PINNUM(0, 24),
  [14] = PINNUM(0, 26),
  [15] = PINNUM(0, 29),
  [16] = PINNUM(0, 31),
  [17] = PINNUM(1,  0),
  [18] = PINNUM(1,  1),
  [19] = PINNUM(1,  2),
  [20] = PINNUM(1,  4),
  [21] = PINNUM(1,  6),
  [22] = PINNUM(1,  7),
  [23] = PINNUM(1, 11),
  [24] = PINNUM(1, 13),
  [25] = PINNUM(1, 15),
  // TODO: additional pins
};

void initVariant()
{
  // LED1 & LED2
  pinMode(PIN_LED1, OUTPUT);
  ledOff(PIN_LED1);

  pinMode(PIN_LED2, OUTPUT);
  ledOff(PIN_LED2);

  pinMode(PIN_NEOPIXEL_POWER, OUTPUT);
  digitalWrite(PIN_NEOPIXEL_POWER, HIGH);
}

