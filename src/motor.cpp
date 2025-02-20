#include "motors.h"

Motor::Motor(uint8_t forward_pin, uint8_t backward_pin, uint8_t pwm_pin, uint8_t encoder_a_pin, uint8_t encoder_b_pin, int CPR) {
    this->forward_pin = forward_pin;
    this->backward_pin = backward_pin;

    this->encoder_a_pin = encoder_a_pin;
    this->encoder_b_pin = encoder_b_pin;

    this->CPR = CPR;
}

void Motor::set(int speed) {
    digitalWrite(forward_pin, speed >= 0 ? HIGH : LOW);
    digitalWrite(backward_pin, speed <= 0 ? HIGH : LOW);
    analogWrite(pwm_pin, speed);
}