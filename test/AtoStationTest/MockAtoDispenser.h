#ifndef __MOCK_ATO_DISPENSER_H__
#define __MOCK_ATO_DISPENSER_H__

#include "AtoStation/AbstractAtoDispenser.h"

class MockAtoDispenser : public AbstractAtoDispenser {
public:
    ~MockAtoDispenser() override = default;

    void startDispensing() override {
        isDispensing = true;
    }

    void stopDispensing() override {
        isDispensing = false;
    }

    void setup() override {
        // pass
    }

    void update(uint32_t currentMillis) override {
        // pass
    }
};

#endif