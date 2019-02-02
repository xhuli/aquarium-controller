#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <random>


#include "DosingStation/DosingPort.h"

#include "../_Mocks/MockStorage.h"
#include "../_Mocks/MockLiquidDispenser.h"

MockLiquidDispenser *mockPeristalticPumpPointer = new MockLiquidDispenser();
MockStorage *mockStoragePointer = new MockStorage();

DosingPort dosingPort = DosingPort(1, mockStoragePointer);

void afterTest() {
    currentMillis += 9000000;
    dosingPort.update(false, currentMillis);
    while (dosingPort.schedule.size() > 0) {
        dosingPort.schedule.removeTask(0);
    }
    assert(dosingPort.schedule.size() == 0);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(mockPeristalticPumpPointer->getIsNotDispensing());
}

static void should_StartDispensing_on_ManualStartDispensing() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    currentMillis += 1;
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);

    // when
    dosingPort.manualStartDispensing();

    // then
    assert(mockPeristalticPumpPointer->getIsDispensing());
    assert(dosingPort.getCurrentState() == DosingPort::State::MANUAL);

    // reset
    dosingPort.manualStopDispensing();
    afterTest();

    std::cout << "pass -> should_StartDispensing_on_ManualStartDispensing" << std::endl;
}

static void should_StopDispensing_on_ManualStopDispensing() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    currentMillis += 1;
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    dosingPort.manualStartDispensing();
    assert(mockPeristalticPumpPointer->getIsDispensing());

    // when
    dosingPort.manualStopDispensing();

    // then
    assert(mockPeristalticPumpPointer->getIsNotDispensing());

    // reset
    afterTest();

    std::cout << "pass -> should_StopDispensing_on_ManualStopDispensing" << std::endl;
}

static void should_CalibrateDosingPort() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);

    // when
    dosingPort.startCalibration();
    assert(mockPeristalticPumpPointer->getIsDispensing());
    assert(dosingPort.getCurrentState() == DosingPort::State::CALIBRATING);
    currentMillis = 200ul * 1000; // assume 100 mL in 200 seconds (200.000 milliseconds)
    dosingPort.update(false, currentMillis);
    dosingPort.stopCalibration();

    // then
    assert(mockPeristalticPumpPointer->getIsNotDispensing());
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.getMillisPerMilliLiter() == 2000);

    // reset
    afterTest();

    std::cout << "pass -> should_CalibrateDosingPort" << std::endl;
}

static void should_AddTask() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 27, 61, 2); // dose (61 + 0.25 * 2)mL at Friday 13:27

    // then
    assert(dosingPort.schedule.size() == 1);
    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == uint8_t(DayOfWeek::Friday));
    assert(dosingPort.schedule.getTaskAtIndex(0)->startHour == 13);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == 27);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters == 61);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction == 2);

    // reset
    afterTest();

    std::cout << "pass -> should_AddTask" << std::endl;
}

static void should_AutoSortTasks_By_DayHourMinute() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 57, 1, 0);
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 10, 58, 1, 0);
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Wednesday), 11, 14, 1, 0);
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Any), 23, 15, 1, 0);
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 59, 1, 0);

    // then
    assert(dosingPort.schedule.size() == 5);
    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == uint8_t(DayOfWeek::Any));
    assert(dosingPort.schedule.getTaskAtIndex(1)->dayOfWeek == uint8_t(DayOfWeek::Wednesday));
    assert(dosingPort.schedule.getTaskAtIndex(2)->startHour == 10);
    assert(dosingPort.schedule.getTaskAtIndex(3)->startMinute == 57);
    assert(dosingPort.schedule.getTaskAtIndex(4)->startMinute == 59);

    // reset
    afterTest();

    std::cout << "pass -> should_AutoSortTasks_By_DayHourMinute" << std::endl;
}

static void should_RemoveTaskByIndex_and_AutoSort() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 28, 64, 2); // dose (64 + 0.25 * 2)mL at Friday 13:28
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 22, 63, 3); // dose (63 + 0.25 * 3)mL at Friday 13:22
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 26, 63, 3); // dose (63 + 0.25 * 3)mL at Friday 13:26
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 27, 63, 3); // dose (63 + 0.25 * 3)mL at Friday 13:27
    assert(dosingPort.schedule.size() == 4);

    // then
    dosingPort.schedule.removeTask(1);
    assert(dosingPort.schedule.size() == 3);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == 22);
    assert(dosingPort.schedule.getTaskAtIndex(1)->startMinute == 27);
    assert(dosingPort.schedule.getTaskAtIndex(2)->startMinute == 28);

    // reset
    afterTest();

    std::cout << "pass -> should_RemoveTaskByIndex_and_AutoSort" << std::endl;
}

