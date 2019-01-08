#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include <Arduino.h>
#include <LinkedList.h>                          // https://github.com/ivanseidel/LinkedList
#include <Time.h>                                // standard Arduino time library
#include <Wire.h>                                // standard Arduino i2c library
#include <avr/wdt.h>                             // Arduino watchdog library
#include <stdint.h>                              // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include "DS3232RTC.h"                           // https://github.com/JChristensen/DS3232RTC
#include "utility/Adafruit_MS_PWMServoDriver.h"  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

#define SERIAL_SPEED = 9600;

typedef Adafruit_MotorShield MotorShield;
typedef Adafruit_DCMotor DcMotor;

typedef struct {
    uint8_t startHour;
    uint8_t startMinute;
    uint8_t doseMiliLiters;
} DosingTask;

class DosingSchedule {
    LinkedList<DosingTask*> dosingSchedule;  // keep a list of dosing task pointers

    bool isValidHourMinute(uint8_t hour, uint8_t minute) {
        if ((hour < 24) && (minute < 60)) {
            return true;
        }
        return false;
    }

   public:
    DosingSchedule() {
        dosingSchedule = LinkedList<DosingTask*>();
    }

    uint8_t size() {
        return dosingSchedule.size();
    }

    DosingTask* getTaskAtIndex(uint8_t index) {
        return dosingSchedule.get(index);
    }

    DosingTask* getTaskAtHourMinute(uint8_t hour, uint8_t minute) {
        // returns the first match, will never return any duplicates if they exist

        DosingTask* dosingTaskPointer;

        for (int i = 0; i < dosingSchedule.size(); i++) {
            if ((dosingTaskPointer->startHour == hour) && (dosingTaskPointer->startMinute == minute)) {
                return dosingTaskPointer;
            }
        }
        return nullptr;
    }

    bool addTask(uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        // add new task at the end of the list

        if (doseMiliLiters < 1) return false;

        // check startHour and startMinute are valid
        // check there is no other dosing task at the same time
        if (isValidHourMinute(startHour, startMinute) && (getTaskAtHourMinute(startHour, startMinute) == nullptr)) {
            DosingTask* dosingTaskPointer = new DosingTask();
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            dosingSchedule.add(dosingTaskPointer);
            return true;
        }
        return false;
    }

    bool updateTask(uint8_t index, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        // update existing task at index

        if (doseMiliLiters < 1) return false;

        DosingTask* dosingTaskPointer = getTaskAtIndex(index);

        // check the index is valid, not NULL
        // check startHour and startMinute are valid
        // check there is no other dosing task at the same time
        if ((dosingTaskPointer != nullptr) && isValidHourMinute(startHour, startMinute) && (getTaskAtHourMinute(startHour, startMinute) == nullptr)) {
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            return true;
        }
        return false;
    }

    void removeTask(uint8_t index) {
        // remove task from schedule 

        DosingTask* dosingTaskPointer = dosingSchedule.remove(index);
        delete dosingTaskPointer;
    }
};

/*!
    @brief To calibrate the dosing pump, call `startCalibration()`, 
    wait for the pump to dispense 100 mL, and call `stopCalibration()`.
     - min dose is 1 mL
     - max dose is 255 mL
     - dose increments by 1mL. 
*/
class DosingPump {
    MotorShield motorShiled;
    uint8_t motorPort;
    DcMotor* dosingPumpPointer;
    DosingSchedule dosingSchedule;

    unsigned long startPumpingMilis;  // used in the `loop()` method
    unsigned long startCalibrationMilis;
    unsigned long dosingPeriodMilis;
    // default value, should be overwirten by calibration
    // must persist calibrated value to eeprom
    unsigned long milisPerMiliLiter = 1000;

    DosingTask* dosingTaskPointer;  // used in the `loop()` method

    enum State {
        IDLE = 0,
        PUMPING = 1,
        CALIBRATING = 2
    } state;

    unsigned long miliLitersToMilis(uint8_t miliLiters) {
        return milisPerMiliLiter * miliLiters;
    }

   public:
    DosingPump(Adafruit_MotorShield attachToShield, uint8_t attachToMotorPort) : motorShiled(attachToShield),
                                                                                 motorPort(attachToMotorPort) {
    }

