#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_SLEEPABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_SLEEPABLE_H_
#pragma once

/**
 * <br/>
 * To do / implement:
 * <ul>
 * <li><tt>startSleeping(uint32_t sleepMs)</tt></li>
 * <li><tt>stopSleeping()</tt></li>
 * </ul>
 */
class AbstractSleepable {
protected:
    uint32_t sleepMs = 0;
    uint32_t sleepStartMs = 0;
    bool sleeping = false;

public:
    bool isSleeping() const {
        return sleeping;
    }

    virtual void startSleeping(uint32_t const &sleepMs) {
        AbstractSleepable::sleepMs = sleepMs;
        AbstractSleepable::sleepStartMs = millis();
        AbstractSleepable::sleeping = true;
    }

    virtual void stopSleeping() {
        AbstractSleepable::sleeping = false;
    }

    virtual bool shouldStopSleeping() {
        return (AbstractSleepable::isSleeping() && (millis() - AbstractSleepable::sleepStartMs >= AbstractSleepable::sleepMs));
    }
};

#endif