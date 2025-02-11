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
#include "hardware.h"

#ifdef ITG_RECEIVER
#include "itg_receiver.h"
#endif // ITG_RECEIVER

#define PLAYER_SWITCH_PIN P0_11
#define ENCODER_BUTTON_PIN P1_00
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
  // digitalWrite(PIN_LED, 1);
  ledOn(PIN_LED);

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

#ifdef ITG_RECEIVER
  receiver_setup1();
#endif // ITG_RECEIVER

  Serial.begin(115200);
#ifdef DEBUG_LOGS
  while (!Serial)
    delay(20);
#ifdef ITG_REMOTE
  Serial.println("Beginning ITG wheel remote sketch");
#endif // ITG_REMOTE
#ifdef ITG_RECEIVER
  Serial.println("Beginning ITG wheel receiver sketch");
#endif // ITG_RECEIVER
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

  pinMode(PLAYER_SWITCH_PIN, INPUT_PULLUP);
  pinMode(ENCODER_BUTTON_PIN, INPUT_PULLUP);
#endif // ITG_REMOTE

#ifdef ITG_RECEIVER
  receiver_setup2();
#endif // ITG_RECEIVER
}

int lastLoopDisplayedRotaryCount = 0;

void loop()
{
#ifdef ITG_REMOTE
  static trigger_keys_t lastAactiveKeys;
  activeKeys = scanMatrix(activeKeys, rows, columns);
  // read encoder center button separately
  if (digitalRead(ENCODER_BUTTON_PIN) == 0)
  {
    activeKeys.push_back(INDEX_ENCODER_MIDDLE);
  }

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

  bool player_switch = digitalRead(PLAYER_SWITCH_PIN);
  process_triggered_keys(activeKeys, player_switch, delta);

  keyboard_api.processDirtyKeys(true);
  delay(10);
#endif // ITG_REMOTE

#ifdef ITG_RECEIVER
  receiver_loop();
#endif // ITG_RECEIVER
}
