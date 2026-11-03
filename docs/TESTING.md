# Bench Testing Checklist

Record the actual result, test date, hardware revision, and any observed issues when running these tests. Do not mark a test as passed until it has been performed on the hardware.

| Test | Expected observation | Result |
|---|---|---|
| Build in MPLAB X with XC8 | Project compiles without errors | Not yet verified |
| LCD startup | Dashboard or startup screen appears | Not yet verified |
| Keypad input | Keys are detected and mapped correctly | Not yet verified |
| ADC input | Displayed speed responds to analogue input | Not yet verified |
| RTC read | Time is displayed and increments | Not yet verified |
| RTC set | User-selected time is stored and displayed | Not yet verified |
| Event logging | Key-triggered events are recorded | Not yet verified |
| EEPROM retention | Stored records remain after reset/power cycle | Not yet verified |
| View/clear logs | Menu operations behave as expected | Not yet verified |
| UART download | Log text appears in serial terminal at 9600 baud | Not yet verified |
| Password workflow | Login and password-change flows work as intended | Not yet verified |

## Suggested evidence for the portfolio

Add genuine photographs or screenshots to an `Images/` folder, such as:

- Assembled prototype (top view).
- Controller and peripheral connections.
- Dashboard LCD displaying time and speed.
- Serial terminal showing downloaded event logs.
- MPLAB X project build output.

Only include photographs and test results from work actually performed.
