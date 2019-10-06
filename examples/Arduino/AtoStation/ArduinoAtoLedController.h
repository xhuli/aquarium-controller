#ifndef _AQUARIUM_CONTROLLER_ARDUINO_ATO_STATION_ARDUINO_ATO_LED_CONTROLLER_H_
#define _AQUARIUM_CONTROLLER_ARDUINO_ATO_STATION_ARDUINO_ATO_LED_CONTROLLER_H_
#pragma once

#include <AtoStation/AtoStation.h>
#include "../Common/ArduinoBlinkingLed.h"

class ArduinoAtoLedController : public AbstractRunnable {

private:

    AtoStation &atoStation;

    ArduinoBlinkingLed redLed;
    ArduinoBlinkingLed yellowLed;
    ArduinoBlinkingLed greenLed;

    AtoStationState atoStationState = AtoStationState::Invalid;

public:

    ArduinoAtoLedController(AtoStation &atoStationToAttachTo, uint8_t redLedPin, uint8_t yellowLedPin, uint8_t greenLedPin) :
            atoStation(atoStationToAttachTo),
            redLed{redLedPin},
            yellowLed{yellowLedPin},
            greenLed{greenLedPin} {}

    void setup() override {
        redLed.setup();
        yellowLed.setup();
        greenLed.setup();
    }

    void loop() override {

        if (atoStationState == atoStation.getState()) {
            return;
        } else {
            atoStationState = atoStation.getState();
        }

        switch (atoStationState) {

            case AtoStationState::Sensing:
                redLed.setOff();
                yellowLed.setOff();
                greenLed.cycleOnOffMs(1820, 680);
                break;

            case AtoStationState::Dispensing:
                redLed.setOff();
                yellowLed.setOff();
                greenLed.setOn();
                break;

            case AtoStationState::Sleeping:
                redLed.setOff();
                yellowLed.cycleOnOffMs(1820, 680);
                greenLed.setOff();
                break;

            case AtoStationState::Alarming:
                redLed.cycleOnOffMs(680, 480);
                yellowLed.setOff();
                greenLed.setOff();
                break;

            default:
                redLed.setOn();
                yellowLed.setOn();
                greenLed.setOn();
                break;
        }
    }
};

#endif