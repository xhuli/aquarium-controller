#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <random>
#include <chrono>
#include "../_Mocks/MockCommon.h"
#include <Abstract/AbstractRunnable.h>
#include "Common/Switchable.h"

class Adafruit_DCMotor {
public:
    Adafruit_DCMotor() = default;

    virtual void run(uint8_t runDirection) {
        // pass
    }
};

class Adafruit_MotorShield {
private:
    uint8_t addr;
public:
    explicit Adafruit_MotorShield(uint8_t addr) : addr(addr) {}

    void begin() {}

    Adafruit_DCMotor *getMotor(uint8_t n) {
        return new Adafruit_DCMotor{};
    }

    uint8_t getAddr() const {
        return addr;
    }
};

#define FORWARD 1
#define RELEASE 4

#include <DosingStation/DosingPort.h>
#include <DosingStation/DosingStation.h>


static void setup() {
    AbstractRunnable::setupAll();
}

static void loop() {
    AbstractRunnable::loopAll();
    /* warn: currentMillis is updated implicitly by a `MocksCommon/TimeKeeper` object */
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static constexpr uint16_t defaultMilliSecondsPerMilliLiter = 2047;

/*
 * § Section: create motor shield objects with I2C addresses
 */
Adafruit_MotorShield adafruitMotorShield0(0x60);
Adafruit_MotorShield adafruitMotorShield1(0x61);

static void shouldStartDispensingOnStartCalibrating() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    assert(dosingPort.isInState(Switched::Off));

    /* when */
    dosingPort.startStopCalibrating();
    loop();

    /* then */
    assert(dosingPort.isInState(Switched::On));

    std::cout << "ok -> shouldStartDispensingOnStartCalibrating\n";
}

static void shouldStopDispensingOnStopCalibrating() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    assert(dosingPort.isInState(Switched::Off));

    /* when */
    dosingPort.startStopCalibrating();
    loop();
    dosingPort.startStopCalibrating();
    loop();

    /* then */
    assert(dosingPort.isInState(Switched::Off));

    std::cout << "ok -> shouldStopDispensingOnStopCalibrating\n";
}

static void shouldGetDefaultMsPerMilliLiterBeforeCalibration() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    assert(dosingPort.isInState(Switched::Off));

    /* when & then */
    assert(dosingPort.getMilliSecondsPerMilliLiter() == defaultMilliSecondsPerMilliLiter);

    std::cout << "ok -> shouldGetDefaultMsPerMilliLiterBeforeCalibration\n";
}

static void shouldUpdateMsPerMilliLiterAfterCalibration() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    assert(dosingPort.isInState(Switched::Off));

    /* when */
    int calibrationDurationMs = getRandomUint32();

    dosingPort.startStopCalibrating();
    loop(calibrationDurationMs);

    dosingPort.startStopCalibrating();
    loop();

    /* then */
    assert(dosingPort.getMilliSecondsPerMilliLiter() == (calibrationDurationMs / 100));

    std::cout << "ok -> shouldUpdateMsPerMilliLiterAfterCalibration\n";
}

static void shouldAddTaskToDosingPortSchedule() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 13, 30, 3, 2);
    loop();

    /* then */
    assert(dosingPort.schedule.size() == 1);

    DosingTask *pDosingTask = dosingPort.schedule.getFirstElement()->value;

    assert(pDosingTask->weekDay == DayOfWeek::Wednesday);
    assert(pDosingTask->hour == 13);
    assert(pDosingTask->minute == 30);
    assert(pDosingTask->doseMilliLiters == 3);
    assert(pDosingTask->doseMilliLiterQuarters == 2);

    std::cout << "ok -> shouldAddTaskToDosingPortSchedule\n";
}

static void shouldRemoveTaskByIndexFromDosingPortSchedule() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 13, 30, 3, 2);
    loop();
    dosingPort.schedule.remove(0);

    /* then */
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> shouldRemoveTaskByIndexFromDosingPortSchedule\n";
}

static void shouldRemoveTaskByValueFromDosingPortSchedule() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 13, 30, 3, 2);
    loop();
    DosingTask *pDosingTask = dosingPort.schedule.getFirstElement()->value;
    dosingPort.schedule.remove(pDosingTask);

    /* then */
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> shouldRemoveTaskByValueFromDosingPortSchedule\n";
}

