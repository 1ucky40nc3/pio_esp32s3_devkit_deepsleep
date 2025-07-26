#include <Arduino.h>
#include "config.h"
#include "led.h"

static const int TSW_PIN = 5; // set input pin for switch
static int TSW_State = 0;     // variable for storing switch status

void setup()
{
  // Initialize the serial connection
  Serial.begin(SERIAL_BAUD);
  // // wait for Serial to come online (only for testing/development purposes)
  // while (!Serial)
  //   ;
  delay(DELAY_AFTER_SERIAL_INITIALIZED);
  Serial.println("Serial start");

  pinMode(TSW_PIN, INPUT_PULLDOWN);
}

void loop()
{

  TSW_State = digitalRead(TSW_PIN); // read state of switch
  Serial.println(TSW_State);        // print state of switch

  if (TSW_State == 0)
  {
    turnLEDRed(LED_PIN);
  }
  else
  {
    turnLEDGreen(LED_PIN);
  }
  delay(300);
}
