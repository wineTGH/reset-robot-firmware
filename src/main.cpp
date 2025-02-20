#include <Arduino.h>

#include "commands.h"
#include "imu.h"

struct Motor {
    uint8_t forward_pin;
    uint8_t backward_pin;
    uint8_t pwm_pin;

    uint8_t encoder_a_pin;
    uint8_t encoder_b_pin;

    const int CPR;  // Counts per revolution
    int encoder_count;
    float target_velocity;  // Target velocity in RPM
    float current_velocity; // Current velocity in RPM
    float integral;
    float previous_error;
};

struct MovementCommand {
    char action;
    int speed;
};

MovementCommand previousCommand = {STOP, 0};

Motor motors[] = {
    {13, 12, 11, 3, 17, 964, 0, 0.0, 0.0, 0.0, 0.0},
    {9, 8, 10, 2, 16, 360, 0, 0.0, 0.0, 0.0, 0.0},
    {6, 7, 5, 19, 15, 360, 0, 0.0, 0.0, 0.0, 0.0},
    {42, 44, 46, 18, 14, 360, 0, 0.0, 0.0, 0.0, 0.0}
};

IMU imu;

volatile long int encoder_counts[] = {0, 0, 0, 0};

const float Kp = 1.0;
const float Ki = 0.1;
const float Kd = 0.01;

const unsigned long interval = 10;
static unsigned long lastTime = 0;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 100;

void setupMotor(int index) {
    pinMode(motors[index].forward_pin, OUTPUT);
    pinMode(motors[index].backward_pin, OUTPUT);
    pinMode(motors[index].pwm_pin, OUTPUT);
}

void countPulse(int motor_index, int trigger_level = LOW);
void countFrontLeftPulse() { countPulse(0); }
void countFrontRightPulse() { countPulse(1); }
void countBackLeftPulse() { countPulse(2, HIGH); }
void countBackRightPulse() { countPulse(3); }

void setMotor(int i, int pwm) {
    digitalWrite(motors[i].forward_pin, pwm > 0 ? HIGH : LOW);
    digitalWrite(motors[i].backward_pin, pwm > 0 ? LOW : HIGH);
    analogWrite(motors[i].pwm_pin, pwm);
}

void forward(int speed) {
    for (size_t i = 0; i < 4; i++) {
        motors[i].target_velocity = speed;
    }
}

void backward(int speed) {
    for (size_t i = 0; i < 4; i++) {
        motors[i].target_velocity = -speed;
    }
}

void right(int speed) {
    motors[0].target_velocity = speed;
    motors[1].target_velocity = -speed;
    motors[2].target_velocity = -speed;
    motors[3].target_velocity = speed;
}

void left(int speed) {
    motors[0].target_velocity = -speed;
    motors[1].target_velocity = speed;
    motors[2].target_velocity = speed;
    motors[3].target_velocity = -speed;
}

void rotate(int speed) {
    motors[0].target_velocity = speed;
    motors[2].target_velocity = speed;

    motors[1].target_velocity = -speed;
    motors[3].target_velocity = -speed;
}

void stop() {
    for (size_t i = 0; i < 4; i++) {
        motors[i].target_velocity = 0;
    }
}

float targetYaw = 0;

void setup() {
    Serial.begin(9600);

    for (size_t i = 0; i < 4; i++) setupMotor(i);

    attachInterrupt(digitalPinToInterrupt(motors[0].encoder_a_pin), countFrontLeftPulse, RISING);
    attachInterrupt(digitalPinToInterrupt(motors[1].encoder_a_pin), countFrontRightPulse, RISING);
    attachInterrupt(digitalPinToInterrupt(motors[2].encoder_a_pin), countBackLeftPulse, RISING);
    attachInterrupt(digitalPinToInterrupt(motors[3].encoder_a_pin), countBackRightPulse, RISING);
}

void loop() {
    unsigned long currentTime = millis();

    if (Serial.available() > 0) {
        auto command = Serial.readStringUntil(';');
        if (command.startsWith("C")) {
            Serial.print("C: ");
            Serial.println(command);

            command = command.substring(1);
            int sep_indx = command.indexOf(':');
            String action = command.substring(sep_indx - 1, sep_indx);
            String arg = command.substring(sep_indx + 1);
            
            switch (action[0]) {
                case ROTATE_TO:
                    targetYaw = arg.toInt();
                    break;
                case MOVE_FORWARD:
                    previousCommand.action = MOVE_FORWARD;
                    previousCommand.speed = arg.toInt();
                    forward(arg.toInt());
                    break;
                case MOVE_BACKWARD:
                    previousCommand.action = MOVE_BACKWARD;
                    previousCommand.speed = arg.toInt();
                    backward(arg.toInt());
                    break;
                case MOVE_LEFT:
                    previousCommand.action = MOVE_LEFT;
                    previousCommand.speed = arg.toInt();
                    left(arg.toInt());
                    break;
                case MOVE_RIGHT:
                    previousCommand.action = MOVE_RIGHT;
                    previousCommand.speed = arg.toInt();
                    right(arg.toInt());
                    break;
                case STOP:
                    previousCommand.action = STOP;
                    previousCommand.speed = 0;
                    stop();
                default:
                    break;
            }
        }
    }


    if (currentTime - lastTime >= interval) {
        noInterrupts();
        for (size_t i = 0; i < 4; i++) {
            encoder_counts[i] = motors[i].encoder_count;
            motors[i].encoder_count = 0;
        }
        interrupts();

        for (size_t i = 0; i < 4; i++) {
            motors[i].current_velocity = (encoder_counts[i] * 60.0) / (motors[i].CPR * (interval / 1000.0));
            
            if (motors[i].target_velocity == 0) {
                setMotor(i, 0);
                continue;
            }

            float error = motors[i].target_velocity - motors[i].current_velocity;
            motors[i].integral += error * (interval / 1000.0);
            float derivative = (error - motors[i].previous_error) / (interval / 1000.0);
            motors[i].previous_error = error;

            float output = Kp * error + Ki * motors[i].integral + Kd * derivative;
            setMotor(i, constrain(output, -255, 255));
        }
        
        lastTime = currentTime;
    }

    float currentYaw = imu.readYaw();
    float error = targetYaw - currentYaw;

    while (error > 180) error -= 360;
    while (error < -180) error += 360;
    if (abs(error) > 3) {
        rotate(error > 0 ? 50 : -50);
    } else {
        // Reapply the previous movement command
        switch (previousCommand.action) {
            case STOP:
                stop();
            case MOVE_FORWARD:
                forward(previousCommand.speed);
                break;
            case MOVE_BACKWARD:
                backward(previousCommand.speed);
                break;
            case MOVE_LEFT:
                left(previousCommand.speed);
                break;
            case MOVE_RIGHT:
                right(previousCommand.speed);
                break;
            default:
                break;
        }
    }
}

void countPulse(int motor_index, int trigger_level) {
    bool pin_b_state = digitalRead(motors[motor_index].encoder_b_pin);
    motors[motor_index].encoder_count += (pin_b_state == trigger_level) ? 1 : -1;
}