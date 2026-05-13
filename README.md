# Embedded Portfolio

Portfolio of embedded systems projects by Daniele Perez (Tallinn, Estonia).

## Focus Areas
- Embedded C / C++
- STM32 and ESP32 firmware
- Peripheral integration
- UART, I2C, SPI, RS485, CAN
- Modular firmware architecture
- Hardware / software debugging

---

# Main Projects

## Sample Routing Module (SRM) — Release 1.0 Completed / Ongoing Development

A modular embedded routing and communication platform designed to simulate a **real-world master-slave architecture**, inspired by industrial systems.

### Architecture
- Raspberry Pi 4 → Master (Linux-based controller)
- STM32 NUCLEO-F207 → Slave communication node
- RS485 communication bus
- LCD1602 for system feedback
- EEPROM for data storage

### Implemented Features (v1.0)
- Custom binary protocol (frame-based communication)
- Master ↔ Slave communication over RS485
- Parser + dispatcher architecture
- CRC validation
- State-machine driven logic
- LCD diagnostic output
- Modular firmware structure

### Purpose
- Simulate industrial embedded communication systems
- Practice structured firmware design
- Develop reusable embedded modules
- Build a portfolio-ready project aligned with real applications

### Next Steps (v2.0)
The project is currently evolving toward a larger smart embedded automation platform with additional hardware modules, actuator control and future AI-assisted interaction experiments.
- Multi-slave architecture
- Additional actuator integration (stepper motors, sensors)
- Extended command set
- Improved system scalability

**Project page:**

projects/sample-routing-module/

---

## Tiny Perceptron Node — Embedded AI Foundations Project

A minimal embedded AI project focused on understanding the real foundations of neural computation directly on microcontrollers.

The project starts from the simplest possible artificial neuron: the perceptron.

### Current Hardware
- ESP32 WROVER-E Freenove
- PlatformIO
- Serial Monitor
- onboard LED

### Current Features
- Embedded inference pipeline
- Fixed-weight AND perceptron
- Mathematical inference on ESP32
- Step activation function
- Real-time serial debugging
- LED hardware control through inference output
- Active LOW hardware abstraction

### Purpose
- Understand embedded AI fundamentals
- Learn inference vs training
- Connect mathematical models to physical hardware
- Build a modular TinyML-oriented learning platform

### Planned Future Development
- OR perceptron
- Simple training algorithms
- GPIO and sensor inputs
- Servo motor output
- Sensor fusion experiments
- Raspberry Pi assisted training
- TinyML exploration

**Project page:**

projects/tiny_perceptron_node/


## Rover Platform — Robotics Hardware Lab

A personal embedded robotics platform used as a **hardware testing and prototyping environment**.
The platform remains important as a long-term robotics and embedded experimentation environment and may be redesigned in future iterations with improved mechanical and electronic architecture.

### Purpose
- Low-level hardware validation
- Peripheral testing (UART, PWM, GPIO)
- Motor driver experimentation
- Debugging with Logic Analyzer
- Rapid prototyping of embedded components


### Status
Currently suspended while active development focuses on SRM and Tiny Perceptron Node.

**Project page:**

projects/rover_h533/


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

Public repositories are intended to demonstrate:
- Firmware structure
- Code quality
- Hardware integration
- Debugging process
- Documentation quality

