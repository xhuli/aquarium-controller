#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_COUNT_DOWN_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_COUNT_DOWN_H_
#pragma once

#include <Abstract/AbstractRunnable.h>

class CountDown : public AbstractRunnable {

private:

    uint32_t countDownMs;
    uint32_t countStartMs = 0;
    bool counting = false;

public:

    void setup() override {}

    void loop() override {
        if (counting) {
            if (millis() - countStartMs > countDownMs) {
                counting = false;
            }
        }
    }

    void start(uint32_t milliseconds) {
        if (!counting) {
            CountDown::countDownMs = milliseconds;
            counting = true;
        }
    }

    bool isCounting() const {
        return counting;
    }

    bool isNotCounting() const {
        return !counting;
    }
};

#endif