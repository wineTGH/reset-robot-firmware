#include "motors.h"

Motor::Motor(uint8_t forward_pin, uint8_t backward_pin, uint8_t pwm_pin,
             uint8_t encoder_a_pin, uint8_t encoder_b_pin, int CPR)
    : forward_pin(forward_pin),
      backward_pin(backward_pin),
      pwm_pin(pwm_pin),
      CPR(CPR) {
    pinMode(forward_pin, OUTPUT);
    pinMode(backward_pin, OUTPUT);

    encoder = new Encoder(encoder_a_pin, encoder_b_pin);
}

void Motor::set(int speed) {
    digitalWrite(forward_pin, speed >= 0 ? HIGH : LOW);
    digitalWrite(backward_pin, speed <= 0 ? HIGH : LOW);
    analogWrite(pwm_pin, speed);
}

Motor::~Motor() { delete encoder; }