# ESP32 Embedded Systems Playground

A collection of self-contained ESP32 demos built while learning embedded C, PlatformIO, and hardware simulation with Wokwi. Each folder is an independent project — its own `platformio.ini`, `wokwi.toml`, `diagram.json`, and `src/main.cpp` — covering a specific peripheral or concept.

## Tools

- **[PlatformIO](https://platformio.org/)** — build system and dependency management
- **[Wokwi](https://wokwi.com/)** — circuit simulation ([Wokwi CLI](https://docs.wokwi.com/wokwi-ci/cli-installation) for local runs)
- **Board:** ESP32 DevKit (`esp32dev`)
- **Framework:** Arduino

## Projects

| # | Project | Concept |
|---|---------|---------|
| 1 | [`LEDdemo`](./LEDdemo) | Digital I/O — LED output + pushbutton input with `INPUT_PULLUP` |
| 2 | [`ADCnPOT`](./ADCnPOT) | Analog input — reading a potentiometer via ADC1, voltage mapping |
| 3 | [`Keypad`](./Keypad) | Matrix scanning — 4x4 keypad row/column input via the `Keypad` library |
| 4 | [`PWM-LED`](./PWM-LED) | PWM output — mapping analog input to LED brightness with `analogWrite` |
| 5 | [`TFT-display`](./TFT-display) | SPI communication — driving an ILI9341 TFT display with `Adafruit_GFX` |

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
├── PWM-LED/
│   └── ...
├── TFT-display/
│   └── ...
└── README.md
```

## Notes

- `.pio/` build artifacts and `.vscode/` settings are gitignored — only source and config files are tracked.
- Each demo's `diagram.json` reflects its specific wiring; check it alongside the code comments for pin mapping.