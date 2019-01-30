#ifndef AQUARIUMCONTROLLER_ABSTRACTDEVICE_H
#define AQUARIUMCONTROLLER_ABSTRACTDEVICE_H

#include <stdint.h>

class AbstractDevice {
protected:
    bool isStarted = false;

public:
    ~AbstractDevice() = default;

    bool isOn() {
        return isStarted;
    }

    bool isOff() {
        return !isStarted;
    }

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;
};

#endif
