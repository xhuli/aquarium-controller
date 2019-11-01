#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_BLINKING_LED_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_BLINKING_LED_H_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Common/Switchable.h>

/**
 * <br/>
 * Implement/Override:
 * <ul>
 * <li><tt>void AbstractRunnable::setup()</tt>.</li>
 * <li><tt>void Switchable::setState(Switched newState)</tt>.</li>
 * </ul>
 */
class AbstractCyclicSwitch :
        public AbstractRunnable,
        public Switchable {

protected:
    uint32_t cycleOnMs = 0;
    uint32_t cycleOffMs = 0;
    uint32_t lastSwitchMs = 0;

    bool isCycling = false;

public:

    explicit AbstractCyclicSwitch() : Switchable() {};

    explicit AbstractCyclicSwitch(Switched state) : Switchable(state) {}

    ~AbstractCyclicSwitch() override = default;

    void setOn() {
        AbstractCyclicSwitch::isCycling = false;
        setState(Switched::On);
    }

    void setOff() {
        AbstractCyclicSwitch::isCycling = false;
        setState(Switched::Off);
    }

    void cycleOnOffMs(uint32_t const onMs, uint32_t const offMs) {
        if (AbstractCyclicSwitch::cycleOnMs != onMs) { AbstractCyclicSwitch::cycleOnMs = onMs; }
        if (AbstractCyclicSwitch::cycleOffMs != offMs) { AbstractCyclicSwitch::cycleOffMs = offMs; }
        if (!AbstractCyclicSwitch::isCycling) { AbstractCyclicSwitch::isCycling = true; }
    }

    void loop() override {
#ifdef __SERIAL_DEBUG__
        // Serial << "AbstractCyclicSwitch::isCycling = " << isCycling << "\n";
#endif

        if (AbstractCyclicSwitch::isCycling) {
            if (Switchable::isInState(Switched::On)) {
                if (millis() - AbstractCyclicSwitch::lastSwitchMs >= AbstractCyclicSwitch::cycleOnMs) {
                    setState(Switched::Off);
                    AbstractCyclicSwitch::lastSwitchMs = millis();
                }
            } else {
                if (AbstractCyclicSwitch::lastSwitchMs == 0 || (millis() - AbstractCyclicSwitch::lastSwitchMs >= AbstractCyclicSwitch::cycleOffMs)) {
                    setState(Switched::On);
                    AbstractCyclicSwitch::lastSwitchMs = millis();
                }
            }
        }
    }
};

#endif