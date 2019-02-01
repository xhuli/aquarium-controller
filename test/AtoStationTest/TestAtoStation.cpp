#define __TEST_MODE__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <random>

#include "MockMillis.h"

#include "AtoStation/AtoStation.h"

#include "../_Mocks/MockLiquidDispenser.h"
#include "../_Mocks/MockLiquidLevelSensor.h"
#include "../_Mocks/MockStorage.h"

uint32_t getRandomUint32() {
    // https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range

    std::random_device rd;                                       // only used once to initialise (seed) engine
    std::mt19937 rng(rd());                                      // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<uint32_t> uni(0, 4294967295);  // guaranteed unbiased

    return uni(rng);
}

MockLiquidDispenser *mockAtoDispenserPointer = new MockLiquidDispenser();
MockLiquidLevelSensor *mainLevelSensorPointer = new MockLiquidLevelSensor();
MockLiquidLevelSensor *backupHighLevelSensorPointer = new MockLiquidLevelSensor();
MockLiquidLevelSensor *backupLowLevelSensorPointer = new MockLiquidLevelSensor();
MockLiquidLevelSensor *reservoirLowLevelSensorPointer = new MockLiquidLevelSensor();
MockStorage *mockStoragePointer = new MockStorage();

AtoSettings atoSettings = AtoSettings();
AtoStation atoStation = AtoStation(mockStoragePointer, mockAtoDispenserPointer);

static void beforeTest() {
    currentMillis = 0;
    atoStation.lastDispenseEndMillis = 0;

    mainLevelSensorPointer->mockIsSensing();
    reservoirLowLevelSensorPointer->mockIsSensing();
    backupHighLevelSensorPointer->mockIsNotSensing();
    backupLowLevelSensorPointer->mockIsSensing();

    mockAtoDispenserPointer->stopDispensing();

    atoStation.setup();
    atoStation.update(currentMillis);

    currentMillis = atoSettings.minDispensingIntervalMillis + 1;

    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
};

static void afterTest() {
    // pass
};

static void mockAtoDispenser_should_mock_start_dispensing() {
    mockAtoDispenserPointer->startDispensing();
    assert(mockAtoDispenserPointer->getIsDispensing());
    std::cout << "pass -> mockAtoDispenser_should_mock_start_dispensing" << std::endl;
};

static void mockAtoDispenser_should_mock_stop_dispensing() {
    mockAtoDispenserPointer->stopDispensing();
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    std::cout << "pass -> mockAtoDispenser_should_mock_stop_dispensing" << std::endl;
};

static void mockSensor_should_mock_sensing_liquid() {
    mainLevelSensorPointer->mockIsSensing();
    assert(mainLevelSensorPointer->isSensingLiquid());
    assert(!mainLevelSensorPointer->isNotSensingLiquid());
    std::cout << "pass -> mockSensor_should_mock_sensing_liquid" << std::endl;
};

static void mockSensor_should_mock_not_sensing_liquid() {
    mainLevelSensorPointer->mockIsNotSensing();
    assert(!mainLevelSensorPointer->isSensingLiquid());
    assert(mainLevelSensorPointer->isNotSensingLiquid());
    std::cout << "pass -> mockSensor_should_mock_not_sensing_liquid" << std::endl;
};

static void mockStorage_should_mock_save_AtoSettings() {
    AtoSettings atoSettings = AtoSettings();
    mockStoragePointer->saveAtoSettings(atoSettings);
    std::cout << "pass -> mockStorage_should_mock_save_AtoSettings" << std::endl;
};

static void mockStorage_should_mock_read_AtoSettings() {
    AtoSettings atoSettings = AtoSettings();
    atoSettings.maxDispensingDurationMillis = 1;
    atoSettings.minDispensingIntervalMillis = 3;

    AtoSettings readAtoSettings;

    readAtoSettings = mockStoragePointer->readAtoSettings(atoSettings);

    assert(readAtoSettings.maxDispensingDurationMillis == atoSettings.maxDispensingDurationMillis);
    assert(readAtoSettings.minDispensingIntervalMillis == atoSettings.minDispensingIntervalMillis);

    std::cout << "pass -> mockStorage_should_mock_read_AtoSettings" << std::endl;
};

