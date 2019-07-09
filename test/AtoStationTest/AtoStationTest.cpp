#define __TEST_MODE__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <AtoStation/AtoStation.h>

#include "../_Mocks/MockCommon.h"
#include "../_Mocks/MockBuzzer.h"

#include "AtoStation/AtoStation.h"

#include "../_Mocks/MockLiquidDispenser.h"
#include "../_Mocks/MockLiquidLevelSensor.h"
#include "../_Mocks/MockStorage.h"

using namespace std;

auto *mockAtoDispenserPointer = new MockLiquidDispenser();
auto *mainLevelSensorPointer = new MockLiquidLevelSensor();
auto *backupHighLevelSensorPointer = new MockLiquidLevelSensor();
auto *backupLowLevelSensorPointer = new MockLiquidLevelSensor();
auto *reservoirLowLevelSensorPointer = new MockLiquidLevelSensor();

auto atoSettings = AtoSettings();


auto alarmStation = AlarmStation();

auto mockBuzzerPointer = new MockBuzzer();

static void beforeTest(AtoStation *atoStation) {
    //
    atoStation->attachAlarmStation(&alarmStation);

    currentMillis = 0;

    mainLevelSensorPointer->mockIsSensing();
    reservoirLowLevelSensorPointer->mockIsSensing();
    backupHighLevelSensorPointer->mockIsNotSensing();
    backupLowLevelSensorPointer->mockIsSensing();

    mockAtoDispenserPointer->stopDispensing();

    atoStation->setup();
    atoStation->update(currentMillis);

    currentMillis = atoSettings.minDispensingIntervalMillis + 1;

    assert(atoStation->getCurrentState() == AtoStation::State::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    while (alarmStation.getNumberOfAlarms() > 0) {
        alarmStation.deleteAlarmByIndex(0);
    }

    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void mockAtoDispenser_should_MockStartDispensing() {
    mockAtoDispenserPointer->startDispensing();
    assert(mockAtoDispenserPointer->getIsDispensing());

    cout << "pass -> mockAtoDispenser_should_MockStartDispensing\n";
}

static void mockAtoDispenser_should_MockStopDispensing() {
    mockAtoDispenserPointer->stopDispensing();
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    cout << "pass -> mockAtoDispenser_should_MockStopDispensing\n";
}

static void mockSensor_should_MockSensingLiquid() {
    mainLevelSensorPointer->mockIsSensing();
    assert(mainLevelSensorPointer->isSensingLiquid());
    assert(!mainLevelSensorPointer->isNotSensingLiquid());

    cout << "pass -> mockSensor_should_MockSensingLiquid\n";
}

static void mockSensor_should_MockNotSensingLiquid() {
    mainLevelSensorPointer->mockIsNotSensing();
    assert(!mainLevelSensorPointer->isSensingLiquid());
    assert(mainLevelSensorPointer->isNotSensingLiquid());

    cout << "pass -> mockSensor_should_MockNotSensingLiquid\n";
}

static void should_GoToSensingStateAfterSetup() {
    // when
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    atoStation.setup();

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);

    cout << "pass -> should_GoToSensingStateAfterSetup\n";
}

static void should_GoToSleepStateOnManualSleep() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    uint32_t sleepStartMillis = getRandomUint32();
    currentMillis = sleepStartMillis;
    uint16_t sleepMinutes = 32;
    mockAtoDispenserPointer->startDispensing();

    //when
    atoStation.sleep(sleepMinutes);
    currentMillis = sleepStartMillis + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SLEEPING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    cout << "pass -> should_GoToSleepStateOnManualSleep\n";
}

static void should_StopDispensingWhenGoingTo_SLEEP() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    uint32_t sleepStartMillis = getRandomUint32();
    currentMillis = sleepStartMillis;
    uint16_t sleepMinutes = 32;
    mockAtoDispenserPointer->startDispensing();

    //when
    atoStation.sleep(sleepMinutes);
    currentMillis = sleepStartMillis + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SLEEPING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    cout << "pass -> should_StopDispensingWhenGoingTo_SLEEP\n";
}

static void should_GoTo_SensingState_after_SetSleepTime() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    currentMillis = getRandomUint32();
    uint16_t sleepMinutes = 1;
    mockAtoDispenserPointer->startDispensing();
    atoStation.sleep(sleepMinutes);
    currentMillis += 1;
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::SLEEPING);

    //when
    currentMillis += sleepMinutes * 60ul * 1000ul + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);

    cout << "pass -> should_GoTo_SensingState_after_SetSleepTime\n";
}

