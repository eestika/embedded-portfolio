# Tiny Perceptron Node

Minimal Embedded AI project based on ESP32 WROVER-E.

The goal of this project is to understand the real foundations of embedded artificial intelligence starting from the simplest possible neural structure: the perceptron.

Instead of using complex frameworks or prebuilt AI models, this project focuses on:

- understanding the mathematics
- implementing inference directly on a microcontroller
- learning embedded AI step by step
- building a modular and explainable architecture

---

# Hardware

Current hardware:

- ESP32 WROVER-E Freenove
- Serial Monitor
- onboard LED / external LED

Future expansions:

- push buttons
- OLED SSD1306
- MPU6050
- VL53L1X
- servo motor
- analog sensors

---

# Project Goals

The project starts from the classic perceptron equation:

y = step(w1*x1 + w2*x2 + bias)

and progressively evolves toward real embedded AI experiments.

---

# Planned Roadmap

## Milestone 1
Fixed-weight AND perceptron

## Milestone 2
Fixed-weight OR perceptron

## Milestone 3
Simple training algorithm

## Milestone 4
Real GPIO/sensor inputs

## Milestone 5
LED/servo outputs

## Milestone 6
Training on Raspberry Pi / inference on ESP32

## Milestone 7
TinyML exploration

---

# Philosophy

This project is intentionally:

- small
- modular
- understandable
- hardware-oriented
- burnout-safe

The purpose is not to create "magic AI".

The purpose is to deeply understand how intelligent behavior can emerge from simple mathematical structures running on real embedded hardware.

---

# Repository Structure

```text
tiny_perceptron_node/
├── README.md
├── firmware/
└── docs/
```

Additional long-term study notes and experimental learning material are maintained inside a private Obsidian Digital Mind vault.

---

# Current Status

Milestone 1 completed locally:

- PlatformIO ESP32 project created
- Serial communication verified
- Fixed-weight AND perceptron implemented
- Inference validated through Serial Monitor

Current perceptron configuration:

- w1 = 1.0
- w2 = 1.0
- bias = -1.5

Next milestone:
Real hardware output control using LED.

## Current Firmware Experiments

### esp32_perceptron_and_serial

First fixed-weight AND perceptron running on ESP32.
Inference results validated through Serial Monitor.

### esp32_perceptron_and_led

AND perceptron controlling real hardware output.

Features:
- fixed-weight inference
- onboard LED control
- hardware abstraction layer
- active LOW LED handling
- serial debugging