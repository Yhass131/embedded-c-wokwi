/* ============================================================
* PROJECT:     Two Task Communication Demo
* BOARD:       ESP32 DevKit (esp32dev)
* AUTHOR:      Yunus Hassen
* DATE:        2026-07-22
*
* DESCRIPTION:
*   This project demonstrates inter-task communication between 
*   two FreeRTOS tasks on an ESP32.
*
* WIRING (GPIO -> Component):
* - HC-SR04 Ultrasonic Sensor:
*     TRIG to GPIO 26
*     ECHO to GPIO 25
* - PIR Motion Sensor:
*     OUT to GPIO 32
* - LCD Display:
*     SDA to GPIO 21
*     SCL to GPIO 22
*
* LIBRARIES:
*  LiquidCrystal_I2C
*  Wire
*
* NOTES:
*  - FreeRTOS uses pdTRUE and pdFALSE for boolean values.
*    Since FreeRTOS predates stdbool and C++11, it uses its own
*    boolean definitions to stay universal.
*
*  - The queue is created with a length of 1, meaning it can hold only one pending reading at a time. 
*    If the queue is full, the new reading will overwrite the old one. 
*    This is suitable for this application since we only need the latest reading.
*    Generally, a queue length of 5 or more is used to hold multiple readings, 
*    but for this application, a length of 1 is sufficient.
*    
* BUILD & RUN:
*   pio run
*   wokwi-cli .
* ============================================================ */
// Libraries

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ============================================================
// Decleration

#define SDA_PIN 21
#define SCL_PIN 22

#define PIN_TRIG 26
#define PIN_ECHO 25

#define PIN_MOTION 32 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Struct to bundle sensor data for queue communication
struct SensorData {
  int distance;
  bool motionDetected;
};

// Queue handle for inter-task communication
QueueHandle_t sensorQueue;

// ============================================================
// Functions

void displayTask(void *pvParameters) {
  // Initialize I2C communication and the LCD
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight(); 
  lcd.print("Start Up");
  delay(1000);

  SensorData received; // Variable to hold received data from the queue

  for (;;) {
    // Wait indefinitely for data from the sensor task and display it on the LCD form received queue data
    if (xQueueReceive(sensorQueue, &received, portMAX_DELAY) == pdTRUE) {
      lcd.setCursor(0, 0);
      lcd.print("Motion: " + String(received.motionDetected ? "Yes" : "No "));
      
      lcd.setCursor(0, 1);
      lcd.print("Dist: " + String(received.distance) + " cm   ");
      vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for readability
    }
  }
}

void sensorTask(void *pvParameters) {
  // Set up the ultrasonic sensor pins
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  /* 
      Wokwi HC-SR04 Ultrasonic Distance Sensor
    To start a new distance measurement 
    set the TRIG pin to high for 10uS or more. 
    Then wait until the ECHO pin goes high, 
    and count the time it stays high (pulse length).
    The length of the ECHO high pulse is proportional 
    to the distance. Use the following table to 
    convert the ECHO pulse length in 
    microseconds into centimeters / inches:
    Centimeters = pulse length / 58
    Inches = pulse length / 148

    https://docs.wokwi.com/parts/wokwi-hc-sr04
  */

  // Set up the motion sensor pin
  pinMode(PIN_MOTION, INPUT);

  /* 
      Wokwi Passive Infrared (PIR) motion sensor.
    Triggering the sensor will drive the OUT pin high 
    for 5 seconds (delay time), and then go low again. 
    The sensor will ignore any further input for the 
    next 1.2 seconds (inhibit time), and then start 
    sensing for motion again.

    https://docs.wokwi.com/parts/wokwi-pir-motion-sensor
  */

  for (;;) {
    SensorData data;
    
    // Start a new measurement:
    digitalWrite(PIN_TRIG, HIGH); 
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
    
    // Read the ECHO pulse length:
    long duration = pulseIn(PIN_ECHO, HIGH);  
    data.distance = duration / 58; // Convert to centimeters

    // Read the motion sensor state:
    data.motionDetected = digitalRead(PIN_MOTION);

    // Send data to the display task:
    if (xQueueOverwrite(sensorQueue, &data) != pdTRUE) {
      Serial.println("Failed to send data to queue");
    }

  }
}


// ============================================================
// Main Setup and Loop

void setup() {
  Serial.begin(9600);

  sensorQueue = xQueueCreate(1, sizeof(SensorData)); // holds up to 1 pending reading
  if(sensorQueue == NULL) {
    Serial.println("Failed to create queue");
    while(1); // Stop execution if queue creation fails
  }
  
  xTaskCreatePinnedToCore(sensorTask, "SensorTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(displayTask, "DisplayTask", 4096, NULL, 1, NULL, 1);

  delay(2000);
} 

void loop() {
  // No code needed here since tasks are running on separate cores
  // Did use for testing purposes to debugg
}