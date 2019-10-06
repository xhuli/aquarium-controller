#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_RUNNABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_ABSTRACT_RUNNABLE_H_
#pragma once

/**
 * <br/>
 * <a href="https://paulmurraycbr.github.io/ArduinoTheOOWay.html#thespookyway">Arduino the Object Oriented way</a>
 * by paulmurraycbr
 *
 * <br/>
 * To do, implement:
 * <ul>
 * <li><tt>void AbstractRunnable::setup()</tt></li>
 * <li><tt>void AbstractSwitchable::setState(Switched newState)</tt></li>
 * </ul>
 * Note: setup() and loop() can be split into different classes, but
 * that would cost additional 2 bytes for per pointer to the object.
 */
class AbstractRunnable {

private:

    static AbstractRunnable *head;
    AbstractRunnable *next;

public:

    AbstractRunnable() {
        /* LIFO: head will point to last instance, first instance will point to null */
        next = head; // save pointer to previous instance
        head = this; // move head to this instance (for now)
    }

    ~AbstractRunnable() {
        AbstractRunnable **tracer = &head;

        /* https://www.youtube.com/watch?v=0ZEX_l0DFK0 */
        while (*tracer) {
            if ((*tracer) == this) {
                *tracer = (*tracer)->next;
                break;
            }

            tracer = &(*tracer)->next;
        }
    }

    virtual void setup() = 0; /* todo: OVERRIDE !!! */

    virtual void loop() = 0; /* todo: OVERRIDE !!! */

    static void setupAll() {
        for (AbstractRunnable *pRunnable = head; pRunnable; pRunnable = pRunnable->next) {
            pRunnable->setup();
        }
    }

    static void loopAll() {
        for (AbstractRunnable *pRunnable = head; pRunnable; pRunnable = pRunnable->next) {
            pRunnable->loop();
        }
    }
};

AbstractRunnable *AbstractRunnable::head = nullptr; // set initial head to nullptr

#endif