static void shouldUpdateTaskFromDosingPortSchedule() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 13, 30, 3, 2);
    loop();

    DosingTask *pDosingTask = dosingPort.schedule.getFirstElement()->value;
    pDosingTask->weekDay = DayOfWeek::MoTuWeThFr;
    pDosingTask->hour = 12;
    pDosingTask->minute = 45;
    pDosingTask->doseMilliLiters = 6;
    pDosingTask->doseMilliLiterQuarters = 3;

    /* then */
    assert(pDosingTask->weekDay == DayOfWeek::MoTuWeThFr);
    assert(pDosingTask->hour == 12);
    assert(pDosingTask->minute == 45);
    assert(pDosingTask->doseMilliLiters == 6);
    assert(pDosingTask->doseMilliLiterQuarters == 3);

    std::cout << "ok -> shouldUpdateTaskFromDosingPortSchedule\n";
}

static void shouldNotAddTaskToPortScheduleWithInvalidHour() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, -1, 30, 3, 2);
    dosingPort.schedule.add(DayOfWeek::Wednesday, 25, 30, 3, 2);
    loop();

    /* then */
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> shouldNotAddTaskToPortScheduleWithInvalidHour\n";
}

static void shouldNotAddTaskToPortScheduleWithInvalidMinute() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 12, -1, 3, 2);
    dosingPort.schedule.add(DayOfWeek::Wednesday, 12, 61, 3, 2);
    loop();

    /* then */
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> shouldNotAddTaskToPortScheduleWithInvalidMinute\n";
}

static void shouldNotAddTaskToPortScheduleWithZeroDoseMilliLiters() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 24, 60, 0, 2);
    loop();

    /* then */
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> shouldNotAddTaskToPortScheduleWithZeroDoseMilliLiters\n";
}

static void shouldNotAddTaskToPortScheduleWithDoseMilliLitersQuarterGreaterThanThree() {
    /* given */
    DosingPort dosingPort{adafruitMotorShield0.getMotor(1), defaultMilliSecondsPerMilliLiter};
    setup();
    loop();

    /* when */
    dosingPort.schedule.add(DayOfWeek::Wednesday, 24, 60, 1, 4);
    loop();

    /* then */
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> shouldNotAddTaskToPortScheduleWithDoseMilliLitersQuarterGreaterThanThree\n";
}

static void shouldRunScheduledTaskAtTheScheduledTime() {
    /* given */
    DosingStation dosingStation{1};
    dosingStation.motorShieldsList.add(&adafruitMotorShield0);

    timeKeeper.setWeekDay(DayOfWeek::Tuesday);
    timeKeeper.setHour(22);
    timeKeeper.setMinute(29);

    setup();
    loop();

    /* when */
    uint8_t nextDoseMilliLiters = 2;
    uint8_t nextDoseMilliLiterQuarters = 2;

    DosingPort *pDosingPort = dosingStation.dosingPortsList.get(0);
    pDosingPort->schedule.add(DayOfWeek::Monday, 22, 30, 1, 1);
    pDosingPort->schedule.add(DayOfWeek::Tuesday, 22, 30, nextDoseMilliLiters, nextDoseMilliLiterQuarters);
    loop();

    assert(pDosingPort->schedule.size() == 2);
    assert(pDosingPort->isInState(Switched::Off));

    timeKeeper.setMinute(30);
    loop();

    /* then */
    assert(pDosingPort->isInState(Switched::On));
    loop(defaultMilliSecondsPerMilliLiter * (nextDoseMilliLiters + 0.25f * nextDoseMilliLiterQuarters) - 1);
    assert(pDosingPort->isInState(Switched::On));
    loop();
    assert(pDosingPort->isInState(Switched::Off));

    std::cout << "ok -> shouldRunScheduledTaskAtTheScheduledTime\n";
}

static void shouldRunScheduledTaskEveryMatchingDayAtMatchingHourAndMinuteWhenDayOfWeekIsMasked() {
    /* given */
    DosingStation dosingStation{1};
    dosingStation.motorShieldsList.add(&adafruitMotorShield0);

    setup();
    loop();

    /* when */
    uint8_t nextDoseMilliLiters = 2;
    uint8_t nextDoseMilliLiterQuarters = 2;

    DosingPort *pDosingPort = dosingStation.dosingPortsList.get(0);
    pDosingPort->schedule.add(DayOfWeek::MoWeFrSu, 22, 30, nextDoseMilliLiters, nextDoseMilliLiterQuarters);
    loop();

    assert(pDosingPort->schedule.size() == 1);
    assert(pDosingPort->isInState(Switched::Off));

    /* then */
    timeKeeper.setHour(22);
    timeKeeper.setMinute(29);
    timeKeeper.setSecond(7);
    loop();

    for (uint8_t weekDay = 1; weekDay < 8; ++weekDay) {
        timeKeeper.setWeekDay(weekDay);
        timeKeeper.setMinute(30);
        loop();

        switch (static_cast<DayOfWeek>(timeKeeper.getWeekDay())) {
            case DayOfWeek::Monday:
            case DayOfWeek::Wednesday:
            case DayOfWeek::Friday:
            case DayOfWeek::Sunday:
                assert(pDosingPort->isInState(Switched::On));
                loop(defaultMilliSecondsPerMilliLiter * (nextDoseMilliLiters + 0.25f * nextDoseMilliLiterQuarters) - 1);
                assert(pDosingPort->isInState(Switched::On));
                loop();
                assert(pDosingPort->isInState(Switched::Off));
                break;

            case DayOfWeek::Tuesday:
            case DayOfWeek::Thursday:
            case DayOfWeek::Saturday:
            default:
                assert(pDosingPort->isInState(Switched::Off));
                loop();
                break;
        }

        timeKeeper.setMinute(29);
        timeKeeper.setSecond(07);
        loop();
    }

    std::cout << "ok -> shouldRunScheduledTaskEveryMatchingDayAtMatchingHourAndMinuteWhenDayOfWeekIsMasked\n";
}

