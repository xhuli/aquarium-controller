#ifndef DOSINGSTATION_MOCKTEMPERATURESENSOR_H
#define DOSINGSTATION_MOCKTEMPERATURESENSOR_H

#include <stdint.h>
#include "Sensors/AbstractTemperatureSensor.h"

class MockTemperatureSensor : public AbstractTemperatureSensor {

    void mockSetTemperatureCelsius(float inputTemperatureCelsius) {
        temperatureCelsius = inputTemperatureCelsius;
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif //DOSINGSTATION_MOCKTEMPERATURESENSOR_H
