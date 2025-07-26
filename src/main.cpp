#include <Arduino.h>
#include "config.h"
#include "led.h"

void setup()
{
  turnLEDOff(LED_PIN);
  // Initialize the serial connection
  Serial.begin(SERIAL_BAUD);
  // // wait for Serial to come online (only for testing/development purposes)
  // while (!Serial)
  //   ;
  delay(DELAY_AFTER_SERIAL_INITIALIZED);
  Serial.println("Serial start");
  turnLEDBlue(LED_PIN);
}

void loop()
{

  delay(2000);
}