static void should_UpdateTaskByIndex_and_AutoSort() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 27, 70, 3); // dose (70 + 0.25 * 3)mL at Friday 13:27
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 13, 28, 61, 3); // dose (61 + 0.25 * 3)mL at Friday 13:28
    dosingPort.schedule.addTask(uint8_t(DayOfWeek::Friday), 14, 29, 61, 2); // dose (61 + 0.25 * 2)mL at Friday 13:29

    // when
    dosingPort.schedule.updateTask(1, uint8_t(DayOfWeek::Monday), 16, 55, 80, 1);

    // then
    assert(dosingPort.schedule.size() == 3);

//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->startHour: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startHour) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->startMinute: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startMinute) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction) << std::endl;

    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == uint8_t(DayOfWeek::Monday));
    assert(dosingPort.schedule.getTaskAtIndex(0)->startHour == 16);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == 55);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters == 80);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction == 1);

    assert(dosingPort.schedule.getTaskAtIndex(1)->dayOfWeek == uint8_t(DayOfWeek::Friday));
    assert(dosingPort.schedule.getTaskAtIndex(1)->startHour == 13);
    assert(dosingPort.schedule.getTaskAtIndex(1)->startMinute == 27);
    assert(dosingPort.schedule.getTaskAtIndex(1)->doseMilliLiters == 70);
    assert(dosingPort.schedule.getTaskAtIndex(1)->doseMilliLitersFraction == 3);

    assert(dosingPort.schedule.getTaskAtIndex(2)->dayOfWeek == uint8_t(DayOfWeek::Friday));
    assert(dosingPort.schedule.getTaskAtIndex(2)->startHour == 14);
    assert(dosingPort.schedule.getTaskAtIndex(2)->startMinute == 29);
    assert(dosingPort.schedule.getTaskAtIndex(2)->doseMilliLiters == 61);
    assert(dosingPort.schedule.getTaskAtIndex(2)->doseMilliLitersFraction == 2);

    // reset
    afterTest();

    std::cout << "pass -> should_UpdateTaskByIndex_and_AutoSort" << std::endl;
}

static void shouldNot_AddMoreThan_16_Tasks() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    uint8_t startHour = 0;
    bool shouldAddTask = true;

    while (shouldAddTask) {
        shouldAddTask = dosingPort.schedule.addTask(uint8_t(DayOfWeek::Any), startHour, 15, 1, 0);
//        std::cout << "dosingPort.schedule.size() = " << unsigned(dosingPort.schedule.size()) << "\n";

        startHour += 1;

        if (startHour > 23) { break; }
    }

    // then
    assert(dosingPort.schedule.size() == 16);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddMoreThan_16_Tasks" << std::endl;
}

static void shouldNot_AddTask_with_DayOfWeek_LessThanZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(-250, 12, 15, 4, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DayOfWeek_LessThanZero" << std::endl;
}

static void shouldNot_AddTask_with_DayOfWeek_GreaterThan_7() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(8, 12, 15, 4, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DayOfWeek_GreaterThan_7" << std::endl;
}

static void shouldNot_AddTask_with_StartHour_LessThanZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, -1, 15, 4, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_StartHour_LessThanZero" << std::endl;
}

static void shouldNot_AddTask_with_StartHour_GreaterThan_23() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 24, 15, 4, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_StartHour_GreaterThan_23" << std::endl;
}

static void shouldNot_AddTask_with_StartMinute_LessThanZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 23, -250, 4, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_StartMinute_LessThanZero" << std::endl;
}

static void shouldNot_AddTask_with_StartMinute_GreaterThan_59() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 23, 60, 4, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_StartMinute_GreaterThan_59" << std::endl;
}

static void shouldNot_AddTask_with_DoseMilliLiters_LessThanZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, -10, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DoseMilliLiters_LessThanZero" << std::endl;
}

static void shouldNot_AddTask_with_DoseMilliLiters_GreaterThan_255() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, 288, 1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DoseMilliLiters_GreaterThan_255" << std::endl;
}

static void shouldNot_AddTask_with_DoseMilliLitersFraction_LessThanZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, 4, -1);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DoseMilliLitersFraction_LessThanZero" << std::endl;
}

static void shouldNot_AddTask_with_DoseMilliLitersFraction_GreaterThan_3() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 23, 59, 4, 254);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DoseMilliLitersFraction_GreaterThan_3" << std::endl;
}

static void shouldNot_AddTask_with_DoseMilliLiters_EqualToZero_and_DoseMilliLitersFraction_EqualToZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 12, 30, 0, 0);

    // then
    assert(!outcome);
    assert(dosingPort.schedule.size() == 0);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_AddTask_with_DoseMilliLiters_EqualToZero_and_DoseMilliLitersFraction_EqualToZero" << std::endl;
}

