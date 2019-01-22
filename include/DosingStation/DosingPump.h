#ifndef __DOSING_PUMP_H__
#define __DOSING_PUMP_H__

#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingSchedule.h"
#include "Storage/Storage.h"

extern Storage storage;

typedef Adafruit_MotorShield MotorShield;
typedef Adafruit_DCMotor DcMotor;

/*!
    @brief To calibrate the dosing pump, call `startCalibration()`, 
    wait for the pump to dispense 100 mL, and call `stopCalibration()`.
     - min dose is 0.25 mL
     - max dose is 255.75 mL
     - dose increments by 0.25 mL. 
*/
class DosingPump {
   private:
    MotorShield motorShiled;
    uint8_t motorPort;
    uint8_t dosingPumpNumber;
    DcMotor* dosingPumpPointer;
    DosingSchedule dosingSchedule = DosingSchedule();

    uint32_t startDosingMilis;
    uint32_t startCalibrationMilis;
    uint32_t dosingPeriodMilis;
    uint16_t milisPerMiliLiter;

    enum class DosingPumpState {
        IDLE = 0,
        DOSING = 1,
        CALIBRATING = 2,
        MANUAL = 3
    } state;

    uint16_t miliLitersToMilis(float miliLiters) {
        return milisPerMiliLiter * miliLiters;
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
        uint8_t attachToNumber) : motorShiled(attachToShield),
                                   motorPort(attachToMotorPort),
                                   dosingPumpNumber(attachToNumber) {
    }

    ~DosingPump() {
        delete dosingPumpPointer;
    }

    void addDosingTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters, uint8_t doseMiliLitersFraction) {
        if (dosingSchedule.addTask(dayOfWeek, startHour, startMinute, doseMiliLiters, doseMiliLitersFraction)) {
            storage.saveDosingPumpSchedule(dosingPumpNumber, dosingSchedule);
        }
    }

    void updateTask(uint8_t index, uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters, uint8_t doseMiliLitersFraction) {
        if (dosingSchedule.updateTask(index, dayOfWeek, startHour, startMinute, doseMiliLiters, doseMiliLitersFraction)) {
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

    void
    manualStartPumping() {
        state = DosingPumpState::MANUAL;
        startPumping();
    }

    void manualStopPumping() {
        stopDosing();
    }

    void startCalibration() {
        startCalibrationMilis = millis();
        state = DosingPumpState::CALIBRATING;
        startPumping();
    }

    void stopCalibration() {
        /* should be called after the pump has dispensed 100mL */
        milisPerMiliLiter = (millis() - startCalibrationMilis) / 100;
        state = DosingPumpState::IDLE;
        stopPumping();

        storage.saveDosingPumpCalibration(dosingPumpNumber, milisPerMiliLiter);
    }

    void startDosing(uint32_t currentMillis) {
        startDosingMilis = currentMillis;
        state = DosingPumpState::DOSING;
        startPumping();
    }

    void stopDosing() {
        state = DosingPumpState::IDLE;
        stopPumping();
    }

    void setup() {
        dosingPumpPointer = motorShiled.getMotor(motorPort);
        delay(100);

        setSpeed(240);

        /* read dosing pump calibration and schedule from storage */
        milisPerMiliLiter = storage.readDosingPumpCalibration(dosingPumpNumber, milisPerMiliLiter);
        milisPerMiliLiter = (milisPerMiliLiter == 0) ? 1000 : milisPerMiliLiter;
        dosingSchedule = storage.readDosingPumpSchedule(dosingPumpNumber, dosingSchedule);

        state = DosingPumpState::IDLE;
    }

    void update(bool minuteHeartbeat, uint32_t currentMillis) {
        //
        switch (state) {
            //
            case DosingPumpState::IDLE:
                if (minuteHeartbeat) {
                    dosingPeriodMilis = miliLitersToMilis(dosingSchedule.getPendingDoseMiliLiters());
                    if (dosingPeriodMilis != 0) {
                        startDosing(currentMillis);
                    }
                }

                break;

            case DosingPumpState::DOSING:
                if ((unsigned long)(currentMillis - startDosingMilis) > dosingPeriodMilis) {
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