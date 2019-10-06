#ifndef _MOCK_PUSH_BUTTON_H_
#define _MOCK_PUSH_BUTTON_H_
#pragma once

#include <Abstract/AbstractPushButton.h>

enum class Click : uint8_t {
    NoClick,
    Short,
    Long,
};

class MockPushButton :
        public AbstractRunnable,
        public AbstractPushButton {

private:

    Click click = Click::NoClick;

public:

    MockPushButton() : AbstractPushButton(Switched::Off) {}

    MockPushButton(
            Switched switched,
            uint16_t debounceMs,
            uint16_t shortClickMs
    ) :
            AbstractPushButton(switched, debounceMs, shortClickMs) {}

    void setup() override {
        //
    }

    void loop() override {
        //
    }

    void push() {
        setState(Switched::On);
    }

    void release() {
        setState(Switched::Off);
    }

    Click getClick() const {
        return click;
    }

protected:

    void shortClick() override {
        // std::cout << "\tPushButton :: shortClick()\n";
        click = Click::Short;
    }

    void longClick() override {
        // std::cout << "\tPushButton :: longClick()\n";
        click = Click::Long;
    }
};

#endif