static void should_AddTask_with_DoseMilliLiters_EqualToZero_and_DoseMilliLitersFraction_GreaterThanZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    bool outcome = dosingPort.schedule.addTask(4, 12, 30, 0, 1);

    // then
    assert(outcome);
    assert(dosingPort.schedule.size() == 1);

    // reset
    afterTest();

    std::cout << "pass -> should_AddTask_with_DoseMilliLiters_EqualToZero_and_DoseMilliLitersFraction_GreaterThanZero" << std::endl;
}

static void should_AddTask_asPointer() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);

    // when
    auto *testDosingTaskPointer = new DosingTask();
    testDosingTaskPointer->dayOfWeek = 7; // value > 7 will overflow
    testDosingTaskPointer->startHour = 23; // value > 23 will CRASH the program
    testDosingTaskPointer->startMinute = 59; // value > 59 will CRASH the program
    testDosingTaskPointer->doseMilliLiters = 255; // value > 255 will overflow
    testDosingTaskPointer->doseMilliLitersFraction = 3; // value > 3 will overflow

    bool outcome = dosingPort.schedule.addTask(testDosingTaskPointer);

//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->startHour: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startHour) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->startMinute: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->startMinute) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters) << std::endl;
//    std::cout << "dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction: " << unsigned(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction) << std::endl;

    // then
    assert(outcome);
    assert(dosingPort.schedule.size() == 1);
    assert(dosingPort.schedule.getTaskAtIndex(0)->dayOfWeek == testDosingTaskPointer->dayOfWeek);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startHour == testDosingTaskPointer->startHour);
    assert(dosingPort.schedule.getTaskAtIndex(0)->startMinute == testDosingTaskPointer->startMinute);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLiters == testDosingTaskPointer->doseMilliLiters);
    assert(dosingPort.schedule.getTaskAtIndex(0)->doseMilliLitersFraction == testDosingTaskPointer->doseMilliLitersFraction);

    // reset
    afterTest();

    std::cout << "pass -> should_AddTask_asPointer" << std::endl;
}

static void should_StartDispensing_when_CurrentTime_MatchesScheduleDayAndTime_and_MinuteHeartbeatTrue() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    assert(dosingPort.schedule.size() == 2);

    // when
    currentMillis += 1;
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;
    dosingPort.update(true, currentMillis);

    // then
    assert(dosingPort.schedule.getPendingDoseMilliLiters() == 4.5f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 6.25f);
    assert(dosingPort.getCurrentState() == DosingPort::State::DISPENSING);
    assert(mockPeristalticPumpPointer->getIsDispensing());

    // reset
    afterTest();

    std::cout << "pass -> should_StartDispensing_when_CurrentTime_MatchesScheduleDayAndTime_and_MinuteHeartbeatTrue" << std::endl;
}

static void should_StopDispensing_after_DosingTheScheduledDose() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    assert(dosingPort.schedule.size() == 2);

    // when
    currentMillis += 1;
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;

    dosingPort.update(true, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::DISPENSING);

    currentMillis += uint32_t(4.5f * dosingPort.getMillisPerMilliLiter());
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::DISPENSING);

    currentMillis += 1;
    dosingPort.update(false, currentMillis);

    // then
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(mockPeristalticPumpPointer->getIsNotDispensing());

    // reset
    afterTest();

    std::cout << "pass -> should_StopDispensing_after_DosingTheScheduledDose" << std::endl;
}

static void should_DispenseEveryDay_when_ScheduleDayEqualsZero() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(0, 12, 0, 4, 2);
    assert(dosingPort.schedule.size() == 1);

    // when
    currentMillis += 1;
    currentDayOfWeek = 6;
    currentHour = 12;
    currentMinute = 0;
    dosingPort.update(true, currentMillis);

    // then
    assert(dosingPort.getCurrentState() == DosingPort::State::DISPENSING);
    assert(mockPeristalticPumpPointer->getIsDispensing());

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_StartDispensing_when_CurrentTime_NotMatchesScheduleDayAndTime_and_MinuteHeartbeatTrue" << std::endl;
}

