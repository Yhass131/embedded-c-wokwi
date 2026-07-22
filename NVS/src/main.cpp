/* ============================================================
 * PROJECT:     Non-Volatile Storage (NVS) Counter
 * BOARD:       ESP32 DevKit (esp32dev)
 * AUTHOR:      Yunus Hassen
 * DATE:        2026-07-19
 *
 * DESCRIPTION:
 *  This project demonstrates using Non-Volatile Storage (NVS) to
 *  persist a counter value across restarts. The counter increments
 *  every 500 ms, is printed to the Serial Monitor, and is saved to
 *  NVS on every update. Every 10 counts, the ESP32 triggers a soft
 *  restart (esp_restart) to prove the stored value survives reboot.
 *
 * WIRING:
 *  None — pure firmware demo, no external components.
 *
 * LIBRARIES:
 * - Preferences.h: For handling non-volatile storage (NVS) on the ESP32.
 * - nvs_flash.h: For initializing and managing the NVS flash storage.
 * - esp_system.h: For system-level functions, including soft restart.
 *
 * NOTES:
 *  - Had to trigger a soft restart to test NVS persistence, since the
 *  counter value is stored in NVS and should persist across restarts.
 *  Wokwi does not simulate real flash wear/power loss, so the soft
 *  restart is just a stand-in for testing that the value survives.
 *
 * BUILD & RUN:
 *   pio run
 *   wokwi-cli .
 * ============================================================ */

#include <Arduino.h>
#include <Preferences.h>
#include <nvs_flash.h>
#include <esp_system.h>

Preferences prefs;
int counter;


void setup()
{
    Serial.begin(9600);

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition needs to be erased/reformatted first
        nvs_flash_erase();
        err = nvs_flash_init();
    }
    
    prefs.begin("counter", false); // "counter" = namespace, false = read/write mode
    counter = prefs.getInt("eventCount", 0); // Retrieve the stored value, 0 if not found
    prefs.end(); // always close when done
}

void loop()
{
    Serial.print("Counter Value: ");
    Serial.println(counter);
    delay(500);

    if (counter > 100) {
        counter = 0;
    } else {
        counter++;
    }

    prefs.begin("counter", false);
    prefs.putInt("eventCount", counter);
    prefs.end();

    // Trigger a soft restart every 10 counts, as a test
    if (counter % 10 == 0) {
        Serial.println("Soft restarting to test NVS persistence...");
        delay(200);
        esp_restart();
    }
}