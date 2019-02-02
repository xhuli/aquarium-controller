#ifndef __MOCK_ATO_DISPENSER_H__
#define __MOCK_ATO_DISPENSER_H__

#include "Abstract/AbstractDispenser.h"

class MockLiquidDispenser : public AbstractDispenser {
public:
    ~MockLiquidDispenser() override = default;

    void startDispensing() override {
        isDispensing = true;
    }

    void stopDispensing() override {
        isDispensing = false;
    }

    void setIntensity(uint16_t intensity) override {
        /* Examples:
         * set motor speed
         * set valve open percent
         * set valve turn angle
         * ...
        */
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif