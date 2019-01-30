#ifndef AQUARIUMCONTROLLER_MOCKDEVICE_H
#define AQUARIUMCONTROLLER_MOCKDEVICE_H

#include <stdint.h>
#include "AbstractDevice.h"

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

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif
