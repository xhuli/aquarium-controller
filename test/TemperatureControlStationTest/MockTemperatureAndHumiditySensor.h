#ifndef DOSINGSTATION_MOCKTEMPERATUREANDHUMIDITYSENSOR_H
#define DOSINGSTATION_MOCKTEMPERATUREANDHUMIDITYSENSOR_H

#include <stdint.h>
#include "Sensors/AbstractTemperatureAndHumiditySensor.h"

class MockTemperatureAndHumiditySensor : public AbstractTemperatureAndHumiditySensor {
public:
    ~MockTemperatureAndHumiditySensor() override = default;

    void mockTemperatureCelsius(float inputTemperatureCelsius) {
        temperatureCelsius = inputTemperatureCelsius;
    }

    void mockHumidityPercent(float inputHumidityPercent) {
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
