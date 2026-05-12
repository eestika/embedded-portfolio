#include <Arduino.h>

/*
    Tiny Perceptron Node
    Milestone 2: AND perceptron controlling LED
*/

// --------------------------------------------------
// HARDWARE CONFIGURATION
// --------------------------------------------------

const int LED_PIN = 2;

// Set to true if the onboard LED is active LOW
const bool LED_ACTIVE_LOW = true;

// Function prototype
void setLed(bool ledOn);

// --------------------------------------------------
// STEP FUNCTION
// --------------------------------------------------

int stepFunction(float value)
{
    return (value >= 0.0f) ? 1 : 0;
}

// --------------------------------------------------
// PERCEPTRON
// --------------------------------------------------

int perceptronAND(int x1, int x2)
{
    const float w1 = 1.0f;
    const float w2 = 1.0f;
    const float bias = -1.5f;

    float sum = (x1 * w1) + (x2 * w2) + bias;

    int output = stepFunction(sum);

    // Serial debug
    Serial.print("x1 = ");
    Serial.print(x1);

    Serial.print(" | x2 = ");
    Serial.print(x2);

    Serial.print(" | sum = ");
    Serial.print(sum);

    Serial.print(" | output = ");
    Serial.println(output);

    return output;
}

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
// TEST CASE
// --------------------------------------------------

void runTestCase(int x1, int x2)
{
    int output = perceptronAND(x1, x2);

    // Control LED
    setLed(output == 1);

    Serial.print("LED = ");

    if (output == 1)
    {
        Serial.println("ON");
    }
    else
    {
        Serial.println("OFF");
    }

    Serial.println("------------------------");

    delay(2000);
}

// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);

    delay(1000);

    pinMode(LED_PIN, OUTPUT);

    // Start with LED OFF
    setLed(false);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" Tiny Perceptron Node");
    Serial.println(" AND Perceptron LED Test");
    Serial.println("=================================");
    Serial.println();
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop()
{
    runTestCase(0, 0);
    runTestCase(0, 1);
    runTestCase(1, 0);
    runTestCase(1, 1);
}