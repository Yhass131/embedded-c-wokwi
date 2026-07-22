# ESP32 Embedded Systems Playground

A collection of self-contained ESP32 demos built while learning embedded C, PlatformIO, and hardware simulation with Wokwi. Each folder is an independent project — its own `platformio.ini`, `wokwi.toml`, `diagram.json`, and `src/main.cpp` — covering a specific peripheral or concept.

## Tools

- **[PlatformIO](https://platformio.org/)** — build system and dependency management
- **[Wokwi](https://wokwi.com/)** — circuit simulation ([Wokwi CLI](https://docs.wokwi.com/wokwi-ci/cli-installation) for local runs)
- **Board:** ESP32 DevKit (`esp32dev`)
- **Framework:** Arduino

## Projects

| # | Project | Concept | Key GPIO wiring | Libraries |
|---|---------|---------|------------------|-----------|
| 1 | [`LEDdemo`](./LEDdemo) | Digital I/O — LED output + pushbutton input with `INPUT_PULLUP` | LED `5`, Button `17` | — |
| 2 | [`ADCnPOT`](./ADCnPOT) | Analog input — reading a potentiometer via ADC1, voltage mapping | Potentiometer `34` | — |
| 3 | [`Keypad`](./Keypad) | Matrix scanning — 4x4 keypad row/column input via the `Keypad` library | Rows `32,33,25,26`, Cols `27,14,12,13` | `chris--a/Keypad` |
| 4 | [`PMW_LED`](./PMW_LED) | PWM output — mapping analog input to LED brightness with `analogWrite` | Potentiometer `34`, LED `5` | — |
| 5 | [`TFTdisplay`](./TFTdisplay) | SPI communication — driving an ILI9341 TFT display with `Adafruit_GFX` | CS `5`, DC `2`, RST `4` | `Adafruit ILI9341`, `Adafruit GFX Library` |
| 6 | [`7seg4dig`](./7seg4dig) | Multiplexed display + FreeRTOS — driving a 4-digit 7-segment display from a pinned task, value set over Serial | Digits `21,19,18,17`, Segments `14,13,32,33,25,26,27,28` | `SevSeg` |
| 7 | [`I2CInput`](./I2CInput) | I2C communication — reading temperature, pressure, and altitude from a BMP085 sensor | BMP SDA `21`, SCL `22` | `Adafruit BMP085 Library` |
| 8 | [`I2Cdisplay`](./I2Cdisplay) | I2C communication — driving a 16x2 character LCD with `LiquidCrystal_I2C`, right-aligned text formatting | LCD SDA `21`, SCL `22` | `LiquidCrystal_I2C` |
| 9 | [`TwoTaskComm`](./TwoTaskComm) | FreeRTOS multitasking — two pinned tasks sharing sensor data through a queue (ultrasonic + PIR -> I2C LCD) | Ultrasonic TRIG `26`/ECHO `25`, PIR `32`, LCD SDA `21`/SCL `22` | `LiquidCrystal_I2C` |
| 10 | [`HardwareInturrupt`](./HardwareInturrupt) | Hardware interrupts — ISR on a PIR motion sensor's rising edge, combined with polled ultrasonic distance readings on an I2C LCD | Ultrasonic TRIG `26`/ECHO `25`, PIR `32` (interrupt pin), LCD SDA `21`/SCL `22` | `LiquidCrystal_I2C` |
| 11 | [`WIFI`](./WIFI) | WiFi + NTP — connects to a network, syncs time from an NTP server, and renders a live clock on an SPI TFT display | TFT CS `5`, DC `2`, RST `4` | `Adafruit ILI9341`, `Adafruit GFX Library` |
| 12 | [`NVS`](./NVS) | Non-Volatile Storage — persisting a counter across restarts with `Preferences`/NVS flash | None (Serial Monitor only) | — |
| 13 | [`SensorToWeb`](./SensorToWeb) | WiFi web server — serving live ultrasonic + PIR readings as a self-refreshing HTML page instead of a Serial Monitor or LCD | Ultrasonic TRIG `26`/ECHO `25`, PIR `27` | `WebServer`, `WiFi` |

This is the final mini-project in the series.

## Running a project

Each folder is a standalone PlatformIO project. From inside any project folder:

```bash
pio run          # build the firmware
wokwi-cli .       # run the Wokwi simulation
```

To flash to a physical ESP32 instead:

```bash
pio run --target upload --target monitor
```

## Setting up a new project (for future demos)

```bash
pio project init --board esp32dev --project-option="framework=arduino"
wokwi-cli init
```

Then drop code into `src/main.cpp` and wire up `diagram.json` to match.

## Repo structure

```
repo-root/
├── LEDdemo/
│   ├── platformio.ini
│   ├── wokwi.toml
│   ├── diagram.json
│   └── src/main.cpp
├── ADCnPOT/
│   └── ...
├── Keypad/
│   └── ...
├── PMW_LED/
│   └── ...
├── TFTdisplay/
│   └── ...
├── 7seg4dig/
│   └── ...
├── I2CInput/
│   └── ...
├── I2Cdisplay/
│   └── ...
├── TwoTaskComm/
│   └── ...
├── HardwareInturrupt/
│   └── ...
├── WIFI/
│   └── ...
├── NVS/
│   └── ...
├── SensorToWeb/
│   └── ...
└── README.md
```

## Reused code across projects

Later demos build on earlier ones instead of starting from scratch — this is intentional, and the code comments reflect it. When a project reuses a pattern from a simpler one, the reused portion is left lightly commented (or uncommented) in the newer file rather than re-explaining it; the detailed explanation lives in whichever project introduced the pattern first. For example:

- **HardwareInturrupt** reuses the FreeRTOS task/queue structure and sensor-polling loop from **TwoTaskComm**, swapping the polled PIR read for an ISR.
- **SensorToWeb** reuses the same ultrasonic + PIR sensor task and length-1 overwrite queue from **TwoTaskComm**/**HardwareInturrupt**, just replacing the I2C LCD output with a WiFi `WebServer` handler.
- I2C setup (`Wire.begin`, LCD init/backlight) is repeated across **I2Cdisplay**, **TwoTaskComm**, and **HardwareInturrupt** without re-deriving it each time.

If you're trying to understand *why* a piece of reused code works the way it does, check the earliest project in the table that uses it.

## Project notes

- **LEDdemo** — Button is active-low (`INPUT_PULLUP`); LED follows button state through a simple polling `loop()` with a 100 ms debounce delay.
- **ADCnPOT** — Reads raw ADC1 counts (0–4095) on GPIO 34 and prints them over Serial at 9600 baud every 500 ms.
- **Keypad** — Uses the `Keypad` library's row/column scan matrix; prints the pressed key character to Serial when detected.
- **PMW_LED** — Combines ADC read + `map()` + `analogWrite()` to drive LED brightness proportionally to the potentiometer position; also logs both raw and mapped values to Serial.
- **TFTdisplay** — Initializes the ILI9341 over SPI, clears the screen, and prints sample text in multiple colors/sizes. `loop()` is intentionally empty (static display).
- **7seg4dig** — Runs the display refresh on a dedicated FreeRTOS task (pinned to core 1) so `sevseg.refreshDisplay()` gets called continuously regardless of what `loop()` is doing; `loop()` just parses integers from Serial and updates a `volatile` shared value. Set `hardwareConfig` in the code to match your display (common anode vs. common cathode).
- **I2CInput** — Reads temperature, pressure (converted to hPa), and altitude from a BMP085 sensor once per second over I2C; halts in `setup()` if the sensor isn't detected.
- **I2Cdisplay** — Counts from 0 to 10 on an I2C LCD: running count top-left, right-aligned countdown-to-10 on the bottom row, then shows "Resetting" before looping back to 0.
- **TwoTaskComm** — Two FreeRTOS tasks pinned to core 1, communicating through a length-1 queue: `sensorTask` polls an HC-SR04 ultrasonic sensor and a PIR motion sensor and pushes each reading with `xQueueOverwrite` (so only the latest reading matters); `displayTask` blocks on `xQueueReceive` and renders distance + motion state to the I2C LCD.
- **HardwareInturrupt** — Builds on `TwoTaskComm` by replacing the polled PIR read with a real hardware interrupt: `attachInterrupt` fires an `IRAM_ATTR` ISR on the motion pin's rising edge, which just sets a `volatile` flag; the sensor task consumes and resets that flag each cycle alongside the polled ultrasonic reading, then pushes both to the display task via a length-1 queue.
- **WIFI** — Connects to WiFi (`Wokwi-GUEST`), syncs the clock via `configTime`/NTP, then prints a continuously updating `HH:MM:SS` clock to an SPI-driven ILI9341 TFT, redrawing only the time region each second (via `fillRect`) to avoid full-screen flicker.
- **NVS** — Increments a counter every 500 ms, prints it to Serial, and saves it to flash with `Preferences` (NVS) on every update; every 10 counts it calls `esp_restart()` to prove the value survives a reboot, since Wokwi doesn't otherwise simulate power loss.
- **SensorToWeb** — Reuses the ultrasonic + PIR sensor task and overwrite-queue pattern from `TwoTaskComm`, but replaces the I2C LCD with a `WebServer` on port 80: `loop()` drains the queue and serves a meta-refresh HTML page showing the latest distance/motion readings, viewable in a browser through the Wokwi IoT Gateway.

## Notes

- `.pio/` build artifacts and `.vscode/` settings are gitignored — only source and config files are tracked.
- Each demo's `diagram.json` reflects its specific wiring; check it alongside the code comments for pin mapping.
