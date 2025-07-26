#include <Arduino.h>
#include "esp_system.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h" // For rtc_gpio_pulldown_en
#include "led.h"

// Define the pin connected to the Common (COM) terminal of your SPDT switch
// IMPORTANT: For reliable detection at boot, especially for deep sleep,
// it's best if this is an RTC GPIO, even if not directly used for wakeup,
// as RTC GPIOs retain their state better across reset types.
#define TSW_PIN GPIO_NUM_5 // Example RTC GPIO (e.g., GPIO32, check your board!)

// Define a timer wakeup period (if you want an alternative way to wake up)
#define TIME_TO_SLEEP_SEC 10 // Example: wake up after 10 seconds if in deep sleep
#define uS_TO_S_FACTOR 1000000ULL

// RTC_DATA_ATTR: Variable stored in RTC memory, persists through deep sleep
// This is useful for retaining data across deep sleep cycles, like a boot counter.
RTC_DATA_ATTR int bootCount = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000); // Give time for serial monitor to connect

  ++bootCount;
  Serial.println("\n------------------------------");
  Serial.println("Boot Count: " + String(bootCount));
  Serial.println("------------------------------");

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
    Serial.println("Configuring timer wakeup in " + String(TIME_TO_SLEEP_SEC) + " seconds.");

    // Configure timer as a wakeup source
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * uS_TO_S_FACTOR);

    // Optional: You could also enable EXT1 wakeup on the TSW_PIN if you wanted
    // to wake up if the switch position changed while sleeping.
    // However, if the switch's purpose is to *decide* the mode at boot,
    // a timer is often more suitable for deep sleep.
    // If you want the switch to be an 'on-demand' wakeup, then use ext1.
    // For this example, we'll assume the switch defines the mode at boot.

    Serial.println("Entering Deep Sleep now.");
    Serial.flush(); // Ensure all serial output is sent
    esp_deep_sleep_start();
  }
  else
  { // switchState == HIGH, Switch in "Stay Awake" position
    Serial.println("SPDT switch set to STAY AWAKE mode.");
    Serial.println("Continuing with normal execution (loop() function).");

    // --- Your normal setup tasks for continuous operation go here ---
    pinMode(LED_BUILTIN, OUTPUT);
    turnLEDGreen(LED_BUILTIN);
    Serial.println("LED_BUILTIN is ON.");
    // ------------------------------------------------------------------
  }
}

void loop()
{
  // This loop will only execute if the SPDT switch was in the "STAY AWAKE" position in setup()

  Serial.println("Looping... (LED blinking)");
  turnLEDGreen(LED_BUILTIN);
  // Toggle LED
  delay(1000);
  turnLEDBlue(LED_BUILTIN);
  delay(1000);

  // You might want to periodically check the switch state here if you
  // want to dynamically go into deep sleep *from* the loop,
  // without a full power cycle.
  // For example:
  if (digitalRead(TSW_PIN) == LOW)
  {
    Serial.println("Switch moved to DEEP SLEEP while in loop. Entering sleep.");
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP_SEC * uS_TO_S_FACTOR);
    turnLEDOff(LED_BUILTIN);
    esp_deep_sleep_start();
  }
}