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

protected:

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

    /* todo: Probably Override !!! */
    virtual void setState(Switched newState) {
        state = newState;
    }

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
