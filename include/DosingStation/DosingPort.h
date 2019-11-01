#ifndef _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_PORT_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_PORT_H_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Common/Switchable.h>
#include "DosingTasksList.h"

#ifdef __MODE_TESTING_PLATFORMIO__
#include <Adafruit_MotorShield.h>
#include <Time.h>
#endif

#ifdef __MODE_EDITING_CLION__
#include "../../test/_Mocks/MockCommon.h"
#endif

class Adafruit_DCMotor;

class DosingPort :
        public Switchable,
        public AbstractRunnable {

private:
    Adafruit_DCMotor *pDcMotor;

    uint16_t ms_per_mL = 1000;
    uint32_t dispensingStartMs = 0;
    uint32_t dispenseMs = 0;
    bool calibrating = false;

    void setState(Switched newState) override {
        /*
        std::cout
                << "Switch: " << static_cast<int >(newState)
                << " " << getDayOfWeek(weekday()) << " "
                << "/" << static_cast<int>(hour())
                << ":" << static_cast<int>(minute())
                << ":" << static_cast<int>(second())
                << "." << static_cast<int>(millis())
                << "\n";
        //*/

        Switchable::setState(newState);
        DosingPort::pDcMotor->run(newState == Switched::On ? FORWARD : RELEASE);
    }

public:
    DosingPort(
            Adafruit_DCMotor *pDcMotor,
            uint16_t milliSecondsPerMilliLiter
    ) : Switchable(),
        pDcMotor(pDcMotor),
        ms_per_mL(milliSecondsPerMilliLiter) {}

    DosingTasksList schedule;

    void startDispensing(uint32_t const &dispenseMs) {
        DosingPort::dispenseMs = dispenseMs;
        DosingPort::dispensingStartMs = millis();
        DosingPort::setState(Switched::On);
    }

    void stopDispensing() {
        DosingPort::setState(Switched::Off);
    }

    void setMilliSecondsPerMilliLiter(uint32_t ms_per_mL) {
        DosingPort::ms_per_mL = ms_per_mL;
    }

    uint16_t getMilliSecondsPerMilliLiter() const {
        return DosingPort::ms_per_mL;
    }

    /**
     * <br/>
     * To start dosing port calibration call <tt>startStopCalibrating()</tt>,<br/>
     * wait for the port to dispense 100 mL, and call <tt>startStopCalibrating()</tt> again.
     */
    void startStopCalibrating() {
        if (calibrating) {
            DosingPort::setMilliSecondsPerMilliLiter((millis() - dispensingStartMs) / 100ul);
            DosingPort::stopDispensing();
            DosingPort::calibrating = false;
        } else {
            if (DosingPort::isInState(Switched::Off)) {
                DosingPort::dispensingStartMs = millis();
                DosingPort::startDispensing(0);
                DosingPort::calibrating = true;
            }
        }
    }

    void runScheduledTasks() {
        if (DosingPort::isInState(Switched::Off)) {
            uint32_t scheduledDoseMs = DosingPort::getScheduledDoseMs();
            if (scheduledDoseMs > 0) {
                DosingPort::startDispensing(scheduledDoseMs);
            }
        }
    }

    void setup() override {}

    void loop() override {
        if (DosingPort::isInState(Switched::On) && !calibrating) {
            if (millis() - DosingPort::dispensingStartMs >= DosingPort::dispenseMs) {
                DosingPort::stopDispensing();
            }
        }
    }

private:
    /**
     * <br/>
     * Return zero or scheduled task duration in milliseconds for the current day, hour, and minute.<br/>
     * Duration is function of ports calibrated <tt>ms/mL</tt>, and scheduled task dose in <tt>mL</tt><br/>
     *
     * @return scheduled task duration in milliseconds for the current day, hour, and minute
     */
    uint32_t getScheduledDoseMs() {

        DosingTask *pDosingTask = DosingPort::schedule.get(static_cast<DayOfWeek>(weekday()), hour(), minute());

        if (pDosingTask != nullptr) {
            return DosingPort::ms_per_mL * ((0.25 * pDosingTask->doseMilliLiterQuarters) + pDosingTask->doseMilliLiters);
        }

        return 0;
    }
};

#endif