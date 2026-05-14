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
Real GPIO button inputs

## Milestone 4
Simple training algorithm

## Milestone 5
Real sensor inputs

## Milestone 6
LED / servo outputs

## Milestone 7
Training on Raspberry Pi / inference on ESP32

## Milestone 8
TinyML exploration

---

# Philosophy

This project is intentionally:

- small
- modular
- understandable
- hardware-oriented
- incremental

The purpose is not to create "magic AI".

The purpose is to deeply understand how intelligent behavior can emerge from simple mathematical structures running on real embedded hardware.

The project follows a step-by-step engineering approach:

- understand the theory
- validate the mathematics
- implement inference on MCU
- connect AI logic to physical hardware
- progressively evolve toward embedded AI systems

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

Milestone 1 completed:

- Fixed-weight AND perceptron
- Serial inference validation

Milestone 2 completed:

- Fixed-weight OR perceptron
- LED hardware output
- Bias threshold experimentation

Milestone 3 completed:

- Real GPIO button inputs
- Physical interactive inference
- INPUT_PULLUP button handling
- Real-time hardware classification

Current capabilities:

- Embedded inference on ESP32
- GPIO input handling
- LED hardware control
- Serial debugging
- Configurable perceptron logic

Current perceptron experiments:

- AND logic
- OR logic
- Physical button interaction

Milestone 4 completed:

- Real GPIO button training
- Perceptron learning rule implemented
- Weights and bias updated automatically
- Error-based correction validated through Serial Monitor

Next milestone:

- Automatic training over full truth table
- Weight persistence using ESP32 NVS

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

### esp32_perceptron_or_led

Fixed-weight OR perceptron controlling real hardware output.

Features:
- OR inference with fixed weights
- bias adjusted for OR logic
- onboard LED control
- active LOW LED abstraction
- serial debugging

### esp32_perceptron_buttons_led

AND perceptron using real GPIO button inputs.

Features:
- physical button inputs
- INPUT_PULLUP configuration
- real-time inference
- interactive hardware AI behavior
- GPIO-to-inference pipeline

### esp32_perceptron_training_buttons

Perceptron training using real GPIO button inputs.

Features:
- physical button inputs
- AND target logic
- perceptron learning rule
- automatic weight update
- bias update
- real-time training debug over Serial Monitor