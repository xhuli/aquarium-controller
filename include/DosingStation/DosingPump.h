#ifndef __DOSING_PUMP_H__
#define __DOSING_PUMP_H__

#ifdef __TEST_MODE__

#include "MockMillis.h"
#include "MockCommon.h"

#endif


#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingSchedule.h"
#include "Storage/Storage.h"

extern Storage storage;

typedef Adafruit_MotorShield MotorShield;
typedef Adafruit_DCMotor DcMotor;

/*!
    To calibrate the dosing pump, call `startCalibration()`, 
    wait for the pump to dispense 100 mL, and call `stopCalibration()`.
     - min dose is 0.25 mL
     - max dose is 255.75 mL
     - dose increments by 0.25 mL. 
*/
class DosingPump {
private:
    MotorShield motorShield;
    uint8_t motorPort;
    uint8_t dosingPumpNumber;
    DcMotor *dosingPumpPointer = nullptr;
    DosingSchedule dosingSchedule = DosingSchedule();

    uint32_t taskStartMillis = 0;
    uint32_t taskDurationMillis = 0;
    uint32_t calibrationStartMillis = 0;
    uint16_t millisPerMilliLiter = 0;

    enum DosingPumpState {
        IDLE = 0,
        DOSING = 1,
        CALIBRATING = 2,
        MANUAL = 3
    } state = IDLE;

    uint16_t milliLitersToMillis(float milliLiters) {
        return static_cast<uint16_t>(millisPerMilliLiter * milliLiters);
    }

    void startPumping() {
        /* change wires polarity to reverse direction, or use BACKWARD */
        dosingPumpPointer->run(FORWARD);
    }

    void stopPumping() {
        dosingPumpPointer->run(RELEASE);
    }

public:
    DosingPump(
            MotorShield attachToShield,
            uint8_t attachToMotorPort,
            uint8_t attachToNumber) : motorShield(attachToShield),
                                      motorPort(attachToMotorPort),
                                      dosingPumpNumber(attachToNumber) {
    }

    ~DosingPump() {
        delete dosingPumpPointer;
    }

    void addDosingTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMilliLiters,
                       uint8_t doseMilliLitersFraction) {
        if (dosingSchedule.addTask(dayOfWeek, startHour, startMinute, doseMilliLiters, doseMilliLitersFraction)) {
            storage.saveDosingPumpSchedule(dosingPumpNumber, dosingSchedule);
        }
    }

    void updateTask(uint8_t index, uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMilliLiters,
                    uint8_t doseMilliLitersFraction) {
        if (dosingSchedule.updateTask(index, dayOfWeek, startHour, startMinute, doseMilliLiters,
                                      doseMilliLitersFraction)) {
            storage.saveDosingPumpSchedule(dosingPumpNumber, dosingSchedule);
        }
    }

    void removeTask(uint8_t index) {
        dosingSchedule.removeTask(index);
        storage.saveDosingPumpSchedule(dosingPumpNumber, dosingSchedule);
    }

    void setSpeed(uint8_t speed) {
        dosingPumpPointer->setSpeed(speed);
    }

    void manualStartPumping() {
        state = DosingPumpState::MANUAL;
        startPumping();
    }

    void manualStopPumping() {
        stopDosing();
    }

    void startCalibration() {
        calibrationStartMillis = millis();
        state = DosingPumpState::CALIBRATING;
        startPumping();
    }

    void stopCalibration() {
        /* should be called after the pump has dispensed 100mL */
        millisPerMilliLiter = static_cast<uint16_t>((millis() - calibrationStartMillis) / 100);
        state = DosingPumpState::IDLE;
        stopPumping();

        storage.saveDosingPumpCalibration(dosingPumpNumber, millisPerMilliLiter);
    }

    void startDosing(uint32_t currentMillis) {
        taskStartMillis = currentMillis;
        state = DosingPumpState::DOSING;
        startPumping();
    }

    void stopDosing() {
        state = DosingPumpState::IDLE;
        stopPumping();
    }

    void setup() {
        dosingPumpPointer = motorShield.getMotor(motorPort);
        delay(100);

        setSpeed(240);

        /* read dosing pump calibration and schedule from storage */
        millisPerMilliLiter = storage.readDosingPumpCalibration(dosingPumpNumber, millisPerMilliLiter);
        millisPerMilliLiter = static_cast<uint16_t>((millisPerMilliLiter == 0) ? 1000 : millisPerMilliLiter);
        dosingSchedule = storage.readDosingPumpSchedule(dosingPumpNumber, dosingSchedule);

        state = DosingPumpState::IDLE;
    }

    void update(bool minuteHeartbeat, uint32_t currentMillis) {
        //
        switch (state) {
            //
            case DosingPumpState::IDLE:
                if (minuteHeartbeat) {
                    taskDurationMillis = milliLitersToMillis(dosingSchedule.getPendingDoseMilliLiters());
                    if (taskDurationMillis > 0) {
                        startDosing(currentMillis);
                    }
                }

                break;

            case DosingPumpState::DOSING:
                if ((currentMillis - taskStartMillis) > taskDurationMillis) {
                    stopDosing();
                }

                break;

            case DosingPumpState::CALIBRATING:
            case DosingPumpState::MANUAL:
            default:
                break;
        }
    }
};

#endif