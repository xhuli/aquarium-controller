#ifndef __DOSING_PUMP_H__
#define __DOSING_PUMP_H__

#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingSchedule.h"

typedef Adafruit_MotorShield MotorShield;
typedef Adafruit_DCMotor DcMotor;

/*!
    @brief To calibrate the dosing pump, call `startCalibration()`, 
    wait for the pump to dispense 100 mL, and call `stopCalibration()`.
     - min dose is 1 mL
     - max dose is 255 mL
     - dose increments by 1mL. 
*/
class DosingPump {
   private:
    MotorShield motorShiled;
    uint8_t motorPort;
    uint8_t dosingPumpNumber;
    DcMotor* dosingPumpPointer;

    unsigned long startDosingMilis;  // used in the `loop()` method
    unsigned long startCalibrationMilis;
    unsigned long dosingPeriodMilis;
    // default value, should be overwirten by calibration
    // must persist calibrated value to eeprom
    unsigned long milisPerMiliLiter = 1000;

    enum State {
        IDLE = 0,
        DOSING = 1,
        CALIBRATING = 2,
        MANUAL = 3
    } state;

    unsigned long miliLitersToMilis(uint8_t miliLiters) {
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

    DosingSchedule dosingSchedule;  // expose the schedule

    void setSpeed(uint8_t speed) {
        dosingPumpPointer->setSpeed(speed);
    }

    void manualStartPumping() {
        state = MANUAL;
        startPumping();
    }

    void manualStopPumping() {
        stopDosing();
    }

    void startCalibration() {
        startCalibrationMilis = millis();
        state = CALIBRATING;
        startPumping();
    }

    void stopCalibration() {
        /* should be called after the pump has dispensed 100mL */
        milisPerMiliLiter = (unsigned long)(millis() - startCalibrationMilis) / 100;
        state = IDLE;
        stopPumping();

        // todo: save calibration
    }

    void startDosing(uint64_t currentMillis) {
        startDosingMilis = currentMillis;
        state = DOSING;
        startPumping();
    }

    void stopDosing() {
        state = IDLE;
        stopPumping();
    }

    void setup() {
        dosingPumpPointer = motorShiled.getMotor(motorPort);
        setSpeed(240);
        state = IDLE;

        // todo: load configuration (milisPerMiliLiter, DosingSchedule)
        dosingSchedule = DosingSchedule();
        dosingSchedule.addTask(0, 12, 30, 3);  // dayOfWeek, hour, minute, doseMiliLiters
        dosingSchedule.addTask(0, 14, 30, 6);
        dosingSchedule.addTask(0, 16, 30, 3);
    }

    void update(bool minuteHeartbeat, uint64_t currentMillis) {
        switch (state) {
            case IDLE:
                if (minuteHeartbeat) {
                    dosingPeriodMilis = miliLitersToMilis(dosingSchedule.getPendingDoseMiliLiters());
                    if (dosingPeriodMilis != 0) {
                        startDosing(currentMillis);
                    }
                }

                break;

            case DOSING:
                if ((unsigned long)(currentMillis - startDosingMilis) > dosingPeriodMilis) {
                    stopDosing();
                }

                break;

            case CALIBRATING:
            case MANUAL:
                // externally controlled via stop methods
                break;
        }
    }
};

#endif