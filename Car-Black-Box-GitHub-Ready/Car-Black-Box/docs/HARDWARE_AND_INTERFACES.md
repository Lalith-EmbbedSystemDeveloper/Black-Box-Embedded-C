# Hardware and Interfaces

This document records the hardware and peripheral interfaces identifiable from the supplied firmware. It is not a substitute for a verified schematic.

## Main controller

- **PIC16F877A** is specified as the target device in `nbproject/configurations.xml`.
- The source defines `FOSC` as 20,000,000 Hz in `i2c.h` and `uart.h`.
- The firmware uses Microchip device-register definitions through `<xc.h>`.

## Peripheral roles

| Peripheral / module | Firmware files | Purpose |
|---|---|---|
| ADC | `adc.c`, `adc.h` | Reads an analogue value and scales it to a 0–99 display value |
| Character LCD | `clcd.c`, `clcd.h` | Dashboard and menu display |
| Digital keypad | `digital_keypad.c`, `digital_keypad.h` | Dashboard event and menu input |
| DS1307 RTC | `ds1307.c`, `ds1307.h` | Reads and writes time registers over I²C |
| 24C02 EEPROM | `EEprom.c`, `EEprom.h` | Stores event records over I²C |
| I²C | `i2c.c`, `i2c.h` | Shared serial bus for RTC and EEPROM |
| UART | `uart.c`, `uart.h` | Sends event logs to a serial terminal |
| Timer 2 / ISR | `timers.c`, `timers.h`, `isr.c` | Periodic interrupt and countdown support |

## Communication settings found in source

- I²C initialization is called with 100 kHz.
- UART initialization is called with 9600 baud.
- The source uses a 20 MHz oscillator constant for peripheral calculations.

## Verify before hardware use

The archive does not provide a verified pinout or schematic. Confirm PIC pin assignments, LCD wiring, keypad wiring, I²C pull-ups, UART level compatibility, power supply, oscillator circuit, and EEPROM/RTC connections from the original hardware design. Do not connect a vehicle electrical system directly to microcontroller pins.
