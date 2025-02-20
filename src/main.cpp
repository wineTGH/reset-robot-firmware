#include <Arduino.h>

#include "commands.h"
#include "imu.h"
#include "motors.h"

Motor motor_left_front(13, 12, 11, 3, 17, 3856);
Motor motor_right_front(9, 8, 10, 2, 16, 1440);
Motor motor_left_back(6, 7, 5, 19, 15, 1440);
Motor motor_right_back(42, 44, 46, 18, 14, 1440);

Motor motors[] = {
    motor_left_front, 
    motor_right_front, 
    motor_left_back, 
    motor_right_back
};

IMU imu;
float targetYaw = 0;

struct Command {
    char action;
    int arg;
};

Command command = {STOP, 0};


void forward(int speed) {
    for (size_t i = 0; i < 4; i++) {
        motors[i].set(speed);
    }
}

void backward(int speed) {
    for (size_t i = 0; i < 4; i++) {
        motors[i].set(-speed);
    }
}

void right(int speed) {
    motors[0].set(speed);
    motors[1].set(-speed);
    motors[2].set(-speed);
    motors[3].set(speed);
}

void left(int speed) {
    motors[0].set(-speed);
    motors[1].set(speed);
    motors[2].set(speed);
    motors[3].set(-speed);
}

void rotate(int speed) {
    motors[0].set(speed);
    motors[2].set(speed);

    motors[1].set(-speed);
    motors[3].set(-speed);
}

void stop() {
    for (size_t i = 0; i < 4; i++) {
        motors[i].set(0);
    }
}


void applyCommand(Command command) {
    switch (command.action) {
        case ROTATE_TO:
            targetYaw = command.arg;
            break;
        case MOVE_FORWARD:
            forward(command.arg);
            break;
        case MOVE_BACKWARD:
            backward(command.arg);
            break;
        case MOVE_LEFT:
            left(command.arg);
            break;
        case MOVE_RIGHT:
            right(command.arg);
            break;
        case STOP:
            stop();
        default:
            break;
    }
}

void setup() {
    Serial.begin(9600);

    // Ждём пока IMU датчик не отколибруется
    delay(25000);
}


void loop() {
    if (Serial.available() > 0) {
        String received_message = Serial.readStringUntil(';');
        if (received_message.startsWith("C")) {
            String raw_command = received_message.substring(1);
            int sep_indx = raw_command.indexOf(':');
            String action = raw_command.substring(sep_indx - 1, sep_indx);
            String arg = raw_command.substring(sep_indx + 1);

            command = {action[0], (int)arg.toInt()};
            applyCommand(command);
        }
    }

    float currentYaw = imu.readYaw();
    float error = targetYaw - currentYaw;

    while (error > 180) error -= 360;
    while (error < -180) error += 360;

    if (abs(error) > 3) {
        rotate(error > 0 ? 50 : -50);
    } else {
        applyCommand(command);
    }
}