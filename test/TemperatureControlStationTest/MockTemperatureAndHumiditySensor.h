#ifndef DOSINGSTATION_MOCKTEMPERATUREANDHUMIDITYSENSOR_H
#define DOSINGSTATION_MOCKTEMPERATUREANDHUMIDITYSENSOR_H

#include <stdint.h>
#include "Sensors/AbstractTemperatureAndHumiditySensor.h"

class MockTemperatureAndHumiditySensor : public AbstractTemperatureAndHumiditySensor {

    void mockSetTemperatureCelsius(float inputTemperatureCelsius) {
        temperatureCelsius = inputTemperatureCelsius;
    }

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
