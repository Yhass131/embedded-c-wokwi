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

More demos will be added as I go (I2C sensors, interrupts, timers, BLE, etc.).

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
└── README.md
```

## Project notes

- **LEDdemo** — Button is active-low (`INPUT_PULLUP`); LED follows button state through a simple polling `loop()` with a 100 ms debounce delay.
- **ADCnPOT** — Reads raw ADC1 counts (0–4095) on GPIO 34 and prints them over Serial at 9600 baud every 500 ms.
- **Keypad** — Uses the `Keypad` library's row/column scan matrix; prints the pressed key character to Serial when detected.
- **PMW_LED** — Combines ADC read + `map()` + `analogWrite()` to drive LED brightness proportionally to the potentiometer position; also logs both raw and mapped values to Serial.
- **TFTdisplay** — Initializes the ILI9341 over SPI, clears the screen, and prints sample text in multiple colors/sizes. `loop()` is intentionally empty (static display).
- **7seg4dig** — Runs the display refresh on a dedicated FreeRTOS task (pinned to core 1) so `sevseg.refreshDisplay()` gets called continuously regardless of what `loop()` is doing; `loop()` just parses integers from Serial and updates a `volatile` shared value. Set `hardwareConfig` in the code to match your display (common anode vs. common cathode).

## Notes

- `.pio/` build artifacts and `.vscode/` settings are gitignored — only source and config files are tracked.
- Each demo's `diagram.json` reflects its specific wiring; check it alongside the code comments for pin mapping.
