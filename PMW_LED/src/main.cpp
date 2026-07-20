/* ============================================================
 * PROJECT:     PWM LED Brightness Control with Potentiometer
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION: 
 *  Use the ESP32's ADC to read a potentiometer value and
 *  control the brightness of an LED using PWM. The potentiometer
 *  value is also output to the Serial Monitor.
 *
 * WIRING (GPIO -> Component):
 *  - Potentiometer ADC input:  34
 *  - LED:  5
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
#define PotentiometerPin 34 // ADC1 channel 6

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(PotentiometerPin, INPUT);
}

void loop() {
  int potValue = analogRead(PotentiometerPin);
  int pwmValue = map(potValue, 0, 4095, 0, 255); // Map ADC value to PWM range
  analogWrite(LED, pwmValue);
  
  Serial.print("Potentiometer Value: ");
  Serial.print(potValue);
  Serial.print(" | PWM Value: ");
  Serial.println(pwmValue);
  
  delay(100); // Delay for readability
}