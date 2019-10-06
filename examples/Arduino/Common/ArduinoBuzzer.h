#ifndef _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_BUZZER_H_
#define _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_BUZZER_H_
#pragma once

#include <Abstract/AbstractBuzzer.h>

class ArduinoBuzzer : public AbstractBuzzer {

    const uint8_t mcuPin;

    void setState(Switched const newState) override {
        if (AbstractBuzzer::state != newState) {

            AbstractBuzzer::state = newState;

            if (newState == Switched::On) {
#ifdef __ATO_MODE_TESTING_PLATFORMIO__
//                Serial << "\tArduinoBuzzer::setState(Switched::On)\n";
#endif
                digitalWrite(mcuPin, HIGH); /* warn: Arduino specific */
            } else {
#ifdef __ATO_MODE_TESTING_PLATFORMIO__
//                Serial << "\tArduinoBuzzer::setState(Switched::Off)\n";
#endif
                digitalWrite(mcuPin, LOW); /* warn: Arduino specific */
            }
        }
    }

public:

    ArduinoBuzzer(uint8_t const mcuPin) : mcuPin(mcuPin) {}

    void setup() override {
#ifdef __ATO_MODE_TESTING_PLATFORMIO__
//        Serial << "ArduinoBuzzer::setup()\n";
#endif
        pinMode(mcuPin, OUTPUT); /* warn: Arduino specific */
    }
};

#endif