#ifndef _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_BUZZER_H_
#define _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_BUZZER_H_
#pragma once

#include <Abstract/AbstractBuzzer.h>

class ArduinoBuzzer : public AbstractBuzzer {

private:

    const uint8_t mcuPin;

    void setState(Switched const newState) override {
        if (AbstractBuzzer::getState() != newState) {

            AbstractBuzzer::setState(newState);

            if (AbstractBuzzer::isInState(Switched::On)) {
                digitalWrite(mcuPin, HIGH); /* warn: Arduino specific */
            } else {
                digitalWrite(mcuPin, LOW); /* warn: Arduino specific */
            }
        }
    }

public:

    ArduinoBuzzer(uint8_t const mcuPin) : mcuPin(mcuPin) {}

    void setup() override {
        pinMode(mcuPin, OUTPUT); /* warn: Arduino specific */
    }
};

#endif