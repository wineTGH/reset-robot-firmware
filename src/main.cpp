#include <Arduino.h>
#include "commands.h"
void setup() {
    Serial.begin(115200);
}

void loop() {
    if (Serial.available()) {
        auto command = Serial.readStringUntil(';');
        if (command.startsWith("C")) {
            command = command.substring(1);
            int sep_indx = command.indexOf(':');
            String action = command.substring(sep_indx - 1, sep_indx);
            String arg = command.substring(sep_indx + 1);
            
            switch (action[0]) {
                case ROTATE_TO:
                    Serial.print("Rotating to ");
                    Serial.println(arg);
                    break;
                case MOVE_FORWARD:
                    Serial.print("Moving forward ");
                    Serial.println(arg);
                    break;
                case MOVE_BACKWARD:
                    Serial.print("Moving backward ");
                    Serial.println(arg);
                    break;
                case MOVE_LEFT:
                    Serial.print("Moving left ");
                    Serial.println(arg);
                case MOVE_RIGHT:
                    Serial.print("Moving right ");
                    Serial.println(arg);
                default:
                    break;
            }
        }
    }
}