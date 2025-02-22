#include "imu.h"

static inline int16_t decodeProtocolInt16( char *int16_bytes ) {
    return *((int16_t *)int16_bytes);
}

static inline float decodeProtocolSignedHundredthsFloat( char *uint8_signed_angle_bytes ) {
    float signed_angle = (float)decodeProtocolInt16(uint8_signed_angle_bytes);
    signed_angle /= 100;
    return signed_angle;
}

IMU::IMU(int address) {
    this->address_ = address;
    Wire.begin();
}

float IMU::readYaw() {
    Wire.beginTransmission(this->address_);
    Wire.write(NAVX_REG_YAW_L);
    Wire.write(2);
    Wire.endTransmission();

    Wire.beginTransmission(this->address_);
    Wire.requestFrom(this->address_, 2);
    byte data[2] = {0, 0};
    uint8_t i = 0;
    delay(1);

    while (Wire.available()) {
        data[i++] = Wire.read();
    }

    Wire.endTransmission();

    return decodeProtocolSignedHundredthsFloat((char *)&data[0]);
}

void IMU::resetYaw() {
    Wire.beginTransmission(this->address_);
    Wire.write(0x80 | 0x56);
    Wire.write(0x80);
    Wire.endTransmission();
}