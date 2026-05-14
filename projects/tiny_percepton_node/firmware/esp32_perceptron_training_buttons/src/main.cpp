#include <Arduino.h>

/*
    Tiny Perceptron Node
    Milestone: real GPIO button training
    Goal: train an AND perceptron using physical button inputs
*/

// --------------------------------------------------
// HARDWARE CONFIGURATION
// --------------------------------------------------

const int LED_PIN = 2;
const int BUTTON1_PIN = 32;
const int BUTTON2_PIN = 33;

const bool LED_ACTIVE_LOW = true;

// --------------------------------------------------
// PERCEPTRON PARAMETERS
// --------------------------------------------------

float w1 = 0.0f;
float w2 = 0.0f;
float bias = 0.0f;

const float LEARNING_RATE = 0.1f;

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
// BUTTON READING
// --------------------------------------------------

int readButtonAsInput(int pin)
{
    // INPUT_PULLUP:
    // released = HIGH -> 0
    // pressed  = LOW  -> 1

    return (digitalRead(pin) == LOW) ? 1 : 0;
}

// --------------------------------------------------
// PERCEPTRON INFERENCE
// --------------------------------------------------

int perceptronPredict(int x1, int x2)
{
    float sum = (x1 * w1) + (x2 * w2) + bias;
    return stepFunction(sum);
}

// --------------------------------------------------
// TRAINING STEP
// --------------------------------------------------

void trainStep(int x1, int x2, int desired)
{
    float sum = (x1 * w1) + (x2 * w2) + bias;
    int output = stepFunction(sum);

    int error = desired - output;

    // Perceptron learning rule
    w1 = w1 + (LEARNING_RATE * error * x1);
    w2 = w2 + (LEARNING_RATE * error * x2);
    bias = bias + (LEARNING_RATE * error);

    setLed(output == 1);

    Serial.print("x1=");
    Serial.print(x1);

    Serial.print(" x2=");
    Serial.print(x2);

    Serial.print(" desired=");
    Serial.print(desired);

    Serial.print(" output=");
    Serial.print(output);

    Serial.print(" error=");
    Serial.print(error);

    Serial.print(" | w1=");
    Serial.print(w1);

    Serial.print(" w2=");
    Serial.print(w2);

    Serial.print(" bias=");
    Serial.println(bias);
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
    Serial.println(" GPIO Button Training - AND");
    Serial.println("=================================");
    Serial.println();

    Serial.println("Press different button combinations to train the perceptron.");
    Serial.println("Target logic: AND");
    Serial.println();
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------

void loop()
{
    int x1 = readButtonAsInput(BUTTON1_PIN);
    int x2 = readButtonAsInput(BUTTON2_PIN);

    // Teacher signal for AND logic
    int desired = (x1 == 1 && x2 == 1) ? 1 : 0;

    trainStep(x1, x2, desired);

    delay(500);
}