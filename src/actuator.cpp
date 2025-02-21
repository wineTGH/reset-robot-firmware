#include "actuator.h"

Actuator::Actuator(uint8_t up_pin, uint8_t down_pin): up_pin(up_pin), down_pin(down_pin) {
    pinMode(up_pin, OUTPUT);
    pinMode(down_pin, OUTPUT);
}

void Actuator::up() {
    digitalWrite(up_pin, HIGH);
    digitalWrite(down_pin, LOW);
    delay(6000);
}

void Actuator::down() {
    digitalWrite(up_pin, LOW);
    digitalWrite(down_pin, HIGH);
    delay(6000);
}