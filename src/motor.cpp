#include "motors.h"

Motor::Motor(uint8_t forward_pin, uint8_t backward_pin, uint8_t pwm_pin)
    : forward_pin(forward_pin),
      backward_pin(backward_pin),
      pwm_pin(pwm_pin) {
    pinMode(forward_pin, OUTPUT);
    pinMode(backward_pin, OUTPUT);
    pinMode(pwm_pin, OUTPUT);
}

void Motor::set(int speed) {
    digitalWrite(forward_pin, speed >= 0 ? HIGH : LOW);
    digitalWrite(backward_pin, speed <= 0 ? HIGH : LOW);
    analogWrite(pwm_pin, abs(speed));
}