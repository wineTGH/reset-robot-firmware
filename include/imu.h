#pragma once
#include <Arduino.h>
#include <Wire.h>

class IMU {
    public:
        IMU(int address = 0x32);
        float readYaw();

    private:
        int address_;
};