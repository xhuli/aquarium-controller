#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_SWITCHABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_SWITCHABLE_H_
#pragma once

#include "Enums/Switched.h"
#include "AbstractRunnable.h"

/**
 * <br/>
 * To do, implement:
 * <ul>
 * <li><tt>void AbstractSwitchable::setState(Switched newState)</tt></li>
 * </ul>
 */
class AbstractSwitchable {

protected:

    Switched state;

public:

    explicit AbstractSwitchable() : state(Switched::Off) {};

    explicit AbstractSwitchable(Switched state) : state(state) {};

    virtual ~AbstractSwitchable() = default;

    bool isInState(Switched const compareState) const {
        return state == compareState;
    }

    Switched const &getState() const {
        return state;
    }

    virtual void setState(Switched newState) = 0; /* todo: OVERRIDE !!! */

    bool setStateIf(Switched newState, bool condition) {
        if (state != newState && condition) {
            setState(newState);
            return true;
        }
        return false;
    }
};

#endif