static void should_be_SENSING_after_setup() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    assert(atoStation.state == atoStation.AtoStationState::INVALID);

    // when
    atoStation.setup();

    // then
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    std::cout << "pass -> should_be_SENSING_after_setup" << std::endl;
};

static void should_GoToSleepState_on_ManualSleep() {
    // given
    uint32_t sleepStartMillis = getRandomUint32();
    currentMillis = sleepStartMillis;
    uint16_t sleepMinutes = 32;
    mockAtoDispenserPointer->startDispensing();

    //when
    atoStation.sleep(sleepMinutes);
    currentMillis = sleepStartMillis + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::SLEEPING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(atoStation.sleepStartMillis == sleepStartMillis);
    assert(atoStation.sleepPeriodMillis == sleepMinutes * 60ul * 1000ul);
    assert(atoStation.sleepPeriodMillis == 1920000);
    std::cout << "pass -> should_GoToSleepState_on_ManualSleep" << std::endl;
};

static void should_StopDispensing_when_GoingTo_SLEEP(){
    // given
    uint32_t sleepStartMillis = getRandomUint32();
    currentMillis = sleepStartMillis;
    uint16_t sleepMinutes = 32;
    mockAtoDispenserPointer->startDispensing();

    //when
    atoStation.sleep(sleepMinutes);
    currentMillis = sleepStartMillis + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::SLEEPING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    std::cout << "pass -> should_StopDispensing_when_GoingTo_SLEEP" << std::endl;
};

static void should_GoTo_SensingState_after_SetSleepTime() {
    // given
    currentMillis = getRandomUint32();
    uint16_t sleepMinutes = 1;
    mockAtoDispenserPointer->startDispensing();
    atoStation.sleep(sleepMinutes);
    currentMillis += 1;
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::SLEEPING);

    //when
    currentMillis += sleepMinutes * 60ul * 1000ul + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.getCurrentState() == atoStation.AtoStationState::SENSING);
    std::cout << "pass -> should_GoTo_SensingState_after_SetSleepTime" << std::endl;
};

static void should_GoTo_SensingState_on_ManualWake() {
    // given
    atoStation.sleep(32);
    assert(atoStation.state == atoStation.AtoStationState::SLEEPING);

    // when
    atoStation.wake();

    // then
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    std::cout << "pass -> should_GoTo_SensingState_on_ManualWake" << std::endl;
};

static void should_not_dispense_before_min_period_if_main_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.wake();
    atoStation.lastDispenseEndMillis = 0;
    currentMillis = atoSettings.minDispensingIntervalMillis - 1;

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    std::cout << "pass -> should_not_dispense_before_min_period_if_main_not_sensing" << std::endl;
};

static void should_dispense_after_min_period_if_main_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.wake();
    atoStation.lastDispenseEndMillis = 0;
    currentMillis = atoSettings.minDispensingIntervalMillis + 1;

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::DISPENSING);
    assert(atoStation.dispensingStartMillis == currentMillis);
    assert(mockAtoDispenserPointer->getIsDispensing());
    std::cout << "pass -> should_dispense_after_min_period_if_main_not_sensing" << std::endl;
};

static void should_stop_dispensing_if_main_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    mainLevelSensorPointer->mockIsSensing();
    currentMillis += 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(atoStation.lastDispenseEndMillis == currentMillis);
    std::cout << "pass -> should_stop_dispensing_if_main_sensing" << std::endl;
};

static void should_stop_dispensing_after_max_dispense_period_and_raise_RESERVOIR_LOW() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    currentMillis = atoSettings.maxDispensingDurationMillis + 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);
    // todo: check alarm
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(atoStation.lastDispenseEndMillis == currentMillis);
    std::cout << "pass -> should_stop_dispensing_after_max_dispense_period_and_raise_RESERVOIR_LOW" << std::endl;
};

static void should_ResumeOperating_after_ReservoirRefill_and_ManualUserReset() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    beforeTest();
    mainLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    currentMillis += atoSettings.maxDispensingDurationMillis + 1;
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);

    // when
    // reservoir refill
    currentMillis = 0;
    atoStation.reset();
    atoStation.update(currentMillis);

    // then
    assert(mockAtoDispenserPointer->getIsDispensing());
    assert(atoStation.getCurrentState() == atoStation.AtoStationState::DISPENSING);
};

