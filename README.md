# Embedded Portfolio

Portfolio of embedded systems projects by Daniele Perez (Tallinn, Estonia).

Focus areas:
- Embedded C / C++
- STM32 and ESP32 firmware
- Peripheral integration
- UART, I2C, SPI, RS485, CAN
- Modular firmware architecture
- Hardware / software debugging

---

# Main Projects

## Rover (personal robotics project)

A personal robotics and firmware architecture project based on STM32.

Main goals:
- Modular firmware structure
- Peripheral testing and validation
- UART communication
- Motor driver integration
- RS485 and CAN experiments
- Future sensor and display integration
- Incremental hardware bring-up

Project page:
- `projects/rover_h533/`

---

## Sample Routing Module

A modular embedded routing and communication test platform designed to simulate a master-slave architecture.

Current hardware:
- Raspberry Pi 4 as Linux master/controller
- STM32 NUCLEO-F207 as communication node
- MikroE UNI-2003 expansion board
- Waveshare USB to RS485 interface
- LCD1602 display
- AT24C128 EEPROM

Planned communication buses:
- UART
- RS485
- I2C
- SPI

Project goals:
- Create a realistic embedded communication topology
- Practice protocol design and packet routing
- Test EEPROM storage and LCD diagnostics
- Build reusable firmware modules
- Develop a portfolio-ready industrial-style demo

Project page:
- `projects/sample-routing-module/`

---

# Skills & Tools

## Languages
- C
- Embedded C
- Basic C++
- Python (basic scripting/tools)

## Platforms & MCUs
- STM32
- ESP32
- Raspberry Pi
- PIC32

## Protocols
- UART
- I2C
- SPI
- PWM
- ADC
- GPIO
- RS485
- CAN

## Tools
- STM32CubeIDE
- STM32CubeMX
- MPLAB X
- PlatformIO
- VS Code
- Git / GitHub
- Logic Analyzer
- Tera Term
- PulseView

---

# Documentation Style

Repositories typically include:
- Wiring notes
- Hardware references
- Datasheets
- Test logs
- README files
- Incremental firmware versions

---

# Notes

Some full or client-related versions remain private.

Private repositories may include:
- NDA-protected work
- Client firmware
- Advanced prototypes
- Experimental integrations

Public repositories are intended to demonstrate:
- Firmware structure
- Code quality
- Hardware integration
- Debugging process
- Documentation quality

