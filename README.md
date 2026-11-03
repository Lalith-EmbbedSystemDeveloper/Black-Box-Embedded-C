# Car Black Box System

An embedded C project for a vehicle event-monitoring and logging system built around the **Microchip PIC16F877A** microcontroller. The project was configured in **MPLAB X IDE** and uses the **MPLAB XC8** C compiler.

> **Project note:** This repository documents and organizes the source code supplied with the project archive. Hardware behavior, wiring, calibration, and the exact operating procedure should be verified on the physical prototype before being treated as validated.

## Overview

The system presents a vehicle-style dashboard and records selected events with a time and speed value. It uses a real-time clock and external EEPROM so that event records can be retained independently of the main application flow.

## Implemented software features

Based on the supplied source code, the application includes:

- Dashboard display with time, event code, and speed value.
- ADC-based speed input conversion.
- Event logging for collision indication and gear changes.
- RTC timekeeping through the DS1307.
- Event storage through an I²C 24C02-compatible external EEPROM.
- Password-based access to a menu.
- Menu operations to view logs, clear logs, download logs, change the password, and set the time.
- UART output for downloading logged event records.

The collision event is represented in software by a keypad input. This repository does not establish that an actual accelerometer or crash sensor is connected.

## Hardware and interfaces

| Device / interface | Role in the supplied firmware |
|---|---|
| PIC16F877A | Main microcontroller |
| ADC peripheral | Reads the analogue input used as the speed value |
| Character LCD (CLCD) | Dashboard, prompts, menus, and log display |
| Digital keypad | User input and event/menu controls |
| DS1307 RTC | Timekeeping over I²C |
| 24C02 external EEPROM | Persistent event-log storage over I²C |
| UART | Serial log output |
| Timer 2 interrupt | Periodic countdown timing used by the UI |

The source defines a 20 MHz oscillator (`FOSC`) and initializes UART at 9600 baud and I²C at 100 kHz. Confirm these settings against the actual board and oscillator before building or testing.

See [`docs/HARDWARE_AND_INTERFACES.md`](docs/HARDWARE_AND_INTERFACES.md) for further notes.

## Repository structure

```text
Car-Black-Box/
├── adc.c / adc.h
├── car_black_box_def.c
├── car_black_box.h
├── clcd.c / clcd.h
├── digital_keypad.c / digital_keypad.h
├── ds1307.c / ds1307.h
├── EEprom.c / EEprom.h
├── i2c.c / i2c.h
├── isr.c
├── main.c / main.h
├── timers.c / timers.h
├── uart.c / uart.h
├── nbproject/
│   ├── configurations.xml
│   └── project.xml
├── docs/
│   ├── HARDWARE_AND_INTERFACES.md
│   ├── PROJECT_STRUCTURE.md
│   └── TESTING.md
├── .gitignore
└── README.md
```

Generated build output, distribution binaries, and machine-specific MPLAB X private settings are intentionally excluded. They can be regenerated locally by the IDE.

## Development environment

- IDE: Microchip MPLAB X IDE
- Compiler: MPLAB XC8 (project configuration specifies XC8 3.00)
- Target device: PIC16F877A
- Language: Embedded C (C99 configured)
- Oscillator setting in source: 20 MHz
- UART baud rate in source: 9600
- I²C baud rate in source: 100 kHz

### Open and build the project

1. Install MPLAB X IDE and a compatible MPLAB XC8 compiler.
2. Clone or download this repository.
3. In MPLAB X IDE, choose **File → Open Project**.
4. Select the project directory containing the MPLAB project metadata (`nbproject/project.xml`).
5. Confirm that PIC16F877A is selected as the target device.
6. Build the project and review compiler warnings/errors.
7. Connect the appropriate programmer/debugger and hardware only after verifying the circuit and device configuration.

The project archive did not include a portable, verified hardware setup guide. The exact pin mapping and circuit connections must be confirmed from the original circuit diagram or prototype.

## Runtime workflow (high-level)

1. Firmware initializes I²C, RTC, LCD, keypad, ADC, Timer 2, and UART.
2. The dashboard reads the analogue input and displays the current time, event code, and speed value.
3. Keypad actions update the gear/event state or trigger the collision event code.
4. Authorized menu access provides log viewing, clearing, serial download, password change, and time setting.
5. Event records are written to external EEPROM and can be sent over UART.

## Validation status and limitations

This is a source-code repository, not a claim of automotive-grade safety or a certified vehicle black-box recorder.

Before demonstrating the project, verify at least:

- Correct oscillator frequency and configuration bits.
- LCD and keypad wiring and operation.
- RTC initialization, time setting, and behavior after power loss.
- EEPROM address layout, write timing, and log retention after reset.
- ADC input range and the mapping from ADC reading to displayed speed.
- UART output using a serial terminal at the configured baud rate.
- Timer interrupt timing and menu timeout behavior.
- Collision input behavior on the actual hardware.

The supplied source code should be reviewed and tested before deployment. It is intended as an educational embedded-systems project, not a safety-critical automotive device.

## Documentation

- [Hardware and interfaces](docs/HARDWARE_AND_INTERFACES.md)
- [Project structure](docs/PROJECT_STRUCTURE.md)
- [Testing checklist](docs/TESTING.md)

## Author

**Lalith Singarapu** — author attribution appears in the supplied source files. Confirm the preferred public author name and add your contact/profile links here before publishing.

## License

No license was included in the supplied archive. Unless you add a license, others should not assume they have permission to reuse, modify, or redistribute this code.
