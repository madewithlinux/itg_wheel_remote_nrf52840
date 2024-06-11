/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial
#include "bluefruit.h"

#include <bluemicro_nrf52.h>
#include <bluemicro_hid.h>
#include "keyboard_api_proxy.h"

#define ENCODER_S1_PIN P1_06
#define ENCODER_S2_PIN P1_04
#include "qdec.h"

const int ROTARY_PIN_A = P1_06; // the first pin connected to the rotary encoder
const int ROTARY_PIN_B = P1_04; // the second pin connected to the rotary encoder

trigger_keys_t activeKeys;
trigger_keycodes_t activeKeycodes;

const byte rows[]{P0_17, P0_20, P0_22, P0_24}; // Contains the GPIO Numbers
const byte columns[]{P0_08, P0_10, P0_09};     // Contains the GPIO Numbers

const uint16_t matrixsize = (sizeof(rows) / sizeof(byte)) * (sizeof(columns) / sizeof(byte));
// const uint16_t  keymapsize = (sizeof(keymap) / sizeof(uint16_t));

#define sleep(r, c) sleep_C2R(r, c)
#define scanMatrix(a, b, c) scanMatrix_C2R(a, b, c)

// Create an instance of the QDecoder class, using the pins define'd above
::SimpleHacks::QDecoder qdec(ROTARY_PIN_A, ROTARY_PIN_B, false);

// Stores a relative value based on the clockwise / counterclockwise events
volatile int rotaryCount = 0;

void IsrForQDEC(void)
{
  ::SimpleHacks::QDECODER_EVENT event = qdec.update();
  if (event & ::SimpleHacks::QDECODER_EVENT_CW)
  {
    rotaryCount = rotaryCount + 1;
  }
  else if (event & ::SimpleHacks::QDECODER_EVENT_CCW)
  {
    rotaryCount = rotaryCount - 1;
  }
  return;
}

void setup()
{
  keyboard_api.begin();

  // delay(2000);

  Serial.begin(115200);
  while (!Serial)
  {
    delay(20);
  } // wait for serial port to connect... needed for boards with native USB serial support
  Serial.print("Beginning QDecoder Sketch ");
  Serial.println(__FILE__); // becomes the sketch's filename during compilation

  // initialize the rotary encoder
  qdec.begin();

  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), IsrForQDEC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), IsrForQDEC, CHANGE);

  activeKeys.reserve(10);
  activeKeycodes.reserve(10);
}

int lastLoopDisplayedRotaryCount = 0;

void loop()
{
  static trigger_keys_t lastAactiveKeys;
  activeKeys = scanMatrix(activeKeys, rows, columns);
  if (activeKeys != lastAactiveKeys)
  {
    fmt::print("activeKeys: {}\n", activeKeys);
    lastAactiveKeys = activeKeys;
  }

  int newValue = rotaryCount;
  if (newValue != lastLoopDisplayedRotaryCount)
  {
    // Also get the difference since the last loop() execution
    int difference = newValue - lastLoopDisplayedRotaryCount;

    // act on the change: e.g., modify PWM to be faster/slower, etc.
    lastLoopDisplayedRotaryCount = newValue;
    Serial.print("Change: ");
    Serial.print(newValue);
    Serial.print(" (");
    Serial.print(difference);
    Serial.println(")");

    uint8_t keycode[6] = {0};
    keycode[0] = (difference > 0) ? HID_KEY_ARROW_RIGHT : HID_KEY_ARROW_LEFT;
    for (int i = 0; i < abs(difference); i++)
    {
      keyboard_api.tap(keycode[0]);
    }
  }

  keyboard_api.processDirtyKeys();
}
