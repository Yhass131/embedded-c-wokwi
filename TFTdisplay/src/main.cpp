/* ============================================================
 * PROJECT:     240x320 2.8" LCD-TFT display with SPI interface
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *  This project demonstrates how to use the Adafruit ILI9341 
 *  library to control a 240x320 2.8" LCD-TFT display with an 
 *  SPI interface. The display is initialized, cleared, and text 
 *  is displayed in different colors and sizes.
 *
 * WIRING (GPIO -> Component):
 *  - TFT_CS (Chip Select):  5
 *  - TFT_DC (Data/Command):  2
 *  - TFT_RST (Reset, optional):  4
 *
 * LIBRARIES:
 *
 * NOTES:
 *  - Had to add the Adafruit ILI9341 and Adafruit GFX libraries 
 *  to the platformio.ini file.
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */
#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
/*
  The ILI9341 is a controller chip embedded on the TFT module 
  itself — it handles the actual pixel-pushing to the LCD panel.
  The Adafruit_ILI9341 class wraps all of that low-level SPI 
  command protocol into simple function calls like println(), 
  fillScreen(), drawPixel(), etc.

    - TFT_CS (Chip Select):  tells the display "the next SPI 
      traffic is for you.CS is how the display knows when to 
      listen versus ignore

    - TFT_DC (Data/Command):  tells the controller chip whether 
    the current byte being sent over SPI is a command or pixel data.

    - TFT_RST (Reset, optional): pulses the display's hardware 
    reset line on startup. Assumes dealt with if not provided.
*/

void setup() {
  tft.begin();  // initializes the display (sends init command sequence over SPI)

  tft.fillScreen(ILI9341_BLACK);// Clear the screen with black color
  tft.setRotation(1); // Landscape mode

  tft.setCursor(26, 120);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3);
  tft.println("Hello, World!");


  tft.setCursor(20, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.println("I can has colors?");
}

void loop() { }

