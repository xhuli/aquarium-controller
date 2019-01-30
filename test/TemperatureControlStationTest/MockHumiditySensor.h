#ifndef AQUARIUMCONTROLLER_MOCKHUMIDITYSENSOR_H
#define AQUARIUMCONTROLLER_MOCKHUMIDITYSENSOR_H

#include <stdint.h>
#include "Sensors/AbstractHumiditySensor.h"

class MockHumiditySensor : public AbstractHumiditySensor {
public:
    ~MockHumiditySensor() override = default;

    void mockSetHumidityPercent(float inputHumidityPercent) {
        humidityPercent = inputHumidityPercent;
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};


#endif
