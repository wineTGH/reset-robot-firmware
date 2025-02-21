#pragma once
#include <Arduino.h>
#include <Wire.h>

#define NAVX_REG_YAW_L 0x16

class IMU {
    public:
        IMU(int address = 0x32);
        float readYaw();

    private:
        int address_;
};