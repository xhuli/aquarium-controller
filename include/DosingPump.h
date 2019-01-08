#ifndef __DOSING_PUMP_H__
#define __DOSING_PUMP_H__

#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingSchedule.h"
#include "DosingTask.h"

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
    MotorShield motorShiled;
    DcMotor* dosingPumpPointer;

    uint8_t motorPort;

    unsigned long startPumpingMilis;  // used in the `loop()` method
    unsigned long startCalibrationMilis;
    unsigned long dosingPeriodMilis;
    // default value, should be overwirten by calibration
    // must persist calibrated value to eeprom
    unsigned long milisPerMiliLiter = 1000;

    DosingTask* currentDosingTaskPointer;  // used in the `loop()` method

    enum State {
        IDLE = 0,
        PUMPING = 1,
        CALIBRATING = 2
    } state;

    unsigned long miliLitersToMilis(uint8_t miliLiters) {
        return milisPerMiliLiter * miliLiters;
    }

   public:
    DosingPump(MotorShield attachToShield, uint8_t attachToMotorPort) : motorShiled(attachToShield),
                                                                        motorPort(attachToMotorPort) {
    }

    ~DosingPump() {
        delete dosingPumpPointer;
        delete currentDosingTaskPointer;
    }

    DosingSchedule dosingSchedule;  // expose the schedule

    void setSpeed(uint8_t speed) {
        dosingPumpPointer->setSpeed(speed);
    }

    void startCalibration() {
        startCalibrationMilis = millis();
        startPumping();
        state = CALIBRATING;
    }

    void stopCalibration() {
        /* should be called after the pump has dispensed 100mL */
        milisPerMiliLiter = (unsigned long)(millis() - startCalibrationMilis) / 100;
        stopPumping();
        state = IDLE;
    }

    void startPumping() {
        /* change wires polarity to reverse direction, or use BACKWARD */
        dosingPumpPointer->run(FORWARD);
    }

    void stopPumping() {
        dosingPumpPointer->run(RELEASE);
    }

    void setup() {
        dosingPumpPointer = motorShiled.getMotor(motorPort);
        setSpeed(240);
        state = IDLE;

        // todo: load configuration (milisPerMiliLiter, DosingSchedule)
        dosingSchedule = DosingSchedule();
        dosingSchedule.addTask(12, 30, 3);  // hour, minute, doseMiliLiters
        dosingSchedule.addTask(14, 30, 3);
        dosingSchedule.addTask(16, 30, 3);
    }

    void loop() {
        switch (state) {
            case IDLE:
                currentDosingTaskPointer = dosingSchedule.getTaskAtHourMinute(hour(), minute());
                if (currentDosingTaskPointer != nullptr) {
                    dosingPeriodMilis = miliLitersToMilis(currentDosingTaskPointer->doseMiliLiters);
                    startPumpingMilis = millis();
                    startPumping();
                    state = PUMPING;
                }
                break;

            case PUMPING:
                if ((unsigned long)(millis() - startPumpingMilis) > dosingPeriodMilis) {
                    stopPumping();
                    state = IDLE;
                }
                break;

            case CALIBRATING:
                // do not mess with the calibrating methods
                break;
        }
    }
};

#endif