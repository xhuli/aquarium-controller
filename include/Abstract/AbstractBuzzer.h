#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_BUZZER_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_BUZZER_H_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Abstract/AbstractSwitchable.h>

/**
 * <br/>
 * To do, implement:
 * <ul>
 * <li><tt>void AbstractRunnable::setup()</tt></li>
 * <li><tt>void AbstractSwitchable::setState(Switched newState)</tt></li>
 * </ul>
 */
class AbstractBuzzer :
        public AbstractRunnable,
        public AbstractSwitchable {

private:

    uint16_t buzzMs = 0;
    uint16_t buzzRestMs = 0;
    uint32_t buzzStartMs = 0;

    bool busy = false;

    void loop() override {
        if (busy) {
            if (AbstractSwitchable::isInState(Switched::On) && (millis() - buzzStartMs >= buzzMs)) {
                setState(Switched::Off);
            }

            /* add resting period after buzz */
            if (AbstractSwitchable::isInState(Switched::Off) && (millis() - buzzStartMs >= buzzMs + buzzRestMs)) {
                busy = false;
            }
        }
    }

public:

    explicit AbstractBuzzer() = default;

    explicit AbstractBuzzer(uint16_t buzzRestMs) : buzzRestMs(buzzRestMs) {};

    ~AbstractBuzzer() override = default;

    bool buzz(uint16_t const &buzzMs) {
        if (AbstractSwitchable::isInState(Switched::Off)) {
            AbstractBuzzer::buzzMs = buzzMs;
            setState(Switched::On);
            buzzStartMs = millis();
            busy = true;
            return true;
        }
        return false;
    }

    bool stop() {
        if (busy) {
            setState(Switched::Off);
            busy = false;
            return true;
        }
        return false;
    }

    bool isBusy() const {
        return busy;
    }
};

#endif