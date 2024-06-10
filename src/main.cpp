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

#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // for Serial

#define ENCODER_S1_PIN P1_06
#define ENCODER_S2_PIN P1_04
#include "qdec.h"

const int ROTARY_PIN_A = P1_06; // the first pin connected to the rotary encoder
const int ROTARY_PIN_B = P1_04; // the second pin connected to the rotary encoder

// Create an instance of the QDecoder class, using the pins define'd above
::SimpleHacks::QDecoder qdec(ROTARY_PIN_A, ROTARY_PIN_B, false);

// Stores a relative value based on the clockwise / counterclockwise events
int rotaryCount = 0;

void setup()
{
  delay(2000);

  Serial.begin(115200);
  while (!Serial)
  {
    delay(20);
  } // wait for serial port to connect... needed for boards with native USB serial support
  Serial.print("Beginning QDecoder Sketch ");
  Serial.println(__FILE__); // becomes the sketch's filename during compilation

  // initialize the rotary encoder
  qdec.begin();

  // for (int pin = 0; pin < 26; pin++)
  // {
  //   pinMode(pin, INPUT_PULLUP);
  // }
}

void loop()
{
  // poll the state of the quadrature encoder,
  // and storing the result (which is an event, if one occurred this time)
  ::SimpleHacks::QDECODER_EVENT event = qdec.update();

  // increment for clockwise, decrement for counter-clockwise
  if (event & ::SimpleHacks::QDECODER_EVENT_CW)
  {
    rotaryCount = rotaryCount + 1;
    Serial.print("change: ");
    Serial.println(rotaryCount);
  }
  else if (event & ::SimpleHacks::QDECODER_EVENT_CCW)
  {
    rotaryCount = rotaryCount - 1;
    Serial.print("change: ");
    Serial.println(rotaryCount);
  }

  // for (int pin = 0; pin < 26; pin++)
  // {
  //   Serial.print(digitalRead(pin));
  //   Serial.print(" ");
  // }
  // Serial.println();
  // delay(100);

} // end of void loop()
