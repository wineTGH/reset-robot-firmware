#pragma once

#include <Arduino.h>

#define DIR_PIN 50
#define CLK_PIN 52
#define EN_PIN 48
#define STEP_DELAY 1000

void startStepper() {
    pinMode(DIR_PIN, OUTPUT);
    pinMode(CLK_PIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
    
    digitalWrite(DIR_PIN, HIGH);
    digitalWrite(EN_PIN, LOW);

}

void rotateStepper(int steps, bool reverse = false) {
    digitalWrite(DIR_PIN, reverse);

    for (size_t i = 0; i < steps; i++) {
        digitalWrite(CLK_PIN, HIGH);
        delayMicroseconds(STEP_DELAY);
        digitalWrite(CLK_PIN, LOW);
        delayMicroseconds(STEP_DELAY);
    }
    
}