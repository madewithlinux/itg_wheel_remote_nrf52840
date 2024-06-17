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
#include "keymap_layers.h"
#ifdef ITG_RECEIVER
#include "itg_receiver.h"
#endif // ITG_RECEIVER

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
#ifdef ITG_REMOTE
  keyboard_api.begin();
#endif // ITG_REMOTE

  TinyUSBDevice.setManufacturerDescriptor("madewithlinux");
#ifdef ITG_REMOTE
  TinyUSBDevice.setProductDescriptor("itg_wheel_remote");
#endif // ITG_REMOTE
#ifdef ITG_RECEIVER
  TinyUSBDevice.setProductDescriptor("itg_wheel_receiver");
#endif // ITG_RECEIVER

  // delay(500);
  Serial.begin(115200);
#ifdef ITG_RECEIVER
  while (!Serial)
    delay(20);
#endif // ITG_RECEIVER

#ifdef ITG_REMOTE
    // // Serial.setTimeout(500);
    // uint32_t begin_ms = millis();
    // while (!Serial)
    // {
    //   delay(20);
    //   // yield();
    //   if ((millis() - begin_ms) > (13790))
    //   {
    //     break;
    //   }
    // } // wait for serial port to connect... needed for boards with native USB serial support
    // fmt::print("delay until serial was ready: {}\n", millis() - begin_ms);
#endif // ITG_REMOTE

#ifdef DEBUG_LOGS
  Serial.print("Beginning ITG wheel remote sketch ");
  Serial.println(__FILE__); // becomes the sketch's filename during compilation
#endif

#ifdef ITG_REMOTE
#endif // ITG_REMOTE
#ifdef ITG_RECEIVER
#endif // ITG_RECEIVER

#ifdef ITG_REMOTE
  // initialize the rotary encoder
  qdec.begin();

  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_A), IsrForQDEC, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_PIN_B), IsrForQDEC, CHANGE);

  activeKeys.reserve(10);
  activeKeycodes.reserve(10);
#endif // ITG_REMOTE

#ifdef ITG_RECEIVER
  receiver_setup();
#endif // ITG_RECEIVER
}

int lastLoopDisplayedRotaryCount = 0;

void loop()
{
#ifdef ITG_REMOTE
  static trigger_keys_t lastAactiveKeys;
  activeKeys = scanMatrix(activeKeys, rows, columns);
  if (activeKeys != lastAactiveKeys)
  {
#ifdef DEBUG_LOGS
    fmt::print("activeKeys: {}\n", activeKeys);
#endif
    lastAactiveKeys = activeKeys;
  }

  int newValue = rotaryCount;
  int delta = newValue - lastLoopDisplayedRotaryCount;
  lastLoopDisplayedRotaryCount = newValue;

  // TODO: read player switch
  // TODO: read center button
  process_triggered_keys(activeKeys, false, delta);

  keyboard_api.processDirtyKeys(true);
  delay(10);
#endif // ITG_REMOTE

#ifdef ITG_RECEIVER
  receiver_loop();
#endif // ITG_RECEIVER
}
