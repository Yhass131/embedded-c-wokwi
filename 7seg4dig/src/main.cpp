/* ============================================================
 * PROJECT:     7-Segment, 4-Digit Display Demo
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-21
 *
 * DESCRIPTION:
 *  This project demonstrates how to use the SevSeg library to 
 *  control a 4-digit 7-segment display with an ESP32. The display 
 *  shows numbers entered via the serial monitor.
 *
 * WIRING (GPIO -> Component):
 *  Digits 1-4: 21, 19, 18, 17
 *  Segments A-G: 14, 13, 32, 33, 25, 26, 27, 28
 *
 * LIBRARIES:
 *  SevSeg
 * 
 * NOTES:
 * Having 'disableDecPoint' set to 'true' is important if your 
 * decimal point isn't connected, as it prevents the library from 
 * trying to control a non-existent segment, casung an error.
 * 
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */

#include <Arduino.h>
#include <SevSeg.h>

SevSeg sevseg;

volatile int displayValue = 0; // shared between tasks, must be volatile

void displayTask(void *pvParameters) {
  byte numDigits = 4;
  byte digitPins[] = {21, 19, 18, 17};
  byte segmentPins[] = {14, 13, 32, 33, 25, 26, 27, 28}; // A,B,C,D,E,F,G — no DP wired
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = true; // Use 'true' if your decimal point doesn't exist or isn't connected

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);

  for (;;) {
    sevseg.setNumber(displayValue, 0); // Display the current value
    sevseg.refreshDisplay(); // Must be called repeatedly
    vTaskDelay(10 / portTICK_PERIOD_MS); // Delay to allow other tasks to run
  }
}

void setup() {

  xTaskCreatePinnedToCore(
    displayTask,   // function
    "DisplayTask", // name
    4096,          // stack size (words)
    NULL,          // params
    1,             // priority
    NULL,          // task handle
    1              // core 1 (core 0 often runs WiFi/BT stack)
  );

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    int input = Serial.parseInt();
    if (input >= 0 && input <= 9999) {
      displayValue = input;
    } else {
      Serial.println("Please enter a number between 0 and 9999.");
    }
  }
}
