# Tiny Perceptron Node - Milestones

---

# Milestone 1
## Fixed-weight AND perceptron inference

Status: Completed

Description:

Implemented the first mathematical perceptron running on ESP32 WROVER-E.

The firmware performs:

- weighted sum calculation
- step activation function
- binary classification
- serial debug output

Implemented equation:

y = step(w1*x1 + w2*x2 + bias)

Current parameters:

- w1 = 1.0
- w2 = 1.0
- bias = -1.5

Validated truth table:

| x1 | x2 | output |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

Key concepts learned:

- inference vs training
- weighted decision systems
- activation functions
- mathematical classification
- embedded AI foundations

Firmware:

```text
firmware/esp32_perceptron_and_serial/
```

---

# Milestone 2
## Perceptron controlling real hardware output

Status: Completed

Description:

Extended the perceptron to control real hardware through the onboard ESP32 LED.

The perceptron output is now connected to a physical output device.

Implemented features:

- LED hardware control
- active LOW abstraction
- hardware/software integration
- embedded output handling
- serial runtime debugging

Hardware behavior:

| perceptron output | LED state |
|---|---|
| 0 | OFF |
| 1 | ON |

Key concepts learned:

- embedded hardware abstraction
- active LOW logic
- GPIO output control
- AI-driven physical interaction
- hardware debugging

Firmware:

```text
firmware/esp32_perceptron_and_led/
```

# Milestone 3
## Fixed-weight OR perceptron with LED output

Status: Completed

Description:

Implemented an OR perceptron running on ESP32 WROVER-E.

Current parameters:

- w1 = 1.0
- w2 = 1.0
- bias = -0.5

Validated truth table:

| x1 | x2 | output | LED |
|---|---|---|---|
| 0 | 0 | 0 | OFF |
| 0 | 1 | 1 | ON |
| 1 | 0 | 1 | ON |
| 1 | 1 | 1 | ON |

Key concepts learned:

- OR logic with perceptron
- bias as decision threshold
- same weights, different bias, different behavior
- embedded inference controlling hardware

Firmware:

```text
firmware/esp32_perceptron_or_led/
```
---

# Milestone 4
## Real GPIO button inputs with AND perceptron

Status: Completed

Description:

Implemented physical GPIO button inputs connected to the ESP32 perceptron.

The perceptron now receives real hardware inputs instead of simulated software test cases.

Hardware configuration:

- GPIO32 → Button 1
- GPIO33 → Button 2
- onboard LED output

Implemented concepts:

- GPIO digital input
- INPUT_PULLUP configuration
- physical hardware interaction
- real-time inference
- interactive embedded AI behavior

Validated behavior:

| Button 1 | Button 2 | Output | LED |
|---|---|---|---|
| 0 | 0 | 0 | OFF |
| 0 | 1 | 0 | OFF |
| 1 | 0 | 0 | OFF |
| 1 | 1 | 1 | ON |

Key concepts learned:

- GPIO input handling
- embedded hardware interaction
- pull-up resistors
- physical input classification
- cyber-physical embedded systems

Firmware:

```text
firmware/esp32_perceptron_buttons_led/
```
---

# Next Planned Milestones

## Milestone 5
GPIO button inputs

## Milestone 6
Sensor integration

## Milestone 7
Servo motor output

## Milestone 8
Training on Raspberry Pi

## Milestone 9
TinyML exploration