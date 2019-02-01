#ifndef __MOCK_LIQUID_LEVEL_SENSOR_H__
#define __MOCK_LIQUID_LEVEL_SENSOR_H__

#include "Abstract/AbstractLiquidLevelSensor.h"

class MockLiquidLevelSensor : public AbstractLiquidLevelSensor {
public:
    void mockIsSensing() {
        isSensing = true;
    }

    void mockIsNotSensing() {
        isSensing = false;
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif