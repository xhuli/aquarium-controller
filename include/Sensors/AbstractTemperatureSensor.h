#ifndef __ABSTRACT_TEMPERATURE_SENSOR_H__
#define __ABSTRACT_TEMPERATURE_SENSOR_H__

#include <stdint.h>

class AbstractTemperatureSensor {
protected:
    float temperatureCelsius = -127;
    float deltaTemperatureCelsius = 0;

public:
    virtual ~AbstractTemperatureSensor() = default;

    void calibrateTemperatureTo(float calibrateToTemperatureCelsius) {
        /* assuming linear deviation */
        deltaTemperatureCelsius = calibrateToTemperatureCelsius - temperatureCelsius;

        // todo: store calibration
    }

    float getTemperatureCelsius() {
        return temperatureCelsius + deltaTemperatureCelsius;
    }

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;
};

#endif