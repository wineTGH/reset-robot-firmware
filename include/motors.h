#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>

class Motor {
    public:
        Motor(uint8_t forward_pin, uint8_t backward_pin, uint8_t pwm_pin);
        void set(int speed);

        int target_velocity;
        int current_velocity;
    private:
        uint8_t forward_pin;
        uint8_t backward_pin;
        uint8_t pwm_pin;
};

#endif