static void shouldRunScheduledTaskEveryHourAtMatchingDayAndMinuteWhenScheduledHourIsMasked() {
    /* given */
    DosingStation dosingStation{1};
    dosingStation.motorShieldsList.add(&adafruitMotorShield0);

    timeKeeper.setWeekDay(DayOfWeek::Sunday);
    timeKeeper.setHour(0);
    timeKeeper.setMinute(29);
    timeKeeper.setSecond(07);

    setup();
    loop();

    /* when */
    uint8_t nextDoseMilliLiters = 2;
    uint8_t nextDoseMilliLiterQuarters = 2;

    DosingPort *pDosingPort = dosingStation.dosingPortsList.get(0);
    pDosingPort->schedule.add(DayOfWeek::Monday, 22, 30, 1, 1);
    pDosingPort->schedule.add(DayOfWeek::MoWeFrSu, 24, 30, nextDoseMilliLiters, nextDoseMilliLiterQuarters);
    loop();

    assert(pDosingPort->schedule.size() == 2);
    assert(pDosingPort->isInState(Switched::Off));

    /* then */
    for (uint8_t hour = 0; hour < 24; ++hour) {
        timeKeeper.setHour(hour);
        timeKeeper.setMinute(30);
        loop();

        /* task is run every hour for the matching day and minute */
        assert(pDosingPort->isInState(Switched::On));
        loop(defaultMilliSecondsPerMilliLiter * (nextDoseMilliLiters + 0.25f * nextDoseMilliLiterQuarters) - 1);
        assert(pDosingPort->isInState(Switched::On));
        loop();
        assert(pDosingPort->isInState(Switched::Off));

        timeKeeper.setMinute(29);
        timeKeeper.setSecond(07);
        loop();
    }

    std::cout << "ok -> shouldRunScheduledTaskEveryHourAtMatchingDayAndMinuteWhenScheduledHourIsMasked\n";
}

static void shouldRunScheduledTaskEveryMinuteAtMatchingDayAndHourWhenScheduledMinuteIsMasked() {
    /* given */
    DosingStation dosingStation{1};
    dosingStation.motorShieldsList.add(&adafruitMotorShield0);

    timeKeeper.setWeekDay(DayOfWeek::Sunday);
    timeKeeper.setHour(17);
    timeKeeper.setMinute(59);

    setup();
    loop();

    /* when */
    uint8_t nextDoseMilliLiters = 2;
    uint8_t nextDoseMilliLiterQuarters = 2;

    DosingPort *pDosingPort = dosingStation.dosingPortsList.get(0);
    pDosingPort->schedule.add(DayOfWeek::Monday, 22, 30, 1, 1);
    pDosingPort->schedule.add(DayOfWeek::MoWeFrSu, 18, 60, nextDoseMilliLiters, nextDoseMilliLiterQuarters);
    loop();

    assert(pDosingPort->schedule.size() == 2);
    assert(pDosingPort->isInState(Switched::Off));

    loop();

    /* then */
    for (uint8_t minute = 0; minute < 60; ++minute) {
        timeKeeper.setHour(18);
        timeKeeper.setMinute(minute);
        timeKeeper.setSecond(03);
        loop();

        /* task is run every minute for the matching day and hour */
        assert(pDosingPort->isInState(Switched::On));
        loop(defaultMilliSecondsPerMilliLiter * (nextDoseMilliLiters + 0.25f * nextDoseMilliLiterQuarters) - 1);
        assert(pDosingPort->isInState(Switched::On));
        loop(10);
        assert(pDosingPort->isInState(Switched::Off));
    }

    std::cout << "ok -> shouldRunScheduledTaskEveryMinuteAtMatchingDayAndHourWhenScheduledMinuteIsMasked\n";
}

