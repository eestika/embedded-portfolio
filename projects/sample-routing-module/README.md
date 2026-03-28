# Sample Routing Module

A portfolio embedded project designed to simulate an industrial / medical-style communication and control node.

The project is built to practice and demonstrate:

- Embedded C
- modular firmware architecture
- UART and RS485 communication
- I2C peripherals
- EEPROM usage
- LCD diagnostics
- master / slave communication
- protocol design
- state machines
- struct / union usage
- pointer-based data handling and parsing

---

# Project Goal

The goal of this project is to create a realistic embedded demo in which:

- a Raspberry Pi 4 acts as a Linux master/controller
- an STM32 NUCLEO-F207 acts as an embedded communication and control node
- external peripherals are connected to the STM32
- communication is performed over UART / RS485
- local diagnostics are shown on LCD
- configuration or status data can be stored in EEPROM

This project is intended both as:

- a learning platform
- a portfolio project for embedded job interviews

---

# Planned Architecture

## Master Side
- Raspberry Pi 4
- C application running on Linux
- USB to RS485 Waveshare interface
- command transmission
- status reception
- protocol testing and diagnostics

## Embedded Node
- STM32 NUCLEO-F207
- UART / RS485 communication
- I2C peripheral integration
- LCD1602 display
- AT24C128 EEPROM
- future actuator and sensor expansion

---

# Hardware

## Main Boards
- Raspberry Pi 4 Model B
- STM32 NUCLEO-F207ZG

## Communication
- Waveshare USB TO RS485/422
- MikroE RS485-related documentation and modules collected in docs

## Peripherals
- LCD1602 I2C display
- AT24C128 / AT24C256 EEPROM
- ULN2003 driver documentation collected for future actuator tests
- future servo / actuator integration planned

---

# Current Focus

The current technical focus of this project is:

- define clean project architecture
- organize documentation and datasheets
- build communication-first design
- practice concepts useful for embedded interviews:
  - struct
  - union
  - pointer arithmetic
  - casting
  - frame parsing
  - RS232 / UART fundamentals
  - RS485 communication
  - state machine design
  - EEPROM read/write
  - LCD diagnostics

---

# Repository Structure

```text
sample-routing-module/
├── docs/
│   ├── LCD1602/
│   ├── Mikroe/
│   ├── NUCLEO F207/
│   ├── Raspberry 4 model B/
│   ├── Uln2003/
│   └── Waveshire/
├── firmware/
├── hardware/
├── images/
└── README.md

---

# Planned firmware organization

A possible future structure is:

firmware/
├── raspberry_master/
│   ├── src/
│   ├── include/
│   └── README.md
│
└── stm32_node/
    ├── Core/
    ├── Drivers/
    ├── App/
    ├── Protocol/
    ├── Peripherals/
    └── README.md
	
	Communication Plan

The project is intended to evolve around a simple packet-based protocol.

Typical fields under evaluation:

header
command
payload length
payload bytes
checksum / CRC

This protocol will be useful to practice:

raw byte handling
buffer parsing
struct / union overlay
endian awareness
master / slave state transitions
Peripheral Plan
LCD1602

Used for:

local status display
error display
communication diagnostics
mode indication
EEPROM

Used for:

configuration persistence
address/config storage
parameter save/load
future fault/event logging
RS485

Used for:

robust serial communication
master / node testing
protocol framing and validation
Why This Project Matters

This project is intentionally designed to train skills that are highly relevant in embedded and industrial environments:

hardware/software integration
debugging on real targets
reading datasheets
designing simple but solid protocols
building modular embedded software
working with Linux + MCU mixed systems

It is also directly aligned with preparation for junior embedded technical interviews.

Status
Current Status
project created
documentation folder prepared
datasheets collected
portfolio repository published
architecture under definition
Next Steps
define exact system topology
define firmware folder structure
define first protocol draft
implement Raspberry master in C
implement STM32 communication node
test UART first
move to RS485
add LCD and EEPROM support
Notes

This project is under active development.

Some design choices may evolve as hardware and firmware are tested on real boards.

The emphasis is on:

learning correctly
documenting clearly
building reusable embedded knowledge
producing a project that is technically credible and portfolio-ready