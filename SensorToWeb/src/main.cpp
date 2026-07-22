/* ============================================================
 * PROJECT:     Sensor Dashboard over WiFi (Web Server)
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *  This project reads an HC-SR04 ultrasonic distance sensor and a
 *  PIR motion sensor on a dedicated FreeRTOS task, then serves the
 *  latest readings as a self-refreshing HTML page over WiFi so they
 *  can be viewed from a browser instead of the Serial Monitor.
 *
 * WIRING (GPIO -> Component):
 *  - HC-SR04 Ultrasonic Sensor:
 *      TRIG to GPIO 26
 *      ECHO to GPIO 25
 *  - PIR Motion Sensor:
 *      OUT to GPIO 27
 *
 * LIBRARIES:
 *  - WebServer
 *  - WiFi
 * NOTES:
 *  - Had to install the Wokwi IoT Gateway to reach the ESP32's web
 *  server from a browser during simulation.
 *  - Had to add a meta-refresh tag to the page so it keeps polling
 *  the ESP32 for updated readings.
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */
#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

// ============================================================

#define PIN_TRIG 26
#define PIN_ECHO 25
#define PIN_MOTION 27

// ===========================================================

WebServer server(80);

const char *ssid = "Wokwi-GUEST";
const char *password = "";

int distance = 0;
bool motionDetected = false;

struct SensorData
{
    int distance;
    bool motionDetected;
};

QueueHandle_t sensorQueue;

void handleRoot()
{
    String html = "<html><head><meta http-equiv='refresh' content='1'></head><body>";
    html += "<h1>Sensor Status</h1>";
    html += "<p>Distance: " + String(distance) + " cm</p>";
    html += "<p>Motion: " + String(motionDetected ? "Yes" : "No") + "</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void readData(void *pvParameters)
{
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    pinMode(PIN_MOTION, INPUT);
    SensorData data;

    for (;;)
    {

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
        if (xQueueOverwrite(sensorQueue, &data) != pdTRUE)
        {
            Serial.println("Failed to send data to queue");
        }
    }
}

// ============================================================
void setup()
{

    Serial.begin(9600);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
    }

    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());

    server.on("/", handleRoot);
    server.begin();

    sensorQueue = xQueueCreate(1, sizeof(SensorData)); // holds up to 1 pending reading
    if (sensorQueue == NULL)
    {
        Serial.println("Failed to create queue");
        while (1); // Stop execution if queue creation fails
    }

    xTaskCreatePinnedToCore(readData, "readData", 4096, NULL, 1, NULL, 1);
}

void loop()
{
    server.handleClient();
    
    SensorData received;
    if (xQueueReceive(sensorQueue, &received, 0) == pdTRUE) {// 0 = don't block, just check
        distance = received.distance;
        motionDetected = received.motionDetected;
    }
}