static void shouldNot_StartNewTask_when_PreviousTaskNotFinished() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(2, 12, 0, 255, 2);
    dosingPort.schedule.addTask(2, 12, 1, 6, 1);
    assert(dosingPort.schedule.size() == 2);

    // when
    currentMillis += 1;
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;

    dosingPort.update(true, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::DISPENSING);

    uint32_t firstTaskTotalDurationMillis = dosingPort.getCurrentTaskTotalDurationMillis();

    currentMillis += uint32_t(4.5f * dosingPort.getMillisPerMilliLiter()) - 1000;
    currentMinute += 1;

    dosingPort.update(true, currentMillis);

    // then
    assert(dosingPort.getCurrentState() == DosingPort::State::DISPENSING);
    assert(dosingPort.getCurrentTaskTotalDurationMillis() == firstTaskTotalDurationMillis);
    assert(mockPeristalticPumpPointer->getIsDispensing());

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_StartNewTask_when_PreviousTaskNotFinished" << std::endl;
}

static void shouldNot_StartDispensing_when_CurrentTime_MatchesScheduleDayAndTime_and_MinuteHeartbeatFalse() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    assert(dosingPort.schedule.size() == 2);

    // when
    currentMillis += 1;
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 0;
    dosingPort.update(false, currentMillis);

    // then
    assert(dosingPort.schedule.getPendingDoseMilliLiters() == 4.5f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 6.25f);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(mockPeristalticPumpPointer->getIsNotDispensing());

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_StartDispensing_when_CurrentTime_MatchesScheduleDayAndTime_and_MinuteHeartbeatFalse" << std::endl;
}

static void shouldNot_StartDispensing_when_CurrentTime_NotMatchesScheduleDayAndTime_and_MinuteHeartbeatTrue() {
    // given
    currentMillis = 0;
    mockPeristalticPumpPointer->stopDispensing();
    dosingPort.attachPeristalticPump(mockPeristalticPumpPointer);
    dosingPort.setup();
    dosingPort.update(false, currentMillis);
    assert(dosingPort.getCurrentState() == DosingPort::State::IDLE);
    assert(dosingPort.schedule.size() == 0);
    dosingPort.schedule.addTask(2, 12, 0, 4, 2);
    dosingPort.schedule.addTask(2, 12, 15, 6, 1);
    assert(dosingPort.schedule.size() == 2);

    // when
    currentMillis += 1;
    currentDayOfWeek = 2;
    currentHour = 12;
    currentMinute = 14;
    dosingPort.update(true, currentMillis);

    // then
    assert(dosingPort.schedule.getPendingDoseMilliLiters() == 0.0f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 4.5f);
    assert(dosingPort.schedule.getPendingDoseMilliLiters() != 6.25f);

    // reset
    afterTest();

    std::cout << "pass -> shouldNot_StartDispensing_when_CurrentTime_NotMatchesScheduleDayAndTime_and_MinuteHeartbeatTrue" << std::endl;
}

int main() {

    should_StartDispensing_on_ManualStartDispensing();
    should_StopDispensing_on_ManualStopDispensing();
    should_CalibrateDosingPort();
    should_AddTask();
    should_AutoSortTasks_By_DayHourMinute();
    should_RemoveTaskByIndex_and_AutoSort();
    should_UpdateTaskByIndex_and_AutoSort();
    shouldNot_AddMoreThan_16_Tasks();
    shouldNot_AddTask_with_DayOfWeek_LessThanZero();
    shouldNot_AddTask_with_DayOfWeek_GreaterThan_7();
    shouldNot_AddTask_with_StartHour_LessThanZero();
    shouldNot_AddTask_with_StartHour_GreaterThan_23();
    shouldNot_AddTask_with_StartMinute_LessThanZero();
    shouldNot_AddTask_with_StartMinute_GreaterThan_59();
    shouldNot_AddTask_with_DoseMilliLiters_LessThanZero();
    shouldNot_AddTask_with_DoseMilliLiters_GreaterThan_255();
    shouldNot_AddTask_with_DoseMilliLitersFraction_LessThanZero();
    shouldNot_AddTask_with_DoseMilliLitersFraction_GreaterThan_3();
    shouldNot_AddTask_with_DoseMilliLiters_EqualToZero_and_DoseMilliLitersFraction_EqualToZero();
    should_AddTask_with_DoseMilliLiters_EqualToZero_and_DoseMilliLitersFraction_GreaterThanZero();
    should_AddTask_asPointer();
    should_StartDispensing_when_CurrentTime_MatchesScheduleDayAndTime_and_MinuteHeartbeatTrue();
    should_StopDispensing_after_DosingTheScheduledDose();
    should_DispenseEveryDay_when_ScheduleDayEqualsZero();
    shouldNot_StartNewTask_when_PreviousTaskNotFinished();
    shouldNot_StartDispensing_when_CurrentTime_MatchesScheduleDayAndTime_and_MinuteHeartbeatFalse();
    shouldNot_StartDispensing_when_CurrentTime_NotMatchesScheduleDayAndTime_and_MinuteHeartbeatTrue();

    return 0;
};
