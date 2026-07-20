/* ============================================================
 * PROJECT:     ADC with potentiometer with Serial Output
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *   Use the ESP32's ADC to read a potentiometer 
 *   value and output it to the Serial Monitor.
 *
 * WIRING (GPIO -> Component):
 *  - Potentiometer ADC input:  34
 *
 * LIBRARIES:
 *
 * NOTES:
 *  - Had potValue as a unit8_t, but changed to int 
 *  to accommodate the full range of ADC values (0-4095).
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */

 #include <Arduino.h>

#define PotentiometerPin 34 // ADC1 channel 6

void setup() {
  Serial.begin(9600);
  pinMode(PotentiometerPin, INPUT);
} 

void loop() {
  int potValue = analogRead(PotentiometerPin);
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);
  delay(500); // Delay for readability
}