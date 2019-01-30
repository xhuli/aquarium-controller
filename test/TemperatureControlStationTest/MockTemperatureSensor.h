#ifndef DOSINGSTATION_MOCKTEMPERATURESENSOR_H
#define DOSINGSTATION_MOCKTEMPERATURESENSOR_H

#include <stdint.h>
#include "Sensors/AbstractTemperatureSensor.h"

class MockTemperatureSensor : public AbstractTemperatureSensor {
public:
    ~MockTemperatureSensor() override = default;

    void mockTemperatureCelsius(float inputTemperatureCelsius) {
        temperatureCelsius = inputTemperatureCelsius;
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif
