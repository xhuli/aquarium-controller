#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <random>
#include <Abstract/AbstractRunnable.h>

#include "DosingStation/DosingPort.h"

#include "../_Mocks/Switchable.h"

static void setup() {
    AbstractRunnable::setupAll();
    ++currentMillis;
}

static void loop() {
    AbstractRunnable::loopAll();
    ++currentMillis;
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}


static void should_StartDispensingOnManualStartDispensing() {
    /* given */

    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    assert(dosingPort.getState() == DosingPort::State::Idle);
    dosingPort.manualStartDispensing();

    /* then */
    assert(mockPeristalticPump.isInState(Switched::On));
    assert(dosingPort.getState() == DosingPort::State::Manual);

    std::cout << "ok -> should_StartDispensingOnManualStartDispensing\n";
}

static void should_StopDispensingOnManualStopDispensing() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.manualStartDispensing();
    dosingPort.manualStopDispensing();

    /* then */
    assert(mockPeristalticPump.isInState(Switched::Off));
    assert(dosingPort.isInState(DosingPort::State::Idle));

    std::cout << "ok -> should_StopDispensingOnManualStopDispensing\n";
}

static void should_CalibrateDosingPort() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.startCalibration();
    assert(mockPeristalticPump.isInState(Switched::On));
    assert(dosingPort.getState() == DosingPort::State::Calibrating);
    loop(200ul * 1000); // assume 100 mL in 200 seconds (200.000 milliseconds)
    dosingPort.stopCalibration();

    /* then */
    assert(mockPeristalticPump.isInState(Switched::Off));
    assert(dosingPort.getState() == DosingPort::State::Idle);
    assert(dosingPort.getMilliSecondsPerMilliLiter() == 2000);

    std::cout << "ok -> should_CalibrateDosingPort\n";
}

static void should_AddTask() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 27, 61, 2); // dose (61 + 0.25 * 2)mL at Friday 13:27

    /* then */
    assert(dosingPort.schedule.size() == 1);
    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == DayOfWeek::Friday);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startHour == 13);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == 27);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters == 61);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction == 2);

    std::cout << "ok -> should_AddTask\n";
}

static void should_AutoSortTasksByDayHourMinute() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 57, 1, 0);
    dosingPort.schedule.addTask(DayOfWeek::Friday, 10, 58, 1, 0);
    dosingPort.schedule.addTask(DayOfWeek::Wednesday, 11, 14, 1, 0);
    dosingPort.schedule.addTask(DayOfWeek::Any, 23, 15, 1, 0);
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 59, 1, 0);

    /* then */
    assert(dosingPort.schedule.size() == 5);
    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == DayOfWeek::Any);
    assert(dosingPort.schedule.getTaskAtIndex(1)->dayOfWeek == DayOfWeek::Wednesday);
    assert(dosingPort.schedule.getTaskAtIndex(2)->startHour == 10);
    assert(dosingPort.schedule.getTaskAtIndex(3)->startMinute == 57);
    assert(dosingPort.schedule.getTaskAtIndex(4)->startMinute == 59);

    std::cout << "ok -> should_AutoSortTasksByDayHourMinute\n";
}

static void should_RemoveTaskByIndexAndAutoSort() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 28, 64, 2); // dose (64 + 0.25 * 2)mL at Friday 13:28
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 22, 63, 3); // dose (63 + 0.25 * 3)mL at Friday 13:22
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 26, 63, 3); // dose (63 + 0.25 * 3)mL at Friday 13:26
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 27, 63, 3); // dose (63 + 0.25 * 3)mL at Friday 13:27

    /* then */
    assert(dosingPort.schedule.size() == 4);
    dosingPort.schedule.removeTask(1);
    assert(dosingPort.schedule.size() == 3);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == 22);
    assert(dosingPort.schedule.getTaskAtIndex(1)->startMinute == 27);
    assert(dosingPort.schedule.getTaskAtIndex(2)->startMinute == 28);

    std::cout << "ok -> should_RemoveTaskByIndexAndAutoSort\n";
}

static void should_UpdateTaskByIndexAndAutoSort() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 27, 70, 3); // dose (70 + 0.25 * 3)mL at Friday 13:27
    dosingPort.schedule.addTask(DayOfWeek::Friday, 13, 28, 61, 3); // dose (61 + 0.25 * 3)mL at Friday 13:28
    dosingPort.schedule.addTask(DayOfWeek::Friday, 14, 29, 61, 2); // dose (61 + 0.25 * 2)mL at Friday 13:29

    /* when */
    dosingPort.schedule.updateTask(1, DayOfWeek::Monday, 16, 55, 80, 1);

    /* then */
    assert(dosingPort.schedule.size() == 3);

    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->startHour: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startHour) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->startMinute: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startMinute) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction) << std::endl;

    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == DayOfWeek::Monday);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startHour == 16);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == 55);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters == 80);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction == 1);

    assert(dosingPort.schedule.getTaskAtIndex(1)->dayOfWeek == DayOfWeek::Friday);
    assert(dosingPort.schedule.getTaskAtIndex(1)->startHour == 13);
    assert(dosingPort.schedule.getTaskAtIndex(1)->startMinute == 27);
    assert(dosingPort.schedule.getTaskAtIndex(1)->doseMilliLiters == 70);
    assert(dosingPort.schedule.getTaskAtIndex(1)->doseMilliLitersFraction == 3);

    assert(dosingPort.schedule.getTaskAtIndex(2)->dayOfWeek == DayOfWeek::Friday);
    assert(dosingPort.schedule.getTaskAtIndex(2)->startHour == 14);
    assert(dosingPort.schedule.getTaskAtIndex(2)->startMinute == 29);
    assert(dosingPort.schedule.getTaskAtIndex(2)->doseMilliLiters == 61);
    assert(dosingPort.schedule.getTaskAtIndex(2)->doseMilliLitersFraction == 2);

    std::cout << "ok -> should_UpdateTaskByIndexAndAutoSort\n";
}

static void should_NotAddMoreThan_16_Tasks() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    uint8_t startHour = 0;
    bool shouldAddTask = true;

    while (shouldAddTask) {
        shouldAddTask = dosingPort.schedule.addTask(DayOfWeek::Any, startHour, 15, 1, 0);

        startHour += 1;

        if (startHour > 23) { break; }
    }

    /* then */
    assert(dosingPort.schedule.size() == 16);

    std::cout << "ok -> should_NotAddMoreThan_16_Tasks\n";
}

static void should_NotAddTaskWithDayOfWeekLessThanZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(-250, 12, 15, 4, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDayOfWeekLessThanZero\n";
}

static void should_NotAddTaskWithDayOfWeekGreaterThan_7() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(8, 12, 15, 4, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDayOfWeekGreaterThan_7\n";
}

static void should_NotAddTaskWithStartHourLessThanZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, -1, 15, 4, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithStartHourLessThanZero\n";
}

static void should_NotAddTaskWithStartHourGreaterThan_23() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 24, 15, 4, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithStartHourGreaterThan_23\n";
}

static void should_NotAddTaskWithStartMinuteLessThanZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 23, -250, 4, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithStartMinuteLessThanZero\n";
}

static void should_NotAddTaskWithStartMinuteGreaterThan_59() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 23, 60, 4, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithStartMinuteGreaterThan_59\n";
}

static void should_NotAddTaskWithDoseMilliLitersLessThanZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, -10, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDoseMilliLitersLessThanZero\n";
}

static void should_NotAddTaskWithDoseMilliLitersGreaterThan_255() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, 288, 1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDoseMilliLitersGreaterThan_255\n";
}

static void should_NotAddTaskWithDoseMilliLitersFractionLessThanZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, 4, -1);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDoseMilliLitersFractionLessThanZero\n";
}

static void should_NotAddTaskWithDoseMilliLitersFractionGreaterThan_3() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, 4, 254);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDoseMilliLitersFractionGreaterThan_3\n";
}

static void should_NotAddTaskWithDoseMilliLitersEqualToZeroAndDoseMilliLitersFractionEqualToZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 12, 30, 0, 0);

    /* then */
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    std::cout << "ok -> should_NotAddTaskWithDoseMilliLitersEqualToZeroAndDoseMilliLitersFractionEqualToZero\n";
}

static void should_AddTaskWithDoseMilliLitersEqualToZeroAndDoseMilliLitersFractionGreaterThanZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    bool outcome = dosingPort.schedule.addTask(4, 12, 30, 0, 1);

    /* then */
    assert(outcome);
    assert(dosingPort.schedule.size() == 1);

    std::cout << "ok -> should_AddTaskWithDoseMilliLitersEqualToZeroAndDoseMilliLitersFractionGreaterThanZero\n";
}

static void should_AddTaskAsPointer() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    auto *testDosingTaskPointer = new DosingTask();
    testDosingTaskPointer->dayOfWeek = 7; // value > 7 will overflow
    testDosingTaskPointer->startHour = 23; // value > 23 will CRASH the program
    testDosingTaskPointer->startMinute = 59; // value > 59 will CRASH the program
    testDosingTaskPointer->doseMilliLiters = 255; // value > 255 will overflow
    testDosingTaskPointer->doseMilliLitersFraction = 3; // value > 3 will overflow

    bool outcome = dosingPort.schedule.addTask(testDosingTaskPointer);

    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->startHour: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startHour) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->startMinute: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startMinute) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters) << std::endl;
    //    cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction) << std::endl;

    /* then */
    assert(outcome);
    assert(dosingPort.schedule.size() == 1);
    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == testDosingTaskPointer->dayOfWeek);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startHour == testDosingTaskPointer->startHour);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == testDosingTaskPointer->startMinute);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters == testDosingTaskPointer->doseMilliLiters);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction == testDosingTaskPointer->doseMilliLitersFraction);

    std::cout << "ok -> should_AddTaskAsPointer\n";

    // reset
    delete testDosingTaskPointer;
}

static void should_StartDispensingWhenCurrentTimeMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;
    loop();

    /* then */
    assert(dosingPort.schedule.getPendingDoseMilliLiters() == 4.5f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 6.25f);
    assert(dosingPort.getState() == DosingPort::State::Dispensing);
    assert(mockPeristalticPump.isInState(Switched::On));

    std::cout << "ok -> should_StartDispensingWhenCurrentTimeMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue\n";
}

static void should_StopDispensingAfterDosingTheScheduledDose() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;

    loop();
    assert(dosingPort.getState() == DosingPort::State::Dispensing);

    loop(uint32_t(4.5f * dosingPort.getMilliSecondsPerMilliLiter()));
    assert(dosingPort.getState() == DosingPort::State::Dispensing);
    loop();

    /* then */
    assert(dosingPort.getState() == DosingPort::State::Idle);
    assert(mockPeristalticPump.isInState(Switched::Off));

    std::cout << "ok -> should_StopDispensingAfterDosingTheScheduledDose\n";
}

static void should_DispenseEveryDayWhenScheduleDayEqualsZero() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.schedule.addTask(0, 12, 0, 4, 2);
    currentDayOfWeek = 6;
    currentHour = 12;
    currentMinute = 0;
    loop();

    /* then */
    assert(dosingPort.getState() == DosingPort::State::Dispensing);
    assert(mockPeristalticPump.isInState(Switched::On));

    std::cout << "ok -> should_NotStartDispensingWhenCurrentTimeNotMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue\n";
}

