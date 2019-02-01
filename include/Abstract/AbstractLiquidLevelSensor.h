#ifndef __ABSTRACT_LIQUID_LEVEL_SENSOR_H__
#define __ABSTRACT_LIQUID_LEVEL_SENSOR_H__

class AbstractLiquidLevelSensor {
protected:
    bool isSensing = false;

public:
    virtual ~AbstractLiquidLevelSensor() = default;

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;

    bool isSensingLiquid() {
        return isSensing;
    }

    bool isNotSensingLiquid() {
        return !isSensing;
    }
};

#endif