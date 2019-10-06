#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_BLINKING_LED_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_BLINKING_LED_H_
#pragma once

/**
 * <br/>
 * Implement:
 * <ul>
 * <li><tt>void AbstractRunnable::setup()</tt>.</li>
 * <li><tt>void AbstractSwitchable::setState(Switched newState)</tt>.</li>
 * </ul>
 */
class AbstractCyclicSwitch :
        public AbstractRunnable,
        public AbstractSwitchable {

protected:
    uint32_t cycleOnMs = 0;
    uint32_t cycleOffMs = 0;
    uint32_t lastCycleMs = 0;

    bool isCycling = false;

public:

    explicit AbstractCyclicSwitch() : AbstractSwitchable() {};

    explicit AbstractCyclicSwitch(Switched state) : AbstractSwitchable(state) {}

    ~AbstractCyclicSwitch() override = default;

    void setOn() {
        isCycling = false;
        setState(Switched::On);
    }

    void setOff() {
        isCycling = false;
        setState(Switched::Off);
    }

    void cycleOnOffMs(uint32_t const onMs, uint32_t const offMs) {
        if (cycleOnMs != onMs) cycleOnMs = onMs;
        if (cycleOffMs != offMs) cycleOffMs = offMs;
        if (!isCycling) isCycling = true;
    }

    void loop() override {
#ifdef __SERIAL_DEBUG__
        // Serial << "AbstractCyclicSwitch::isCycling = " << isCycling << "\n";
#endif

        if (isCycling) {
            if (AbstractSwitchable::isInState(Switched::On)) {
                if (millis() - lastCycleMs >= cycleOnMs) {
                    setState(Switched::Off);
                    lastCycleMs = millis();
                }
            } else {
                if (lastCycleMs == 0 || (millis() - lastCycleMs >= cycleOffMs)) {
                    setState(Switched::On);
                    lastCycleMs = millis();
                }
            }
        }
    }
};

#endif