static void shouldNotRunSameTaskMoreThanOncePerMinute() {
    /* given */
    DosingStation dosingStation{1};
    dosingStation.motorShieldsList.add(&adafruitMotorShield0);

    timeKeeper.setWeekDay(DayOfWeek::Sunday);
    timeKeeper.setHour(22);
    timeKeeper.setMinute(29);
    timeKeeper.setSecond(59);

    setup();
    loop();

    /* when */
    uint8_t nextDoseMilliLiters = 2;
    uint8_t nextDoseMilliLiterQuarters = 2;

    DosingPort *pDosingPort = dosingStation.dosingPortsList.get(0);
    pDosingPort->schedule.add(DayOfWeek::Monday, 22, 30, 1, 1);
    pDosingPort->schedule.add(DayOfWeek::MoWeFrSu, 22, 30, nextDoseMilliLiters, nextDoseMilliLiterQuarters);
    loop();

    assert(pDosingPort->schedule.size() == 2);
    assert(pDosingPort->isInState(Switched::Off));

    timeKeeper.setSecond(0);
    timeKeeper.setMinute(30);
    loop();

    /* then */
    assert(pDosingPort->isInState(Switched::On));
    loop(defaultMilliSecondsPerMilliLiter * (nextDoseMilliLiters + 0.25f * nextDoseMilliLiterQuarters) - 1);
    assert(pDosingPort->isInState(Switched::On));
    loop();
    assert(pDosingPort->isInState(Switched::Off));
    loop(10);
    assert(pDosingPort->isInState(Switched::Off));

    std::cout << "ok -> shouldNotRunSameTaskMoreThanOncePerMinute\n";
}

static void shouldNotDispenseIfThereIsNoScheduledTaskOnMinuteChange() {
    /* given */
    DosingStation dosingStation{1};
    dosingStation.motorShieldsList.add(&adafruitMotorShield0);

    setup();
    loop();

    /* when */
    DosingPort *pDosingPort = dosingStation.dosingPortsList.get(0);
    pDosingPort->schedule.add(DayOfWeek::Monday, 22, 30, 1, 1);
    loop();

    assert(pDosingPort->schedule.size() == 1);
    assert(pDosingPort->isInState(Switched::Off));

    timeKeeper.setSecond(0);
    timeKeeper.setMinute(30);
    loop();

    /* then */
    assert(pDosingPort->isInState(Switched::Off));

    std::cout << "ok -> shouldNotDispenseIfThereIsNoScheduledTaskOnMinuteChange\n";
}

int main(int argc, char *argv[]) {

    std::cout << "\n"
              << "------------------------------------------------------------\n"
              << " >> TEST START\n"
              << "------------------------------------------------------------\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {


        shouldStartDispensingOnStartCalibrating();
        shouldStopDispensingOnStopCalibrating();
        shouldGetDefaultMsPerMilliLiterBeforeCalibration();
        shouldUpdateMsPerMilliLiterAfterCalibration();

        shouldAddTaskToDosingPortSchedule();
        shouldRemoveTaskByIndexFromDosingPortSchedule();
        shouldRemoveTaskByValueFromDosingPortSchedule();
        shouldUpdateTaskFromDosingPortSchedule();

        shouldNotAddTaskToPortScheduleWithInvalidHour();
        shouldNotAddTaskToPortScheduleWithInvalidMinute();
        shouldNotAddTaskToPortScheduleWithZeroDoseMilliLiters();
        shouldNotAddTaskToPortScheduleWithDoseMilliLitersQuarterGreaterThanThree();

        shouldRunScheduledTaskAtTheScheduledTime();
        shouldRunScheduledTaskEveryMatchingDayAtMatchingHourAndMinuteWhenDayOfWeekIsMasked();
        shouldRunScheduledTaskEveryHourAtMatchingDayAndMinuteWhenScheduledHourIsMasked();
        shouldRunScheduledTaskEveryMinuteAtMatchingDayAndHourWhenScheduledMinuteIsMasked();
        shouldNotRunSameTaskMoreThanOncePerMinute();

        shouldNotDispenseIfThereIsNoScheduledTaskOnMinuteChange();

        if (repeat > 1) {
            std::cout << "------------------------------------------------------------\n";
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\n"
                 "------------------------------------------------------------\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout << "------------------------------------------------------------\n"
              << " >> TEST END\n"
              << "------------------------------------------------------------\n"
              << "\n";

    return 0;
}
