#ifndef __AQUARIUM_CONTROLLER_ABSTRACT_DEVICE_H__
#define __AQUARIUM_CONTROLLER_ABSTRACT_DEVICE_H__

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

    virtual void release() = 0; // go to the device default state: heaters->On, coolers->Off

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;

};

#endif
