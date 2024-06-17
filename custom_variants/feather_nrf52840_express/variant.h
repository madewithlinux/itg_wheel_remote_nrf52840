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

#ifndef _VARIANT_FEATHER52840_
#define _VARIANT_FEATHER52840_

/** Master clock frequency */
#define VARIANT_MCK       (64000000ul)

#define USE_LFXO      // Board uses 32khz crystal for LF
// define USE_LFRC    // Board uses RC for LF

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Number of pins defined in PinDescription array
#define PINS_COUNT           (35)
#define NUM_DIGITAL_PINS     (35)
#define NUM_ANALOG_INPUTS    (6) // A6 is used for battery, A7 is analog reference
#define NUM_ANALOG_OUTPUTS   (0)

// these reference indexes in g_ADigitalPinMap[]
#define P0_02 (0)
#define P0_04 (1)
#define P0_06 (2)
#define P0_08 (3)
#define P0_09 (4)
#define P0_10 (5)
#define P0_11 (6)
#define P0_12 (7)
#define P0_13 (8)
#define P0_15 (9)
#define P0_17 (10)
#define P0_20 (11)
#define P0_22 (12)
#define P0_24 (13)
#define P0_26 (14)
#define P0_29 (15)
#define P0_31 (16)
#define P1_00 (17)
#define P1_01 (18)
#define P1_02 (19)
#define P1_04 (20)
#define P1_06 (21)
#define P1_07 (22)
#define P1_11 (23)
#define P1_13 (24)
#define P1_15 (25)

// LEDs
#define PIN_LED              (P0_15)
#define LED_BUILTIN          PIN_LED
// it's not actually blue
#define LED_BLUE             PIN_LED
// just to satisfy bluemicro HID
#define LED_RED              LED_BLUE

#define LED_STATE_ON         1         // State when LED is litted

/*
 * Buttons
 */
#define PIN_BUTTON1          (7)

/*
 * Analog pins
 */
#define PIN_A0               (P0_02)
#define PIN_A2               (P0_04)
#define PIN_A5               (P0_29)
#define PIN_A7               (P0_31)

static const uint8_t A0  = PIN_A0 ;
static const uint8_t A2  = PIN_A2 ;
static const uint8_t A5  = PIN_A5 ;
static const uint8_t A7  = PIN_A7 ;

#define ADC_RESOLUTION    14

// Other pins
#define PIN_AREF           PIN_A7
#define PIN_VBAT           P0_04
#define PIN_NFC1           (33)
#define PIN_NFC2           (2)

static const uint8_t AREF = PIN_AREF;

/*
 * Serial interfaces
 */
#define PIN_SERIAL1_RX       (P0_08)
#define PIN_SERIAL1_TX       (P0_06)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (P1_11)
#define PIN_SPI_MOSI         (P0_10 )
#define PIN_SPI_SCK          (P1_13)

// static const uint8_t SS   = (5);
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (P0_17)
#define PIN_WIRE_SCL         (P0_20)

// QSPI Pins
#define PIN_QSPI_SCK         27
#define PIN_QSPI_CS          28
#define PIN_QSPI_IO0         29
#define PIN_QSPI_IO1         30
#define PIN_QSPI_IO2         31
#define PIN_QSPI_IO3         32

// On-board QSPI Flash
#define EXTERNAL_FLASH_DEVICES   GD25Q16C
#define EXTERNAL_FLASH_USE_QSPI

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#endif
