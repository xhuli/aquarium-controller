#ifndef _TEST_MOCKS_MOCK_SWITCHABLE_H_
#define _TEST_MOCKS_MOCK_SWITCHABLE_H_
#pragma once

#include "Abstract/AbstractSwitchable.h"

class Switchable : public AbstractSwitchable {

public:

    explicit Switchable() : AbstractSwitchable(Switched::Off) {}

    explicit Switchable(Switched state) : AbstractSwitchable(state) {}

    void setState(Switched const newState) override {
        state = newState;
    }

    bool operator==(Switchable rhs) const {
        return !(this != &rhs); // warn: do not use == , it will loop
    }
};

#endif