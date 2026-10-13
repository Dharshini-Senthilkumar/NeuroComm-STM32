# NeuroComm – Brainwave-Inspired Communication System (STM32)

An embedded prototype that converts an analog input into simple
messages (YES / NO / HELP) and displays them on a 16x2 LCD.

## Hardware
- STM32F103C8 (Blue Pill)
- 10k potentiometer (emulates a brainwave-like analog signal)
- 16x2 LCD with I2C (PCF8574) backpack
- ST-Link V2 (SWD programming)

## Working
Potentiometer -> PA0 (ADC1_IN0) -> 12-bit ADC -> threshold check -> I2C LCD

| ADC value | Message |
|-----------|---------|
| 0 - 999   | YES     |
| 1000 - 2499 | NO    |
| 2500 - 4095 | HELP  |

## Connections
- Potentiometer: outer pins to 3.3V and GND, wiper to PA0
- I2C LCD: SCL -> PB6, SDA -> PB7, plus VCC and GND
- ST-Link (SWD): SWDIO -> PA13, SWCLK -> PA14, 3.3V, GND
- BOOT0 to GND for normal flash boot

## Files
- `main.c` – ADC setup, ADC read, threshold classification, message display
- `i2c_lcd.c` / `i2c_lcd.h` – register-level I2C driver for the LCD

## Tools
Embedded C (register-level), Keil uVision, STM32CubeProgrammer

## Limitation and Future Scope
The input is a potentiometer, not a real EEG signal, and messages use
fixed thresholds. 
Future work: integrate an EEG acquisition module,
filter the signal, and add a trained classifier.
