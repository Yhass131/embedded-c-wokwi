/* ============================================================
 * PROJECT:     WiFi NTP Clock on ILI9341 TFT Display
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *  This project connects the ESP32 to a WiFi network, syncs the
 *  system clock with an NTP server, and renders a live-updating
 *  HH:MM:SS clock on a 240x320 2.8" ILI9341 TFT display (SPI).
 *
 * WIRING (GPIO -> Component):
 *  - TFT_CS (Chip Select):  5
 *  - TFT_DC (Data/Command):  2
 *  - TFT_RST (Reset, optional):  4
 *
 * LIBRARIES:
 *  - Adafruit GFX Library:
 *  - Adafruit ILI9341 Library:
 *  - WiFi Library:
 *  - time.h:
 * NOTES:
 *  - not including CLEAR_TFT in the main loop to avoid clearing 
 *  the screen every second, which would refresh too quickly, 
 *  which can cause flickering.
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <WiFi.h>
#include <time.h>

#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4

#define CLEAR_TFT tft.fillScreen(ILI9341_WHITE) // Clear the screen with white color

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup()
{
    tft.begin();

    CLEAR_TFT;

    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_BLACK); // Set text color to black with white background
    tft.setTextSize(2);

    tft.print("Connecting to WiFi");
    delay(1000); // Wait for a second before starting the connection process
    CLEAR_TFT;
    tft.setCursor(10, 10);

    WiFi.begin("Wokwi-GUEST", "", 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        tft.print(".");
    }

    CLEAR_TFT;
    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(3);
    tft.print("Connected!");
    delay(1000); // Wait for a second before displaying the IP address
    CLEAR_TFT;

    tft.setCursor(10, 10);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(3);
    tft.print("IP: ");
    tft.print(WiFi.localIP());
    delay(1000);
    CLEAR_TFT;

    long gmtOffset_sec = -5 * 3600; // US Eastern Time (Virginia): UTC−5 during standard time
    int daylightOffset_sec = 3600;
    // sets up the ESP32's internal clock to sync with an NTP (Network Time Protocol) server the WiFi connection
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
}

void loop()
{
    // breaks a moment in time down into individual human-readable fields
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        tft.print("Time sync failed");
        delay(1000);
        return;
    }

    char timeString[9]; // HH:MM:SS
    strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
    tft.fillRect(0, 0, 240, 40, ILI9341_WHITE); // clear just the time area
    tft.setCursor(10, 10);
    tft.print(timeString);

    delay(1000); // Update every second
}