static void should_GoTo_SensingState_on_ManualWake() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.sleep(32);
    assert(atoStation.getCurrentState() == AtoStation::State::SLEEPING);

    // when
    atoStation.wake();

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);

    cout << "pass -> should_GoTo_SensingState_on_ManualWake\n";
}

static void should_NotDispenseBeforeMinPeriodWhenMainSensorNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.wake();
    currentMillis = atoSettings.minDispensingIntervalMillis - 1;

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    cout << "pass -> should_NotDispenseBeforeMinPeriodWhenMainSensorNotSensing\n";
}

static void should_DispenseAfterMinPeriodWhenMainSensorNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.wake();
    currentMillis = atoSettings.minDispensingIntervalMillis + 1;

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::DISPENSING);
    assert(mockAtoDispenserPointer->getIsDispensing());

    cout << "pass -> should_DispenseAfterMinPeriodWhenMainSensorNotSensing\n";
}

static void should_StopDispensingWhenMainSensorIsSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    beforeTest(&atoStation);
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::DISPENSING);

    // when
    mainLevelSensorPointer->mockIsSensing();
    currentMillis += 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    cout << "pass -> should_StopDispensingWhenMainSensorIsSensing\n";
}

static void should_StopDispensingAfterMaxDispensePeriodAndRaiseTopOffFailed() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    currentMillis = atoSettings.maxDispensingDurationMillis + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::TopOffFailed);
    assert(mockAtoDispenserPointer->getIsNotDispensing());

    cout << "pass -> should_StopDispensingAfterMaxDispensePeriodAndRaiseTopOffFailed\n";
}

static void should_ResumeOperatingAfterReservoirRefillAndManualUserReset() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    beforeTest(&atoStation);
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    currentMillis += atoSettings.maxDispensingDurationMillis + 1;
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::TopOffFailed);

    // when
    // reservoir refill
    currentMillis = 0;
    atoStation.reset();
    atoStation.update(currentMillis);

    // then
    assert(mockAtoDispenserPointer->getIsDispensing());
    assert(atoStation.getCurrentState() == AtoStation::State::DISPENSING);
    assert(alarmStation.getNumberOfAlarms() == 0);

    cout << "pass -> should_ResumeOperatingAfterReservoirRefillAndManualUserReset\n";
}

static void should_RaiseReservoirLowWhenReservoirSensorNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    currentMillis += 1;
    mainLevelSensorPointer->mockIsSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::ReservoirLow);

    cout << "pass -> should_RaiseReservoirLowWhenReservoirSensorNotSensing\n";
}

static void should_RaiseReservoirLowWhenMainAndReservoirSensorNotSensingWhileSensingState() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    currentMillis += 1;
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    // cout << "atoStation.state: " << atoStation.state << "\n";
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::ReservoirLow);

    cout << "pass -> should_RaiseReservoirLowWhenMainAndReservoirSensorNotSensingWhileSensingState\n";
}

static void should_RaiseReservoirLowWhenMainAndReservoirSensorNotSensingWhileDispensingState() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::DISPENSING);
    currentMillis += 1;
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::ReservoirLow);

    cout << "pass -> should_RaiseReservoirLowWhenMainAndReservoirSensorNotSensingWhileDispensingState\n";
}

static void should_ResumeSensingStateAfterReservoirRefillWhenMainIsSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest(&atoStation);
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::ReservoirLow);

    // when
    currentMillis += 1;
    reservoirLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getNumberOfAlarms() == 0);

    cout << "pass -> should_ResumeSensingStateAfterReservoirRefillWhenMainIsSensing\n";
}

static void should_ResumeDispensingStateAfterReservoirRefillWhenMainIsNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest(&atoStation);
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::ReservoirLow);

    // when
    currentMillis += 1;
    reservoirLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);
    currentMillis += 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::DISPENSING);
    assert(mockAtoDispenserPointer->getIsDispensing());
    assert(alarmStation.getNumberOfAlarms() == 0);

    cout << "pass -> should_ResumeDispensingStateAfterReservoirRefillWhenMainIsNotSensing\n";
}

static void should_GoToInvalidStateWhenMainSensorNotSensingAndBackupHighSensorSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    backupHighLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::BackupHighSensorOn);

    cout << "pass -> should_GoToInvalidStateWhenMainSensorNotSensingAndBackupHighSensorSensing\n";
}

static void should_GotoInvalidStateWhenMainSensorSensingAndBackupHighSensorSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsSensing();  // dispenser failed to stop
    backupHighLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::BackupHighSensorOn);

    cout << "pass -> should_GotoInvalidStateWhenMainSensorSensingAndBackupHighSensorSensing\n";
}

