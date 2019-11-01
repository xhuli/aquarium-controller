#ifndef _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_REMOTE_CONTROL_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_REMOTE_CONTROL_H_
#pragma once

#ifdef __MODE_EDITING_CLION__

#include <Common/LinkedHashMap.h>
#include "../../.pio/libdeps/nanoatmega328/IRremote_ID4/IRremote.h"

#else
#include <IRremote.h>
#endif

class IrAction {

public:
    void invoke() {};
};

class DosingIrControl :
        public AbstractRunnable {
private:
    IRrecv irReceiver;
    LinkedHashMap<uint32_t, IrAction> actionsMap;

    decode_results irReading{};

public:

    explicit DosingIrControl(
            const uint8_t mcuPin,
            const LinkedHashMap<uint32_t, IrAction> &actionsMap
    ) : irReceiver(mcuPin), actionsMap(actionsMap) {}

    void setup() override {
        irReceiver.enableIRIn(); // <- starts the receiver
    }

    void loop() override {
        if (irReceiver.decode(&irReading)) {
#ifdef __SERIAL_DEBUG__
            Serial << "DosingIrControl::irReading == " << static_cast<uint8_t>(irReading.value) << "\n";
#endif
            /* todo: what about long clicks, and double clicks? */
            invokeActionForIrCode(irReading.value);

            irReceiver.resume(); // <- receive the next value
        }
    }

private:
    void invokeActionForIrCode(uint32_t const &code) {

//        IrAction defaultAction{};
//        IrAction action = DosingIrControl::actionsMap.getOrDefault(irReading.value, defaultAction);

        if (actionsMap.containsKey(code)) {
            IrAction action = DosingIrControl::actionsMap.get(irReading.value);
            action.invoke();
        }
    }
};

#endif