static void should_NotStartNewTaskWhenPreviousTaskNotFinished() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.schedule.addTask(2, 12, 0, 255, 2);
    dosingPort.schedule.addTask(2, 12, 1, 6, 1);
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;

    loop();
    assert(dosingPort.getState() == DosingPort::State::Dispensing);

    uint32_t firstTaskTotalDurationMillis = dosingPort.getCurrentTaskTotalDurationMillis();

    loop(uint32_t(4.5f * dosingPort.getMilliSecondsPerMilliLiter()) - 1000);

    /* then */
    assert(dosingPort.getState() == DosingPort::State::Dispensing);
    assert(dosingPort.getCurrentTaskTotalDurationMillis() == firstTaskTotalDurationMillis);
    assert(mockPeristalticPump.isInState(Switched::On));

    std::cout << "ok -> should_NotStartNewTaskWhenPreviousTaskNotFinished\n";
}

static void should_NotStartDispensingWhenCurrentTimeMatchesScheduleDayAndTimeAndMinuteHeartbeatFalse() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;
    loop();

    /* then */
    assert(dosingPort.schedule.getPendingDoseMilliLiters() == 4.5f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 6.25f);
    assert(dosingPort.getState() == DosingPort::State::Idle);
    assert(mockPeristalticPump.isInState(Switched::Off));

    std::cout << "ok -> should_NotStartDispensingWhenCurrentTimeMatchesScheduleDayAndTimeAndMinuteHeartbeatFalse\n";
}

static void should_NotStartDispensingWhenCurrentTimeNotMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue() {
    /* given */
    Switchable mockPeristalticPump{};
    MockStorage mockStorage{};
    DosingPort dosingPort(1, mockStorage, mockPeristalticPump);

    currentMillis = getRandomUint32();
    setup();
    loop();

    /* when */
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 14;
    loop();

    /* then */
    assert(dosingPort.schedule.getPendingDoseMilliLiters() == 0.0f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 4.5f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 6.25f);

    std::cout << "ok -> should_NotStartDispensingWhenCurrentTimeNotMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue\n";
}

int main() {

    should_StartDispensingOnManualStartDispensing();
    should_StopDispensingOnManualStopDispensing();
    should_CalibrateDosingPort();
    should_AddTask();
    should_AutoSortTasksByDayHourMinute();
    should_RemoveTaskByIndexAndAutoSort();
    should_UpdateTaskByIndexAndAutoSort();
    should_NotAddMoreThan_16_Tasks();
    should_NotAddTaskWithDayOfWeekLessThanZero();
    should_NotAddTaskWithDayOfWeekGreaterThan_7();
    should_NotAddTaskWithStartHourLessThanZero();
    should_NotAddTaskWithStartHourGreaterThan_23();
    should_NotAddTaskWithStartMinuteLessThanZero();
    should_NotAddTaskWithStartMinuteGreaterThan_59();
    should_NotAddTaskWithDoseMilliLitersLessThanZero();
    should_NotAddTaskWithDoseMilliLitersGreaterThan_255();
    should_NotAddTaskWithDoseMilliLitersFractionLessThanZero();
    should_NotAddTaskWithDoseMilliLitersFractionGreaterThan_3();
    should_NotAddTaskWithDoseMilliLitersEqualToZeroAndDoseMilliLitersFractionEqualToZero();
    should_AddTaskWithDoseMilliLitersEqualToZeroAndDoseMilliLitersFractionGreaterThanZero();
    should_AddTaskAsPointer();
    should_StartDispensingWhenCurrentTimeMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue();
    should_StopDispensingAfterDosingTheScheduledDose();
    should_DispenseEveryDayWhenScheduleDayEqualsZero();
    should_NotStartNewTaskWhenPreviousTaskNotFinished();
    should_NotStartDispensingWhenCurrentTimeMatchesScheduleDayAndTimeAndMinuteHeartbeatFalse();
    should_NotStartDispensingWhenCurrentTimeNotMatchesScheduleDayAndTimeAndMinuteHeartbeatTrue();

    return 0;
}
