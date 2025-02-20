#pragma once
#include <Arduino.h>
#include <Wire.h>

#define NAVX_REG_YAW_L 0x16

static inline int16_t decodeProtocolInt16( char *int16_bytes );
static inline float decodeProtocolSignedHundredthsFloat( char *uint8_signed_angle_bytes );

class IMU {
    public:
        IMU(int address = 0x32);
        float readYaw();

    private:
        int address_;
};