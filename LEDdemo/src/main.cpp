/* ============================================================
 * PROJECT:     LED and Button Demo
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *  This project demonstrates how to use a button to control 
 *  an LED on an ESP32 DevKit board.
 *
 * WIRING (GPIO -> Component):
 *  - LED:  5
 *  - ButtonInput:  17
 *  - TFT_RST (Reset, optional):  4
 *
 * LIBRARIES:
 *
 * NOTES:
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */

#include <Arduino.h>

#define LED 5
#define ButtonInput 17

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(ButtonInput, INPUT_PULLUP);
}

void loop() {
  uint8_t buttonState = digitalRead(ButtonInput);
  if (buttonState == LOW) { // Button is pressed
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(100); // Add a small delay to avoid bouncing issues
}
