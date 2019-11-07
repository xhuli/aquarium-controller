#ifndef _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_SWITCHABLE_H_
#define _AQUARIUM_CONTROLLER_ARDUINO_COMMON_ARDUINO_SWITCHABLE_H_

#ifndef HIGH
#define HIGH 0x1
#endif

#ifndef LOW
#define LOW  0x0
#endif

#ifndef INPUT
#define INPUT 0x0
#endif

#ifndef OUTPUT
#define OUTPUT 0x1
#endif

#include <stdint.h>

#include "Enums/Switched.h"
#include "Common/Switchable.h"
#include "Abstract/AbstractRunnable.h"

class ArduinoSwitchable :
        public AbstractRunnable,
        public Switchable {

private:

    uint8_t const mcuPin;

public:

    explicit ArduinoSwitchable(uint8_t const mcuPin) : mcuPin(mcuPin) {}

    void setState(Switched const newState) override {
        if (Switchable::getState() != newState) {

            Switchable::setState(newState);

            if (Switchable::isInState(Switched::On)) {
                digitalWrite(mcuPin, HIGH); /* warn: Arduino specific */
            } else {
                digitalWrite(mcuPin, LOW); /* warn: Arduino specific */
            }
        }
    }

    void setup() override {
        pinMode(mcuPin, OUTPUT); /* warn: Arduino specific */
    };

    void loop() override {
        // pass
    }
};

#endif