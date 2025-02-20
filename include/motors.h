#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include <Encoder.h>

class Motor {
    public:
        Motor(uint8_t forward_pin, uint8_t backward_pin, uint8_t pwm_pin, uint8_t encoder_a_pin, uint8_t encoder_b_pin, int CPR);
        ~Motor();
        void set(int speed);
        const int CPR;

        int target_velocity;
        int current_velocity;

        Encoder* encoder;
    private:
        uint8_t forward_pin;
        uint8_t backward_pin;
        uint8_t pwm_pin;
};

#endif