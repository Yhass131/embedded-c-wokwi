/* ============================================================
 * PROJECT:     Keypad Input with Serial Output
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *  Use the ESP32 to read input from a 4x4 keypad and 
 *  output the pressed key to the Serial Monitor.
 *
 * WIRING (GPIO -> Component):
 *  - Keypad rows: 32, 33, 25, 26
 *  - Keypad columns: 27, 14, 12, 13
 *
 * LIBRARIES:
 *  Keypad.h: driver for interfacing with matrix keypads
 * 
 * NOTES:
 *  had to add the Keypad library to the project using 
 *  PlatformIO's library manager. Add 'chris--a/Keypad'
 *  to the lib_deps section of platformio.ini.
 * 
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */
#include <Arduino.h>
#include <Keypad.h>

#define ROWS 4
#define COLS 4  
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

uint8_t rowPins[ROWS] = {32, 33, 25, 26}; // Connect to the row pinouts of the keypad
uint8_t colPins[COLS] = {27, 14, 12, 13}; // Connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
/*
  Keypad object initialization parameters:
    -keys: 2Darr of physical keypad layout
    -rowPins/colpins: GPIOs connected to the rows and columns of the keypad
    -ROWS/COLS: number of rows and columns in the keypad
*/ 

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();
  if (key) { // Check if a key is pressed
    Serial.print("Key Pressed: ");
    Serial.println(key);
  }
}