#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>

class Actuator {
    public:
        Actuator(uint8_t up_pin, uint8_t down_pin);
        void up();
        void down();
        void stop();

    private:
        uint8_t up_pin;
        uint8_t down_pin;
};

#endif