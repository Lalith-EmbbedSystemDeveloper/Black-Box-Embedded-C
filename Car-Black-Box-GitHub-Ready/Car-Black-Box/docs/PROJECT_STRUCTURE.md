# Project Structure

The repository keeps the original C source and header files at the project root to minimize disruption to the MPLAB X project configuration.

## Application files

- `main.c`, `main.h` — initialization, main loop, and application state transitions.
- `car_black_box_def.c`, `car_black_box.h` — dashboard, login, menu, event logging, and log operations.

## Peripheral drivers

- `adc.c`, `adc.h` — ADC initialization and conversion.
- `clcd.c`, `clcd.h` — character LCD driver.
- `digital_keypad.c`, `digital_keypad.h` — keypad driver.
- `ds1307.c`, `ds1307.h` — RTC driver.
- `EEprom.c`, `EEprom.h` — external EEPROM driver.
- `i2c.c`, `i2c.h` — I²C routines.
- `uart.c`, `uart.h` — UART routines.
- `timers.c`, `timers.h`, `isr.c` — timer setup and interrupt service routine.

## MPLAB X metadata

The `nbproject` directory contains project configuration metadata needed by MPLAB X. Machine-specific private settings and generated build outputs are excluded from version control.
