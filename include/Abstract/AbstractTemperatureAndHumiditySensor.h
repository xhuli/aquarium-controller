#ifndef __ABSTRACT_TEMPERATURE_AND_HUMIDITY_SENSOR_H__
#define __ABSTRACT_TEMPERATURE_AND_HUMIDITY_SENSOR_H__

#include "AbstractHumiditySensor.h"
#include "AbstractTemperatureSensor.h"

class AbstractTemperatureAndHumiditySensor : public AbstractHumiditySensor, public AbstractTemperatureSensor {
   public:
    virtual ~AbstractTemperatureAndHumiditySensor() = default;
    virtual void setup() = 0;
    virtual void update(uint32_t currentMillis) = 0;
};

#endif