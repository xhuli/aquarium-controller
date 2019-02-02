#ifndef AQUARIUMCONTROLLER_MOCKDEVICE_H
#define AQUARIUMCONTROLLER_MOCKDEVICE_H

#include <stdint.h>
#include "Abstract/AbstractDevice.h"

class MockDevice : public AbstractDevice {
public:
    ~MockDevice() = default;

    void mockStarted() {
        isStarted = true;
    }

    void mockNotStarted() {
        isStarted = false;
    }

    void start() override {
        isStarted = true;
    }

    void stop() override {
        isStarted = false;
    }

    void release() override {
        // go to the device specific default state
        // start(); // if heating device, start the heater in colder climates
        stop(); // // if cooling device stop the cooler in colder climates
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif
