#ifndef __DOSING_PORT_H__
#define __DOSING_PORT_H__

#ifdef __TEST_MODE__

#include "MockMillis.h"
#include "MockCommon.h"

#endif

#include "DosingSchedule.h"
#include <Abstract/AbstractConfigurationStorage.h>
#include "Abstract/AbstractDispenser.h"

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
    DosingSchedule dosingSchedule = DosingSchedule();

    uint32_t taskStartMillis = 0;
    uint32_t taskDurationMillis = 0;
    uint32_t calibrationStartMillis = 0;
    uint16_t millisPerMilliLiter = 0;

    enum DosingPortState {
        IDLE = 0,
        DOSING = 1,
        CALIBRATING = 2,
        MANUAL = 3
    } state = IDLE;

    uint16_t milliLitersToMillis(float milliLiters) {
        return static_cast<uint16_t>(millisPerMilliLiter * milliLiters);
    }

public:
    DosingPort(uint8_t attachToNumber, AbstractConfigurationStorage *storagePointerToAttach) :
            portNumber(attachToNumber),
            storagePointer(storagePointerToAttach) {

    }

    ~DosingPort() {
        delete peristalticPumpPointer;
    }

    void addDosingTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMilliLiters,
                       uint8_t doseMilliLitersFraction) {
        if (dosingSchedule.addTask(dayOfWeek, startHour, startMinute, doseMilliLiters, doseMilliLitersFraction)) {
            storagePointer->saveDosingPortSchedule(portNumber, dosingSchedule);
        }
    }

    void updateTask(uint8_t index, uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMilliLiters,
                    uint8_t doseMilliLitersFraction) {
        if (dosingSchedule.updateTask(index, dayOfWeek, startHour, startMinute, doseMilliLiters,
                                      doseMilliLitersFraction)) {
            storagePointer->saveDosingPortSchedule(portNumber, dosingSchedule);
        }
    }

    void removeTask(uint8_t index) {
        dosingSchedule.removeTask(index);
        storagePointer->saveDosingPortSchedule(portNumber, dosingSchedule);
    }

    void setSpeed(uint8_t speed) {
        peristalticPumpPointer->setIntensity(speed);
    }

    void manualStartDispensing() {
        peristalticPumpPointer->startDispensing();
        state = DosingPortState::MANUAL;
    }

    void manualStopDispensing() {
        peristalticPumpPointer->stopDispensing();
        state = DosingPortState::IDLE;
    }

    void startCalibration() {
        calibrationStartMillis = millis();
        peristalticPumpPointer->startDispensing();
        state = DosingPortState::CALIBRATING;
    }

    void stopCalibration() {
        /* should be called after dispensing 100mL of liquid */
        millisPerMilliLiter = static_cast<uint16_t>((millis() - calibrationStartMillis) / 100);
        peristalticPumpPointer->stopDispensing();
        state = DosingPortState::IDLE;

        storagePointer->saveDosingPortCalibration(portNumber, millisPerMilliLiter);
    }

    void startDosing(uint32_t currentMillis) {
        taskStartMillis = currentMillis;
        peristalticPumpPointer->startDispensing();
        state = DosingPortState::DOSING;
    }

    void stopDosing() {
        peristalticPumpPointer->stopDispensing();
        state = DosingPortState::IDLE;
    }

    void setup() {
        setSpeed(240);

        /* read calibration and schedule from storage */
        millisPerMilliLiter = storagePointer->readDosingPortCalibration(portNumber, millisPerMilliLiter);
        millisPerMilliLiter = static_cast<uint16_t>((millisPerMilliLiter == 0) ? 1000 : millisPerMilliLiter);
        dosingSchedule = storagePointer->readDosingPortSchedule(portNumber, dosingSchedule);

        state = DosingPortState::IDLE;
    }

    void update(bool minuteHeartbeat, uint32_t currentMillis) {
        //
        switch (state) {
            //
            case DosingPortState::IDLE:
                if (minuteHeartbeat) {
                    taskDurationMillis = milliLitersToMillis(dosingSchedule.getPendingDoseMilliLiters());
                    if (taskDurationMillis > 0) {
                        startDosing(currentMillis);
                    }
                }

                break;

            case DosingPortState::DOSING:
                if ((currentMillis - taskStartMillis) > taskDurationMillis) {
                    stopDosing();
                }

                break;

            case DosingPortState::CALIBRATING:
            case DosingPortState::MANUAL:
            default:
                break;
        }
    }
};

#endif