#ifndef __DOSING_PORT_H__
#define __DOSING_PORT_H__

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

#endif

#include "Common/DayOfWeek.h"
#include "DosingSchedule.h"
#include <Abstract/AbstractConfigurationStorage.h>
#include <Abstract/AbstractDispenser.h>

/*!
    To calibrate dosing ports, call `startCalibration()`,
    wait for the port to dispense 100 mL, and call `stopCalibration()`.
     - min dose is 0.25 mL
     - max dose is 255.75 mL
     - dose increments by 0.25 mL. 
*/
class DosingPort {
private:
    uint8_t portNumber;
    AbstractConfigurationStorage *storagePointer;
    AbstractDispenser *peristalticPumpPointer = nullptr;
//    DosingSchedule schedule = DosingSchedule();

    uint32_t taskStartMillis = 0;
    uint32_t taskDurationMillis = 0;
    uint32_t calibrationStartMillis = 0;
    uint16_t millisPerMilliLiter = 0;

    bool hasPeristalticPump = false;

    uint32_t milliLitersToMillis(float milliLiters) {
        return static_cast<uint32_t>(millisPerMilliLiter * milliLiters);
    }

public:
    DosingSchedule schedule = DosingSchedule();

    DosingPort(uint8_t attachToNumber, AbstractConfigurationStorage *storagePointerToAttach) :
            portNumber(attachToNumber),
            storagePointer(storagePointerToAttach) {

    }

    ~DosingPort() {
        delete peristalticPumpPointer;
    }

#ifdef __TEST_MODE__

    uint16_t getMillisPerMilliLiter() {
        return millisPerMilliLiter;
    }

    uint32_t getCurrentTaskTotalDurationMillis() {
        return taskDurationMillis;
    }

#endif

    enum class State {
        IDLE = 0,
        DISPENSING = 1,
        CALIBRATING = 2,
        MANUAL = 3,
        INVALID = 4,
    } state = State::IDLE;

    void attachPeristalticPump(AbstractDispenser *abstractDispenserPointer) {
        peristalticPumpPointer = abstractDispenserPointer;
        hasPeristalticPump = true;
    }

    State getCurrentState() {
        return state;
    }

    void setSpeed(uint8_t speed) {
        peristalticPumpPointer->setIntensity(speed);
    }

    void manualStartDispensing() {
        peristalticPumpPointer->startDispensing();
        state = State::MANUAL;
    }

    void manualStopDispensing() {
        peristalticPumpPointer->stopDispensing();
        state = State::IDLE;
    }

    void startCalibration() {
        calibrationStartMillis = millis();
        peristalticPumpPointer->startDispensing();
        state = State::CALIBRATING;
    }

    void stopCalibration() {
        /* should be called after dispensing 100mL of liquid */
        millisPerMilliLiter = static_cast<uint16_t>((millis() - calibrationStartMillis) / 100);
        peristalticPumpPointer->stopDispensing();
        state = State::IDLE;

        storagePointer->saveDosingPortCalibration(portNumber, millisPerMilliLiter);
    }

    void setup() {
        setSpeed(240);

        /* read calibration and schedule from storage */
        millisPerMilliLiter = storagePointer->readDosingPortCalibration(portNumber, millisPerMilliLiter);
        millisPerMilliLiter = static_cast<uint16_t>((millisPerMilliLiter == 0) ? 1000 : millisPerMilliLiter);
        schedule = storagePointer->readDosingPortSchedule(portNumber, schedule);

        if (hasPeristalticPump) {
            state = State::IDLE;
        } else {
            // todo: raise alarm

            state = State::INVALID;
        }
    }

    void reset() {
        setup();
    }

    void update(bool minuteHeartbeat, uint32_t currentMillis) {
        //
        switch (state) {
            //
            case State::IDLE:
                if (minuteHeartbeat) {
                    taskDurationMillis = milliLitersToMillis(schedule.getPendingDoseMilliLiters());
                    if (taskDurationMillis > 0) {
                        //
                        taskStartMillis = currentMillis;
                        peristalticPumpPointer->startDispensing();
                        state = State::DISPENSING;
                    }
                }

                break;

            case State::DISPENSING:
                if ((currentMillis - taskStartMillis) > taskDurationMillis) {
                    //
                    taskDurationMillis = 0;
                    peristalticPumpPointer->stopDispensing();
                    state = State::IDLE;
                }

                break;

            case State::CALIBRATING:
            case State::MANUAL:
            case State::INVALID:
            default:
                break;
        }
    }
};

#endif