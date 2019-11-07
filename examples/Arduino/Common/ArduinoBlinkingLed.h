#ifndef _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_BLINKING_LED_
#define _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_BLINKING_LED_
#pragma once

#include <Abstract/AbstractCyclicSwitch.h>

class ArduinoBlinkingLed : public AbstractCyclicSwitch {

    uint8_t mcuPin;

    void setState(Switched newState) override {

        AbstractCyclicSwitch::setState(newState);

        if (AbstractCyclicSwitch::isInState(Switched::On)) {
            digitalWrite(mcuPin, HIGH); /* warn: Arduino specific */
        } else {
            digitalWrite(mcuPin, LOW); /* warn: Arduino specific */
        }
    }

public:

    explicit ArduinoBlinkingLed(uint8_t const mcuPin) :
            AbstractCyclicSwitch(Switched::Off),
            mcuPin(mcuPin) {}

    void setup() override {
        pinMode(mcuPin, OUTPUT); /* warn: Arduino specific */
    }
};

#endif