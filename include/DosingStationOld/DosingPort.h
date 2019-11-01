#ifndef __DOSING_PORT_H__
#define __DOSING_PORT_H__

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

#endif

#include "Common/DayOfWeek.h"
#include <Common/Switchable.h>
#include "DosingSchedule.h"

/*!
    To calibrate dosing ports, call `startCalibration()`,
    wait for the port to dispense 100 mL, and call `stopCalibration()`.
     - min dose is 0.25 mL
     - max dose is 255.75 mL
     - dose increments by 0.25 mL. 
*/
class DosingPort : public AbstractRunnable {

private:
    uint8_t portNumber;
//    AbstractConfigurationStorage &configurationStorage;
    Switchable &peristalticPump;

    uint32_t taskStartMs = 0;
    uint32_t taskDurationMs = 0;
    uint32_t calibrationStartMs = 0;
    uint32_t ms_per_mL = 0;

    uint32_t milliLitersToMillis(float milliLiters) {
        return static_cast<uint32_t>(ms_per_mL * milliLiters);
    }

public:
    DosingSchedule schedule = DosingSchedule();

    explicit DosingPort(
            uint8_t attachToNumber,
//            AbstractConfigurationStorage &configurationStorage,
            Switchable &dispenserToAttach) :

            portNumber(attachToNumber),
//            configurationStorage(configurationStorage),
            peristalticPump(dispenserToAttach) {}

    ~DosingPort() = default;

    const enum class State : uint8_t {
        Idle,
        Dispensing,
        Calibrating,
        Manual,
        Invalid,
    } state = State::Idle;

#ifdef __TEST_MODE__

    uint16_t getMilliSecondsPerMilliLiter() {
        return ms_per_mL;
    }

    uint32_t getCurrentTaskTotalDurationMillis() {
        return taskDurationMs;
    }

    State const &getState() {
        return state;
    }

    bool isInState(State const &compareState) const {
        return state == compareState;
    }

#endif

    void manualStartDispensing() {
        peristalticPump.setState(Switched::On);
        state = State::Manual;
    }

    void manualStopDispensing() {
        peristalticPump.setState(Switched::Off);
        state = State::Idle;
    }

    void startCalibration() {
        calibrationStartMs = millis();
        peristalticPump.setState(Switched::On);
        state = State::Calibrating;
    }

    void stopCalibration() {
        /* should be called after dispensing 100mL of liquid */
        ms_per_mL = static_cast<uint16_t>((millis() - calibrationStartMs) / 100);
        peristalticPump.setState(Switched::Off);
        state = State::Idle;

//        configurationStorage.saveDosingPortCalibration(portNumber, milliSecondsPerMilliLiter);
    }

    void setup() override {

        /* read calibration and dosingTasksList from configurationStorage */
        ms_per_mL = configurationStorage.readDosingPortCalibration(portNumber, ms_per_mL);
        ms_per_mL = static_cast<uint16_t>((ms_per_mL == 0) ? 1000 : ms_per_mL);
        schedule = configurationStorage.readDosingPortSchedule(portNumber, schedule);
    }

    void reset() {
        setup();
    }

//    void loop(bool minuteHeartbeat) {
    void loop() override {
        //
        switch (state) {
            //
            case State::Idle:
//                if (minuteHeartbeat) {
                if (second() < 3) {
                    taskDurationMs = milliLitersToMillis(dosingTasksList.getPendingDoseMilliLiters());
                    if (taskDurationMs > 0) {
                        //
                        taskStartMs = millis();
                        peristalticPump.setState(Switched::On);
                        state = State::Dispensing;
                    }
                }

                break;

            case State::Dispensing:
                if ((millis() - taskStartMs) > taskDurationMs) {
                    //
                    taskDurationMs = 0;
                    peristalticPump.setState(Switched::Off);
                    state = State::Idle;
                }

                break;

            case State::Calibrating:
            case State::Manual:
            case State::Invalid:
            default:
                break;
        }
    }
};

#endif