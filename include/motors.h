#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>

class Motor {
    public:
        Motor(uint8_t forward_pin, uint8_t backward_pin, uint8_t pwm_pin, uint8_t encoder_a_pin, uint8_t encoder_b_pin, int CPR);
        void set(int speed);
        int CPR;
    private:
        uint8_t forward_pin;
        uint8_t backward_pin;
        uint8_t pwm_pin;
        
        uint8_t encoder_a_pin;
        uint8_t encoder_b_pin;
};

#endif