static void should_raise_RESERVOIR_LOW_when_reservoir_sensor_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest();

    // when
    currentMillis += 1;
    mainLevelSensorPointer->mockIsSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_RESERVOIR_LOW_when_reservoir_sensor_not_sensing" << std::endl;
};

static void should_raise_RESERVOIR_LOW_when_main_and_reservoir_sensor_not_sensing_while_SENSING() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest();

    // when
    currentMillis += 1;
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    // std::cout << "atoStation.state: " << atoStation.state << std::endl;
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_RESERVOIR_LOW_when_main_and_reservoir_sensor_not_sensing_while_SENSING" << std::endl;
};

static void should_raise_RESERVOIR_LOW_when_main_and_reservoir_sensor_not_sensing_while_DISPENSING() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::DISPENSING);
    currentMillis += 1;
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    assert(atoStation.lastDispenseEndMillis == currentMillis);
    // todo: check alarm
    std::cout << "pass -> should_raise_RESERVOIR_LOW_when_main_and_reservoir_sensor_not_sensing_while_DISPENSING" << std::endl;
};

static void should_resume_SENSING_after_reservoir_refill_when_main_is_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);

    // when
    currentMillis += 1;
    reservoirLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_resume_SENSING_after_reservoir_refill_when_main_is_sensing" << std::endl;
};

static void should_resume_DISPENSING_after_reservoir_refill_when_main_is_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    beforeTest();
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);

    // when
    currentMillis += 1;
    reservoirLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    currentMillis += 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::DISPENSING);
    assert(mockAtoDispenserPointer->getIsDispensing());
    assert(atoStation.dispensingStartMillis == currentMillis);
    // todo: check alarm
    std::cout << "pass -> should_resume_DISPENSING_after_reservoir_refill_when_main_is_not_sensing" << std::endl;
};

static void should_raise_INVALID_if_main_sensor_is_not_sensing_and_bckp_high_is_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    backupHighLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_INVALID_if_main_sensor_is_not_sensing_and_bckp_high_is_sensing" << std::endl;
};

static void should_raise_INVALID_if_main_sensor_is_sensing_and_bckp_high_is_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsSensing();  // dispenser failed to stop
    backupHighLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_INVALID_if_main_sensor_is_sensing_and_bckp_high_is_sensing" << std::endl;
};

static void should_raise_INVALID_if_reservoir_not_sensing_and_main_sensor_is_sensing_and_bckp_high_is_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsSensing();  // dispenser failed to stop
    backupHighLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_INVALID_if_reservoir_not_sensing_and_main_sensor_is_sensing_and_bckp_high_is_sensing" << std::endl;
};

static void should_start_DISPENSING_if_main_sensor_not_sensing_and_bckp_low_is_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsNotSensing();
    backupLowLevelSensorPointer->mockIsSensing();
    atoStation.update(currentMillis);

    // then
    // std::cout << "atoStation.state " << atoStation.state << std::endl;
    assert(atoStation.state == atoStation.AtoStationState::DISPENSING);
    assert(mockAtoDispenserPointer->getIsDispensing());
    // todo: check alarm
    std::cout << "pass -> should_start_DISPENSING_if_main_sensor_not_sensing_and_bckp_low_is_sensing" << std::endl;
};

static void should_raise_INVALID_if_main_sensor_sensing_and_bckp_low_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest();

    // when
    mainLevelSensorPointer->mockIsSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    // std::cout << "atoStation.state " << atoStation.state << std::endl;
    assert(atoStation.state == atoStation.AtoStationState::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_INVALID_if_main_sensor_sensing_and_bckp_low_not_sensing" << std::endl;
};

static void should_raise_INVALID_if_bckp_high_sensing_and_bckp_low_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest();

    // when
    backupHighLevelSensorPointer->mockIsSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    // std::cout << "atoStation.state " << atoStation.state << std::endl;
    assert(atoStation.state == atoStation.AtoStationState::INVALID);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_INVALID_if_bckp_high_sensing_and_bckp_low_not_sensing" << std::endl;
};

