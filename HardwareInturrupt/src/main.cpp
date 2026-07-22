/* ============================================================
 * PROJECT:     Hardware Interrupt Demo (PIR motion ISR + ultrasonic + I2C LCD)
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-22
 *
 * DESCRIPTION:
 *  This project demonstrates a hardware interrupt (ISR) attached to a PIR
 *  motion sensor's rising edge, alongside polled ultrasonic distance
 *  readings. A FreeRTOS task consumes both readings and displays them on
 *  an I2C LCD.
 *
 * WIRING (GPIO -> Component):
 * - GPIO 21 (SDA) -> LCD SDA
 * - GPIO 22 (SCL) -> LCD SCL
 * - GPIO 26 (TRIG) / GPIO 25 (ECHO) -> HC-SR04 ultrasonic sensor
 * - GPIO 32 -> PIR motion sensor OUT (interrupt pin)
 *
 * LIBRARIES:
 * - LiquidCrystal_I2C: For controlling the I2C LCD.
 * - Wire: For I2C communication.
 *
 * NOTES:
 * - volatile tells the compiler this value
 *   can change outside normal program flow (from an interrupt),
 *   so it must re-read from memory every time, never cache/optimize it away.
 * - ISR: must be short, fast, and marked IRAM_ATTR so it's placed in
 *   fast internal RAM rather than flash.
 * - DO NOT DO HEAVY WORK IN AN ISR! Just set a flag and let the main loop handle it.
 * - The PIR sensor, when triggerred, will stay high for 5 seconds, and will wait 1.2
 *  seconds before it can trigger again; however, the ISR will trigger on the rising
 *  edge of the signal, so it will only set the flag once per motion event with atleast
 *  6.2 seconds between events.
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

#define PIN_TRIG 26
#define PIN_ECHO 25

#define PIN_MOTION 32

// ===========================================================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Flag set by the ISR, read by the sensor task.
volatile bool motionFlag = false;

struct SensorData
{
    int distance;
    bool motionDetected;
};

// placed in fast internal RAM rather than flash as to not be interrupted
void IRAM_ATTR motionISR()
{
    motionFlag = true;
}

QueueHandle_t sensorQueue;

// ============================================================

void readDataTask(void *pvParameters)
{
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIN_MOTION, INPUT);

    // Register the ISR: call motionISR() whenever PIN_MOTION goes LOW->HIGH
    attachInterrupt(digitalPinToInterrupt(PIN_MOTION), motionISR, RISING);

    for (;;)
    {
        SensorData data;

        digitalWrite(PIN_TRIG, HIGH);
        delayMicroseconds(10);
        digitalWrite(PIN_TRIG, LOW);

        long duration = pulseIn(PIN_ECHO, HIGH);
        data.distance = duration / 58;

        // Check and consume the flag set by the ISR
        if (motionFlag)
        {
            data.motionDetected = true;
            motionFlag = false; // reset after reading
        }
        else
        {
            data.motionDetected = false;
        }

        xQueueOverwrite(sensorQueue, &data);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void displayTask(void *pvParameters)
{
    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.init();
    lcd.backlight();

    SensorData received;
    for (;;)
    {
        if (xQueueReceive(sensorQueue, &received, portMAX_DELAY) == pdTRUE)
        {
            lcd.setCursor(0, 0);
            lcd.print("Dist:");
            lcd.print(received.distance);
            lcd.print("cm    ");

            lcd.setCursor(0, 1);
            lcd.print("Motion:");
            lcd.print(received.motionDetected ? "Yes" : "No ");
        }
    }
}

// ============================================================

void setup()
{
    Serial.begin(9600);
    sensorQueue = xQueueCreate(1, sizeof(SensorData));

    xTaskCreatePinnedToCore(readDataTask, "SensorTask", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(displayTask, "DisplayTask", 4096, NULL, 1, NULL, 1);
}

void loop()
{
}