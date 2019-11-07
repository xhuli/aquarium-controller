#ifndef _AQUARIUM_CONTROLLER_INCLUDE_COMMON_SWITCHABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_COMMON_SWITCHABLE_H_
#pragma once

#include "Enums/Switched.h"

/**
 * <br/>
 * To do, override (probably):
 * <ul>
 * <li><tt>void Switchable::setState(Switched newState)</tt></li>
 * </ul>
 */
class Switchable {

private:

    Switched state;

public:

    explicit Switchable() : state(Switched::Off) {};

    explicit Switchable(Switched state) : state(state) {};

    virtual ~Switchable() = default;

    bool isInState(Switched const compareState) const {
        return state == compareState;
    }

    Switched const &getState() const {
        return state;
    }

    /**
     * <br/>
     * Default implementation, can be overwritten!<br/>
     * Sets new state as the current state.
     *
     * @param newState – the new state to be set
     */
    virtual void setState(Switched newState) {
        state = newState;
    }

    /**
     * <br/>
     * Sets new state if the new state is different from the old state and the condition evaluates to <tt>true</tt>.
     *
     * @param newState – the new state to be set
     * @param condition – condition that must evaluate to <tt>true</tt> for the new state to be set
     * @return <tt>true</tt> if the new state was set
     */
    bool setStateIf(Switched newState, bool condition) {
        if (state != newState && condition) {
            setState(newState);
            return true;
        }
        return false;
    }

    bool operator==(Switchable rhs) const {
        return !(this != &rhs); // warn: do not use == , it will loop
    }
};

#endif