static void should_raise_RESERVOIR_LOW_if_all_sensors_not_sensing() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest();

    // when
    // std::cout << "atoStation.state " << atoStation.state << std::endl;
    mainLevelSensorPointer->mockIsNotSensing();
    reservoirLowLevelSensorPointer->mockIsNotSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);

    // then
    // std::cout << "atoStation.state " << atoStation.state << std::endl;
    assert(atoStation.state == atoStation.AtoStationState::RESERVOIR_LOW);
    assert(mockAtoDispenserPointer->getIsNotDispensing());
    // todo: check alarm
    std::cout << "pass -> should_raise_RESERVOIR_LOW_if_all_sensors_not_sensing" << std::endl;
};

static void should_exit_INVALID_after_manual_reset() {
    // given
    atoStation.attachMainLevelSensor(mainLevelSensorPointer);
    atoStation.attachReservoirLowLevelSensor(reservoirLowLevelSensorPointer);
    atoStation.attachBackupHighLevelSensor(backupHighLevelSensorPointer);
    atoStation.attachBackupLowLevelSensor(backupLowLevelSensorPointer);
    beforeTest();

    backupHighLevelSensorPointer->mockIsSensing();
    backupLowLevelSensorPointer->mockIsNotSensing();
    atoStation.update(currentMillis);
    assert(atoStation.state == atoStation.AtoStationState::INVALID);

    // when
    backupHighLevelSensorPointer->mockIsNotSensing();  // user fixed it
    backupLowLevelSensorPointer->mockIsSensing();      // user fixed it
    currentMillis += 1;
    atoStation.reset();
    currentMillis += 1;
    atoStation.update(currentMillis);

    // then
    assert(atoStation.state == atoStation.AtoStationState::SENSING);
    std::cout << "pass -> should_exit_INVALID_after_manual_reset" << std::endl;
};

int main() {
    std::cout << std::endl
              << "------------------------------------------------------------" << std::endl
              << " >> TEST START" << std::endl
              << "------------------------------------------------------------" << std::endl;
    mockAtoDispenser_should_mock_start_dispensing();
    mockAtoDispenser_should_mock_stop_dispensing();

    mockSensor_should_mock_sensing_liquid();
    mockSensor_should_mock_not_sensing_liquid();

    mockStorage_should_mock_save_AtoSettings();
    mockStorage_should_mock_read_AtoSettings();

    should_be_SENSING_after_setup();

    should_GoToSleepState_on_ManualSleep();
    should_StopDispensing_when_GoingTo_SLEEP();
    should_GoTo_SensingState_after_SetSleepTime();
    should_GoTo_SensingState_on_ManualWake();

    should_not_dispense_before_min_period_if_main_not_sensing();
    should_dispense_after_min_period_if_main_not_sensing();
    should_stop_dispensing_if_main_sensing();

    should_stop_dispensing_after_max_dispense_period_and_raise_RESERVOIR_LOW();
    should_ResumeOperating_after_ReservoirRefill_and_ManualUserReset();

    should_raise_RESERVOIR_LOW_when_reservoir_sensor_not_sensing();
    should_raise_RESERVOIR_LOW_when_main_and_reservoir_sensor_not_sensing_while_SENSING();
    should_raise_RESERVOIR_LOW_when_main_and_reservoir_sensor_not_sensing_while_DISPENSING();

    should_resume_SENSING_after_reservoir_refill_when_main_is_sensing();
    should_resume_DISPENSING_after_reservoir_refill_when_main_is_not_sensing();

    should_raise_INVALID_if_main_sensor_is_not_sensing_and_bckp_high_is_sensing();
    should_raise_INVALID_if_main_sensor_is_sensing_and_bckp_high_is_sensing();
    should_raise_INVALID_if_reservoir_not_sensing_and_main_sensor_is_sensing_and_bckp_high_is_sensing();

    should_start_DISPENSING_if_main_sensor_not_sensing_and_bckp_low_is_sensing();

    should_raise_INVALID_if_main_sensor_sensing_and_bckp_low_not_sensing();
    should_raise_INVALID_if_bckp_high_sensing_and_bckp_low_not_sensing();
    should_raise_RESERVOIR_LOW_if_all_sensors_not_sensing();

    should_exit_INVALID_after_manual_reset();

    std::cout
            << "------------------------------------------------------------" << std::endl
            << " >> TEST END" << std::endl
            << "------------------------------------------------------------" << std::endl
            << std::endl;

    return 0;
}