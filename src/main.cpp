#include <Arduino.h>
#include "esp_system.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h" // For rtc_gpio_pulldown_en and rtc_gpio_deinit
#include "led.h"

// Define the pin connected to the Common (COM) terminal of your SPDT switch
// IMPORTANT: This MUST be an RTC GPIO for external wakeup (EXT0/EXT1) to work!
// Common RTC GPIOs include: GPIO0, 2, 4, 12-15, 25-27, 32-39.
// Check your ESP32 dev board's pinout for specifics.
#define TSW_PIN GPIO_NUM_5 // Example RTC GPIO (e.g., GPIO32)

// RTC_DATA_ATTR: Variable stored in RTC memory, persists through deep sleep
RTC_DATA_ATTR int bootCount = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000); // Give time for serial monitor to connect

  ++bootCount;
  Serial.println("\n------------------------------");
  Serial.println("Boot Count: " + String(bootCount));
  Serial.println("------------------------------");

  // If we woke up from EXT0, we need to deinitialize the RTC GPIO
  // so we can use it as a regular digital pin again.
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0)
  {
    rtc_gpio_deinit(TSW_PIN);
  }

  // Configure the switch pin.
  // We use INPUT_PULLDOWN. If the switch connects to 3.3V, it reads HIGH.
  // If the switch connects to GND, it reads LOW (pulled down by internal resistor).
  pinMode(TSW_PIN, INPUT_PULLDOWN);

  // Read the state of the switch immediately
  int switchState = digitalRead(TSW_PIN);
  Serial.print("TSW_PIN (GPIO ");
  Serial.print(TSW_PIN);
  Serial.print(") state: ");
  Serial.println(switchState == HIGH ? "HIGH (Stay Awake)" : "LOW (Go to Deep Sleep)");

  if (switchState == LOW)
  { // Switch in "Deep Sleep" position
    Serial.println("SPDT switch set to DEEP SLEEP mode.");

    // Configure EXT0 wakeup: wake up if TSW_PIN goes HIGH
    // TSW_PIN must be an RTC GPIO.
    // The '1' means wake on HIGH. Use '0' to wake on LOW.
    esp_sleep_enable_ext0_wakeup(TSW_PIN, HIGH);

    // This is important: ensure the internal pulldown is enabled for deep sleep
    // so the pin isn't floating and can reliably detect HIGH.
    rtc_gpio_pulldown_en(TSW_PIN);

    Serial.println("Entering Deep Sleep now. Flip switch to HIGH to wake up immediately.");
    Serial.flush(); // Ensure all serial output is sent
    esp_deep_sleep_start();
  }
  else
  { // switchState == HIGH, Switch in "Stay Awake" position
    Serial.println("SPDT switch set to STAY AWAKE mode.");
    Serial.println("Continuing with normal execution (loop() function).");

    // --- Your normal setup tasks for continuous operation go here ---
    pinMode(LED_BUILTIN, OUTPUT);
    turnLEDGreen(LED_BUILTIN); // Toggle LED
    Serial.println("LED_BUILTIN is ON.");
    // ------------------------------------------------------------------
  }
}

void loop()
{
  // This loop will only execute if the SPDT switch was in the "STAY AWAKE" position in setup()

  Serial.println("Looping... (LED blinking)");
  turnLEDGreen(LED_BUILTIN); // Toggle LED
  delay(1000);
  turnLEDBlue(LED_BUILTIN); // Toggle LED
  delay(1000);

  // If you want to dynamically enter deep sleep from loop if the switch is flipped
  // (e.g., you start in active mode, then flip the switch to put it to sleep)
  // you can add this:
  if (digitalRead(TSW_PIN) == LOW)
  {
    Serial.println("Switch moved to DEEP SLEEP position while in loop. Entering sleep.");
    Serial.flush();
    // Configure EXT0 wakeup before going to sleep
    esp_sleep_enable_ext0_wakeup(TSW_PIN, HIGH); // Still wake on HIGH
    rtc_gpio_pulldown_en(TSW_PIN);               // Ensure pulldown is enabled during sleep
    turnLEDOff(LED_BUILTIN);
    esp_deep_sleep_start();
  }
}