static void should_GoToInvalidStateWhenReservoirSensorNotSensingAndMainSensorSensingAndBackupHighSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsSensing();  // dispenser failed to stop
    backupHighLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::BackupHighSensorOn);

    cout << "pass -> should_GoToInvalidStateWhenReservoirSensorNotSensingAndMainSensorSensingAndBackupHighSensing\n";
}

static void should_StartDispensingWhenMainSensorNotSensingAndBackupLowSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    backupLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::DISPENSING);
    assert(mockAtoDispenserPointer->getIsDispensing());

    cout << "pass -> should_StartDispensingWhenMainSensorNotSensingAndBackupLowSensing\n";
}

static void should_GoToInvalidStateWhenMainSensorSensingAndBackupLowNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::BackupLowSensorOff);

    cout << "pass -> should_GoToInvalidStateWhenMainSensorSensingAndBackupLowNotSensing\n";
}

static void should_GoToInvalidStateWhenBackupHighSensingAndBackupLowNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    backupHighLevelSensorPointer->mockIsSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::BackupHighSensorOn);

    cout << "pass -> should_GoToInvalidStateWhenBackupHighSensingAndBackupLowNotSensing\n";
}

static void should_GoToReservoirLowStateWhenAllSensorsNotSensing() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest(&atoStation);

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::BLOCKING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::ReservoirLow);

    cout << "pass -> should_GoToReservoirLowStateWhenAllSensorsNotSensing\n";
}

static void should_ExitInvalidStateAfterManualReset() {
    // given
    AtoStation atoStation = AtoStation(mainLevelSensorPointer, mockAtoDispenserPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest(&atoStation);

    backupHighLevelSensorPointer->mockIsSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.getCurrentState() == AtoStation::State::INVALID);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::BackupHighSensorOn);

    // when
    backupHighLevelSensorPointer->mockIsNotSensing();  // user fixed it
    backupLowLevelSensorPointer->mockIsSensing();      // user fixed it
    currentMillis += 1;
    atoStation.reset();
    currentMillis += 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == AtoStation::State::SENSING);
    assert(alarmStation.getNumberOfAlarms() == 0);

    cout << "pass -> should_ExitInvalidStateAfterManualReset\n";
}

int main() {
    alarmStation.attachListener(mockBuzzerPointer);

    cout << "\n"
         << "------------------------------------------------------------\n"
         << " >> TEST START\n"
         << "------------------------------------------------------------\n";

    mockAtoDispenser_should_MockStartDispensing();
    mockAtoDispenser_should_MockStopDispensing();

    mockSensor_should_MockSensingLiquid();
    mockSensor_should_MockNotSensingLiquid();

    should_GoToSensingStateAfterSetup();

    should_GoToSleepStateOnManualSleep();
    should_StopDispensingWhenGoingTo_SLEEP();
    should_GoTo_SensingState_after_SetSleepTime();
    should_GoTo_SensingState_on_ManualWake();

    should_NotDispenseBeforeMinPeriodWhenMainSensorNotSensing();
    should_DispenseAfterMinPeriodWhenMainSensorNotSensing();
    should_StopDispensingWhenMainSensorIsSensing();

    should_StopDispensingAfterMaxDispensePeriodAndRaiseTopOffFailed();
    should_ResumeOperatingAfterReservoirRefillAndManualUserReset();

    should_RaiseReservoirLowWhenReservoirSensorNotSensing();
    should_RaiseReservoirLowWhenMainAndReservoirSensorNotSensingWhileSensingState();
    should_RaiseReservoirLowWhenMainAndReservoirSensorNotSensingWhileDispensingState();

    should_ResumeSensingStateAfterReservoirRefillWhenMainIsSensing();
    should_ResumeDispensingStateAfterReservoirRefillWhenMainIsNotSensing();

    should_GoToInvalidStateWhenMainSensorNotSensingAndBackupHighSensorSensing();
    should_GotoInvalidStateWhenMainSensorSensingAndBackupHighSensorSensing();
    should_GoToInvalidStateWhenReservoirSensorNotSensingAndMainSensorSensingAndBackupHighSensing();

    should_StartDispensingWhenMainSensorNotSensingAndBackupLowSensing();

    should_GoToInvalidStateWhenMainSensorSensingAndBackupLowNotSensing();
    should_GoToInvalidStateWhenBackupHighSensingAndBackupLowNotSensing();
    should_GoToReservoirLowStateWhenAllSensorsNotSensing();

    should_ExitInvalidStateAfterManualReset();

    cout
            << "------------------------------------------------------------\n"
            << " >> TEST END\n"
            << "------------------------------------------------------------\n"
            << "\n";

    return 0;
}