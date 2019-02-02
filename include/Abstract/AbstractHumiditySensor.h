#ifndef __ABSTRACT_HUMIDITY_SENSOR_H__
#define __ABSTRACT_HUMIDITY_SENSOR_H__

#include <stdint.h>

class AbstractHumiditySensor {
protected:
    float humidityPercent = -1.0f;
    float deltaHumidityPercent = 0;

public:
    virtual ~AbstractHumiditySensor() = default;

    void calibrateHumidityTo(float calibrateToHumidityPercent) {
        /* assuming linear deviation */
        deltaHumidityPercent = calibrateToHumidityPercent - humidityPercent;

        // todo: store calibration
    }

    float getHumidityPercent() {
        return humidityPercent + deltaHumidityPercent;
    }

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;
};

#endif