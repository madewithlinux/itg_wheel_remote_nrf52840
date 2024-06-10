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
  PINNUM(0,  0),
  PINNUM(0,  1),
  PINNUM(0,  2),
  PINNUM(0,  3),
  PINNUM(0,  4),
  PINNUM(0,  5),
  PINNUM(0,  6),
  PINNUM(0,  7),
  PINNUM(0,  8),
  PINNUM(0,  9),
  PINNUM(0, 10),
  PINNUM(0, 11),
  PINNUM(0, 12),
  PINNUM(0, 13),
  PINNUM(0, 14),
  PINNUM(0, 15),
  PINNUM(0, 16),
  PINNUM(0, 17),
  PINNUM(0, 18),
  PINNUM(0, 19),
  PINNUM(0, 20),
  PINNUM(0, 21),
  PINNUM(0, 22),
  PINNUM(0, 23),
  PINNUM(0, 24),
  PINNUM(0, 25),
  PINNUM(0, 26),
  PINNUM(0, 27),
  PINNUM(0, 28),
  PINNUM(0, 29),
  PINNUM(0, 30),
  PINNUM(0, 31),
  PINNUM(1,  0),
  PINNUM(1,  1),
  PINNUM(1,  2),
  PINNUM(1,  3),
  PINNUM(1,  4),
  PINNUM(1,  5),
  PINNUM(1,  6),
  PINNUM(1,  7),
  PINNUM(1,  8),
  PINNUM(1,  9),
  PINNUM(1, 10),
  PINNUM(1, 11),
  PINNUM(1, 12),
  PINNUM(1, 13),
  PINNUM(1, 14),
  PINNUM(1, 15),
  PINNUM(1, 16),
  PINNUM(1, 17),
  PINNUM(1, 18),
  PINNUM(1, 19),
  PINNUM(1, 20),
  PINNUM(1, 21),
  PINNUM(1, 22),
  PINNUM(1, 23),
  PINNUM(1, 24),
  PINNUM(1, 25),
  PINNUM(1, 26),
  PINNUM(1, 27),
  PINNUM(1, 28),
  PINNUM(1, 29),
  PINNUM(1, 30),
  PINNUM(1, 31),
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

