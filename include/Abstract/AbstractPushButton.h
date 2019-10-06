#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_PUSH_BUTTON_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_PUSH_BUTTON_H_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Enums/Switched.h>

/**
 * <br/>
 * To do, implement:
 * <ul>
 * <li><tt>void AbstractRunnable::setup()</tt></li>
 * <li><tt>void AbstractRunnable::loop()</tt> and <strong>important</strong> call <tt>AbstractPushButton::setState(Switched const state)</tt></li>
 * <li><tt>void AbstractPushButton::shortClick()</tt></li>
 * <li><tt>void AbstractPushButton::longClick()</tt></li>
 * </ul>
 */
class AbstractPushButton {

protected:

    Switched normalState;
    Switched state;
    Switched previousState;
    uint16_t debounceMs;
    uint16_t shortClickMs;
    uint32_t buttonDownMs = 0;

    virtual void shortClick() = 0; /* todo: OVERRIDE !!! */

    virtual void longClick() = 0; /* todo: OVERRIDE !!! */

public:

    AbstractPushButton(Switched normalState, uint16_t debounceMs, uint16_t shortClickMs) :
            normalState(normalState),
            state(normalState),
            previousState(normalState),
            debounceMs(debounceMs),
            shortClickMs(shortClickMs) {};

    /**
     * Call this method from the implementation <tt>loop()</tt> method.
     * @param state – the current state of the push button
     */
    void setState(Switched const newState) {
#ifdef __SERIAL_DEBUG__
        Serial << "AbstractPushButton::setState()\n";
#endif
        if (state != newState) {
            state = newState;
#ifdef __SERIAL_DEBUG__
            if (newState == Switched::Off) {
                Serial << "AbstractPushButton::newState = Switched::Off\n";
            } else {
                Serial << "AbstractPushButton::newState = Switched::On\n";
            }
#endif
            if (previousState == normalState && state != normalState) {
                /* button pushed */
                previousState = state;
                buttonDownMs = millis();
            } else if (previousState != normalState && state == normalState) {
                /* button released */
                previousState = state;
                if (millis() - buttonDownMs <= debounceMs) {
                    // pass
                } else if (millis() - buttonDownMs <= shortClickMs) {
                    shortClick();
                } else {
                    longClick();
                }
            }
        }
    }
};

#endif