#include <Arduino.h>

/*
    Tiny Perceptron Node
    Milestone 5: GPIO button inputs + AND perceptron LED output
*/

// --------------------------------------------------
// HARDWARE CONFIGURATION
// --------------------------------------------------

const int LED_PIN = 2;

const int BUTTON1_PIN = 32;
const int BUTTON2_PIN = 33;

// On this ESP32 board the onboard LED is active LOW
const bool LED_ACTIVE_LOW = true;

// --------------------------------------------------
// LED CONTROL
// --------------------------------------------------

void setLed(bool ledOn)
{
    if (LED_ACTIVE_LOW)
    {
        digitalWrite(LED_PIN, ledOn ? LOW : HIGH);
    }
    else
    {
        digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    }
}

// --------------------------------------------------
// STEP FUNCTION
// --------------------------------------------------

int stepFunction(float value)
{
    return (value >= 0.0f) ? 1 : 0;
}

// --------------------------------------------------
// AND PERCEPTRON
// --------------------------------------------------

int perceptronAND(int x1, int x2)
{
    const float w1 = 1.0f;
    const float w2 = 1.0f;
    const float bias = -1.5f;

    float sum = (x1 * w1) + (x2 * w2) + bias;

    int output = stepFunction(sum);

    Serial.print("x1 = ");
    Serial.print(x1);

    Serial.print(" | x2 = ");
    Serial.print(x2);

    Serial.print(" | sum = ");
    Serial.print(sum);

    Serial.print(" | output = ");
    Serial.print(output);

    Serial.print(" | LED = ");
    Serial.println(output == 1 ? "ON" : "OFF");

    return output;
}

// --------------------------------------------------
// BUTTON READING
// --------------------------------------------------

int readButtonAsInput(int pin)
{
    /*
        Buttons are wired from GPIO to GND.

        With INPUT_PULLUP:
        - not pressed = HIGH
        - pressed     = LOW

        For the perceptron:
        - not pressed = 0
        - pressed     = 1
    */

    return (digitalRead(pin) == LOW) ? 1 : 0;
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_PIN, OUTPUT);
    setLed(false);

    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" Tiny Perceptron Node");
    Serial.println(" Button Inputs + AND Perceptron");
    Serial.println("=================================");
    Serial.println();

    Serial.println("Press both buttons to turn LED ON.");
    Serial.println();
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop()
{
    int x1 = readButtonAsInput(BUTTON1_PIN);
    int x2 = readButtonAsInput(BUTTON2_PIN);

    int output = perceptronAND(x1, x2);

    setLed(output == 1);

    delay(200);
}