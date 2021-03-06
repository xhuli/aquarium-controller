#ifndef _AQUARIUM_CONTROLLER_MOCK_BUZZER_H_
#define _AQUARIUM_CONTROLLER_MOCK_BUZZER_H_
#pragma once

#include <Abstract/AbstractBuzzer.h>

class MockBuzzer : public AbstractBuzzer {

public:

    MockBuzzer() : AbstractBuzzer() {};

    MockBuzzer(uint16_t restMs) : AbstractBuzzer(restMs) {};

    void setState(Switched const newState) override {
        AbstractBuzzer::setState(newState);
    }

    void setup() override {
        // pass
    }
};

#endif
