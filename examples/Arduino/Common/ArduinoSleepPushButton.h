#ifndef _AQUARIUM_CONTROLLER__ARDUINO_ATO_STATION_ARDUINO_ATO_PUSH_BUTTON_H_
#define _AQUARIUM_CONTROLLER__ARDUINO_ATO_STATION_ARDUINO_ATO_PUSH_BUTTON_H_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Abstract/AbstractPushButton.h>
#include <AtoStation/AtoStation.h>

class ArduinoSleepPushButton :
        public AbstractRunnable,
        public AbstractPushButton {

private:

    uint8_t sleepMinutes;
    ISleepable &sleepable;
    const uint8_t mcuPin;

    void shortClick() override {
        sleepable.stopSleeping();
    }

    void longClick() override {
        sleepable.startSleeping(sleepMinutes * 60 * 1000ul);
    }

public:

    ArduinoSleepPushButton(
            uint16_t debounceMs,
            uint16_t shortClickMs,
            uint8_t sleepMinutes,
            ISleepable &sleepable,
            uint8_t mcuPin
    ) :
            AbstractPushButton(Switched::Off, debounceMs, shortClickMs),
            sleepMinutes(sleepMinutes),
            sleepable(sleepable),
            mcuPin(mcuPin) {}

    void setup() override {
        pinMode(mcuPin, INPUT);
    }

    void loop() override {
        if (digitalRead(mcuPin) == HIGH) { // <- if digital pin
#ifdef __SERIAL_DEBUG__
            Serial << "ArduinoSleepPushButton::loop() => Switched::On\n";
#endif
            setState(Switched::On);
        } else {
#ifdef __SERIAL_DEBUG__
            Serial << "ArduinoSleepPushButton::loop() => Switched::Off\n";
#endif
            setState(Switched::Off);
        }
    }
};

#endif