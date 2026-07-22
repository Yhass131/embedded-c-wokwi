/* ============================================================
 * PROJECT:     I2C Display Demo
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-22
 *
 * DESCRIPTION:
 * This project counts from 0 to 10 and displays the running
 * count on the top line of an I2C LCD, with a countdown to 10
 * right-aligned on the bottom line. Once the counter hits 10,
 * "Resetting" is shown briefly before the count starts over.
 *
 * WIRING (GPIO -> Component):
 * - GPIO 21 (SDA) -> LCD SDA
 * - GPIO 22 (SCL) -> LCD SCL
 *
 * LIBRARIES:
 * - LiquidCrystal_I2C: For controlling the I2C LCD.
 * - Wire: For I2C communication.
 *
 * NOTES:
 *  - counter is a uint8_t since it only ever ranges 0-10.
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 21
#define SCL_PIN 22

LiquidCrystal_I2C lcd(0x27, 16, 2); // Create an LCD object with I2C address 0x27, 16 columns, and 2 rows
uint8_t counter = 0;

// This function prints an integer value right-aligned on the LCD at a specified row.
void printRightAligned(int value, int row, int screenWidth = 16) {
  String str = String(value);
  int startCol = screenWidth - str.length();
  lcd.setCursor(startCol, row);
  lcd.print(str);
}

void setup() {
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN); // ESP32 default I2C pins
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  lcd.init();
  lcd.backlight(); 

  lcd.print("Start Up");
  delay(2000); // Display "Start Up" for 2 seconds
} 

void loop() {
    lcd.clear(); // Clear the LCD for the next value
    lcd.setCursor(0, 0); // Set cursor to first column, first row
    
    lcd.print(counter); // Print the counter value on the LCD
    if(counter == 10) {
        lcd.setCursor(7, 0); 
        lcd.print("Resetting");
        lcd.setCursor(0, 1);
        lcd.print("Resetting");
    }
    
    lcd.setCursor(15, 1);
    printRightAligned(10 - counter, 1);

    counter++; 
    delay(1000); // Wait for 1 second before the next loop iteration

    if(counter > 10) {
        counter = 0;
    }
}