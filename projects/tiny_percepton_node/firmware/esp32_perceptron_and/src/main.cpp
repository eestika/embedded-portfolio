#include <Arduino.h>

/*
    Tiny Perceptron Node
    First AND perceptron on ESP32
*/

// --------------------------------------------------
// STEP FUNCTION
// --------------------------------------------------

int stepFunction(float value)
{
    if (value >= 0)
    {
        return 1;
    }

    return 0;
}

// --------------------------------------------------
// PERCEPTRON
// --------------------------------------------------

int perceptronAND(int x1, int x2)
{
    // Fixed weights
    float w1 = 1.0;
    float w2 = 1.0;

    // Bias
    float bias = -1.5;

    // Weighted sum
    float sum = (x1 * w1) + (x2 * w2) + bias;

    // Activation
    int output = stepFunction(sum);

    // Debug output
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
// SETUP
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("=================================");
    Serial.println(" Tiny Perceptron Node ");
    Serial.println(" AND Perceptron Test ");
    Serial.println("=================================");

    Serial.println();

    // Test all AND combinations
    perceptronAND(0, 0);
    perceptronAND(0, 1);
    perceptronAND(1, 0);
    perceptronAND(1, 1);
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop()
{
    // Nothing here for now
}