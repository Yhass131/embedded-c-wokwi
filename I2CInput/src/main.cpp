/* ============================================================
 * PROJECT:     I2C Barometric pressure and temperature sensor
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-22
 *
 * DESCRIPTION:
 * This project reads temperature, pressure, and altitude data 
 * from a BMP085 sensor using I2C communication. The data is 
 * printed to the serial monitor.
 * 
 * WIRING (GPIO -> Component):
 * - GPIO 21 (SDA) -> BMP SDA
 * - GPIO 22 (SCL) -> BMP SCL
 *
 * LIBRARIES:
 * - Adafruit_BMP085: For controlling the I2C BMP085 sensor.
 * - Wire: For I2C communication.
 *
 * NOTES:
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define BMP_SCL 22
#define BMP_SDA 21

Adafruit_BMP085 bmp;

void setup()
{
    Serial.begin(9600);
    Wire.begin(BMP_SDA, BMP_SCL);

    if (!bmp.begin())
    {
        Serial.println("BMP180 not found!");
        while (1)
            ;
    }

    delay(2000);
}

void loop()
{
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0; // Convert to hPa
    float altitude = bmp.readAltitude();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa");

    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" m");

    Serial.println();
    delay(1000);
}