    ~DosingPump() {
        delete dosingPumpPointer;
        delete dosingTaskPointer;
    }

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
                dosingTaskPointer = dosingSchedule.getTaskAtHourMinute(hour(), minute());
                if (dosingTaskPointer != nullptr) {
                    dosingPeriodMilis = miliLitersToMilis(dosingTaskPointer->doseMiliLiters);
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

class DosingStation {
    unsigned long startSleepMilis;
    unsigned long sleepPeriodMilis;

    enum ShieldPort {
        M1 = 1,
        M2 = 2,
        M3 = 3,
        M4 = 4
    };

    enum State {
        SLEEPING = false,
        ACTIVE = true,
    } state;

    MotorShield motorShiled01 = MotorShield();  // default address is 0x60
    /* 
        Check the link below to learn how te set the shield address
        https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino?view=all#addressing-the-shields-13-2
    */
    // MotorShield motorShiled02 = MotorShield(0x61);
    // MotorShield motorShiled03 = MotorShield(0x62);

    DosingPump dosingPump01 = DosingPump(motorShiled01, M1);
    DosingPump dosingPump02 = DosingPump(motorShiled01, M2);
    DosingPump dosingPump03 = DosingPump(motorShiled01, M3);
    DosingPump dosingPump04 = DosingPump(motorShiled01, M4);

    // DosingPump dosingPump05 = DosingPump(motorShiled02, M1);
    // DosingPump dosingPump06 = DosingPump(motorShiled02, M2);
    // DosingPump dosingPump07 = DosingPump(motorShiled02, M3);
    // DosingPump dosingPump08 = DosingPump(motorShiled02, M4);

   public:
    void sleep(uint8_t sleepMinutes) {
        state = SLEEPING;
        sleepPeriodMilis = ((unsigned long)sleepMinutes) * 60UL * 1000UL;
        startSleepMilis = millis();
    }

    void setup() {
        state = ACTIVE;
        motorShiled01.begin();
        dosingPump01.setup();
        dosingPump02.setup();
        dosingPump03.setup();
        dosingPump04.setup();

        // motorShiled02.begin();
        // dosingPump05.setup();
        // dosingPump06.setup();
        // dosingPump07.setup();
        // dosingPump08.setup();
    }

    void loop() {
        switch (state) {
            case ACTIVE:
                dosingPump01.loop();
                dosingPump02.loop();
                dosingPump03.loop();
                dosingPump04.loop();
                break;

            case SLEEPING:
                if ((unsigned long)(millis() - startSleepMilis) > sleepPeriodMilis) {
                    state = ACTIVE;
                }
                break;
        }
    }
};

/*!
    @brief function to return the compile date and time as a time_t value
*/
time_t getCompileTime() {
    const time_t FUDGE(12);  // fudge factor to allow for upload time, etc. (seconds, YMMV)
    const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char compMon[3], *m;

    strncpy(compMon, compDate, 3);
    compMon[3] = '\0';
    m = strstr(months, compMon);

    tmElements_t tm;
    tm.Month = ((m - months) / 3 + 1);
    tm.Day = atoi(compDate + 4);
    tm.Year = atoi(compDate + 7) - 1970;
    tm.Hour = atoi(compTime);
    tm.Minute = atoi(compTime + 3);
    tm.Second = atoi(compTime + 6);

    time_t t = makeTime(tm);
    return t + FUDGE;  // add fudge factor to allow for compile time
}

DosingStation dosingStation = DosingStation();

void setup() {
    /* Open Serial Port for Debugging */
    // Serial.begin(SERIAL_SPEED);
    // while (!Serial) {
    //     // wait until Arduino Serial Monitor opens
    // }

    /* DS3232RTC */
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);

    /*
        Set the RTC time and date to the compile time and date.
        Run once to set the RTC time, and then comment out the line.
    */
    // RTC.set(getCompileTime());

    /*
        Connect/sync the Time library with the RTC
    */
    setSyncProvider(RTC.get);  // default re-sync interval is 5 minutes
    // setSyncInterval(interval);  // set the number of seconds between re-sync

    /* Watchdog */
    wdt_enable(WDTO_4S);  // set the watchdog time out

    /* Dosing Station */
    dosingStation.setup();
}

void loop() {
    wdt_reset();  // reset watchdog so it won't "bite" - reset the board

    dosingStation.loop();
}
