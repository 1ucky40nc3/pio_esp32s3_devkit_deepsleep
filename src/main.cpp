#include <Arduino.h>
#include <esp_sleep.h>
#include "config.h" // Assuming this file defines SERIAL_BAUD, DELAY_AFTER_SERIAL_INITIALIZED, and LED_PIN
#include "led.h"    // Assuming this file defines turnLEDRed() and turnLEDGreen()

static const int TSW_PIN = 5; // set input pin for switch

// Define the external wakeup pin and its mask
const int ext_wakeup_pin = TSW_PIN;
// We'll configure this to wake on HIGH for a button connected to 3.3V with INPUT_PULLDOWN
const uint64_t ext_wakeup_pin_mask = 1ULL << ext_wakeup_pin;

// Flag to indicate if we've just woken up from deep sleep by the button
// We'll use a global to persist this across setup() and loop() effectively for initial behavior
RTC_DATA_ATTR bool wokeFromButton = false; // RTC_DATA_ATTR preserves value across deep sleep

void setup()
{
  Serial.begin(SERIAL_BAUD);
  // Add a longer delay here to ensure serial output catches up, especially after a reset
  delay(DELAY_AFTER_SERIAL_INITIALIZED);
  Serial.println("--- ESP32 Startup ---");

  pinMode(TSW_PIN, INPUT_PULLDOWN); // Use INPUT_PULLDOWN: Pin is LOW by default, HIGH when button (to 3.3V) is pressed.

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT1)
  {
    Serial.println("Woke up from deep sleep by external (EXT1) pin!");
    wokeFromButton = true; // Set flag to indicate we just woke via button
    Serial.print("Current TSW_PIN state immediately after wakeup: ");
    Serial.println(digitalRead(TSW_PIN)); // Debugging: See what the pin reads right now
  }
  else if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    Serial.println("Initial boot or wake up from reset (not deep sleep).");
    wokeFromButton = false; // Not woken by button
  }
  else
  {
    Serial.printf("Woke up due to other reason: %d\n", wakeup_reason);
    wokeFromButton = false;
  }

  // --- Decision Logic for Initial State ---
  // If we just woke up from the button, or it's an initial boot and the button IS pressed,
  // we want to stay awake and enter the loop.
  // Otherwise, if it's an initial boot and the button is NOT pressed, go to sleep.

  if (wokeFromButton)
  {
    Serial.println("Proceeding to loop() as we just woke up via button.");
  }
  else
  {
    // This is an initial power-on or a non-EXT1 reset.
    // Check the current button state to decide.
    int initial_tsw_state = digitalRead(TSW_PIN);
    Serial.print("Initial boot TSW_State: ");
    Serial.println(initial_tsw_state);

    if (initial_tsw_state == 0)
    { // Button is NOT pressed on initial boot
      Serial.println("Initial boot, button not pressed. Entering deep sleep immediately.");
      // Configure to wake up if the button is pressed (goes HIGH)
      esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
      esp_deep_sleep_start(); // Go to deep sleep
    }
    else
    { // Button IS pressed on initial boot (e.g., held down)
      Serial.println("Initial boot, button pressed. Proceeding to loop().");
    }
  }
  // If we reach here, it means we are intended to stay awake and run the loop.
  // Reset the flag so that loop() can evaluate normally after the first cycle.
  wokeFromButton = false; // Reset the flag for the next cycle, loop() will manage.
}

void loop()
{
  // If we just woke from the button, let's ensure we stay awake for a bit
  // even if the button is released instantly.
  // We can add a simple state machine or a timeout.

  // For this example, let's keep it simpler but effective:
  // If the button is currently held down (TSW_State == 1), stay awake and do active tasks.
  // If the button is released (TSW_State == 0), then we consider going to sleep.

  int TSW_State = digitalRead(TSW_PIN);
  Serial.print("TSW_State in loop: ");
  Serial.println(TSW_State);

  if (TSW_State == 1) // Button IS pressed (or was pressed recently for debounce)
  {
    turnLEDGreen(LED_PIN);
    Serial.println("TSW_State is 1 (button pressed). Executing active code.");
    // --- Your active code goes here ---
    // This is where you would do your sensor readings, network communication, etc.
    // As long as the button is held, this part of the loop will execute.
    // If you only want it to run once per "wake" cycle, you'd need a flag
    // to track if the task has already been performed since waking.
    // ---------------------------------
  }
  else // TSW_State is 0 (button released)
  {
    turnLEDRed(LED_PIN);
    Serial.println("TSW_State is 0 (button released). Preparing to enter deep sleep...");
    // Configure to wake up when the button is pressed (goes HIGH)
    esp_sleep_enable_ext1_wakeup(ext_wakeup_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_start(); // Enter deep sleep
  }
  delay(300); // Small delay to debounce and make serial output readable
}