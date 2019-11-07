#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <chrono>

#include "../_Mocks/MockCommon.h"

#include <Abstract/AbstractRunnable.h>

#include <Enums/AlarmCode.h>
#include <Enums/Level.h>

#include <AtoStation/AtoStation.h>
#include <AtoStation/NormalLevelSensorConnection.h>
#include <AtoStation/LowLevelSensorConnection.h>
#include <AtoStation/HighLevelSensorConnection.h>
#include <AtoStation/ReservoirLowLevelSensorConnection.h>

#include <AlarmStation/AlarmStation.h>
#include <Common/Sensor.h>
#include <Common/Switchable.h>

#include "../_Mocks/MockBuzzer.h"

AtoSettings atoSettings{};
static LinkedMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};

static void setup() {
    AbstractRunnable::setupAll();
}

static void loop() {
    AbstractRunnable::loopAll();
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static void testAtoLiquidLevelSensorStateChange() {

    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    atoNormalLevelSensor.setReading(Level::High);
    assert(atoNormalLevelSensor.isReading(Level::High));

    atoNormalLevelSensor.setReading(Level::Low);
    assert(atoNormalLevelSensor.isReading(Level::Low));

    std::cout << "ok -> testAtoLiquidLevelSensorStateChange\n";
}

static void mockAtoDispenserShouldChangeStateFromOffToOn() {
    /* given */
    Switchable atoDispenser{};

    /* when */
    assert(atoDispenser.isInState(Switched::Off));
    atoDispenser.setState(Switched::On);

    /* then */
    assert(atoDispenser.isInState(Switched::On));

    std::cout << "ok -> mockAtoDispenserShouldChangeStateFromOffToOn\n";
}

static void mockAtoDispenserShouldChangeStateFromOnToOff() {
    /* given */
    Switchable atoDispenser{};

    /* when */
    atoDispenser.setState(Switched::On);
    assert(atoDispenser.isInState(Switched::On));
    atoDispenser.setState(Switched::Off);

    /* then */
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> mockAtoDispenserShouldChangeStateFromOnToOff\n";
}

static void mockAtoNormalLiquidLevelSensorShouldHaveDefaultStateUnknown() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    /* when */
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

/* then */
    assert(atoNormalLevelSensor.isReading(Level::Unknown));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldHaveDefaultStateUnknown\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    /* when */
    atoNormalLevelSensor.setReading(Level::High);

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::High));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToHigh\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToLow\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromHighToLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    atoNormalLevelSensor.setReading(Level::High);

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromHighToLow\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromLowToHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    atoNormalLevelSensor.setReading(Level::Low);

    /* when */
    atoNormalLevelSensor.setReading(Level::High);

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::High));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromLowToHigh\n";
}

static void atoStationShouldBeInstanceOfAbstractSleepable() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    /* when & then */
    assert(&atoStation == dynamic_cast<AbstractSleepable *>(&atoStation));

    std::cout << "ok -> atoStationShouldBeInstanceOfAbstractSleepable\n";

}

static void atoStationShouldBeInStateInvalidBeforeSetup() {
    /* when */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    /* then */
    assert(atoStation.isInState(AtoStationState::Invalid));

    std::cout << "ok -> atoStationShouldBeInStateInvalidBeforeSetup\n";
}

static void atoStationShouldBeInStateSensingAfterSetup() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    /* when */
    setup();

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));

    std::cout << "ok -> atoStationShouldBeInStateSensingAfterSetup\n";
}

static void atoStationShouldGoToStateSleepingOnStartSleeping() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    setup();
    loop();

    //when
    atoStation.startSleeping(300000);
    loop();

    /* then */
    assert(atoStation.isInState(AtoStationState::Sleeping));

    std::cout << "ok -> atoStationShouldGoToStateSleepingOnStartSleeping\n";
}

static void atoStationShouldStopDispensingOnStartSleeping() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    setup();
    loop();

    //when
    atoDispenser.setState(Switched::On);
    atoStation.startSleeping(300000); // 5min
    loop();

    /* then */
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldStopDispensingOnStartSleeping\n";
}

static void atoStationShouldNotRaiseAlarmsWhileSleeping() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    HighLevelSensorConnection<AtoStation, Level> highLevelSensorConnection(atoStation);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);
    Sensor<Level> highLevelSensor(&highLevelSensorConnection, Level::Unknown);
    Sensor<Level> normalLevelSensor(&normalLevelSensorConnection, Level::High);
    Sensor<Level> lowLevelSensor(&highLevelSensorConnection, Level::Unknown);
    Sensor<Level> reservoirLowLevelSensor(&highLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    //when
    atoStation.startSleeping(300000); // 5min
    loop();
    normalLevelSensor.setReading(Level::Low);
    loop();
    highLevelSensor.setReading(Level::High);
    loop();
    lowLevelSensor.setReading(Level::Low);
    loop();
    reservoirLowLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(alarmStation.alarmList.isEmpty());

    std::cout << "ok -> atoStationShouldNotRaiseAlarmsWhileSleeping\n";
}

static void atoStationShouldGoToStateSensingAfterSleepTimeHasPassed() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    setup();
    loop();

    atoDispenser.setState(Switched::On);

    uint32_t sleepMs = 300000; // 5min
    atoStation.startSleeping(sleepMs);
    loop();

    assert(atoStation.isInState(AtoStationState::Sleeping));
    assert(atoDispenser.isInState(Switched::Off));

    //when
    loop(sleepMs);

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldGoToStateSensingAfterSleepTimeHasPassed\n";
}

static void atoStationShouldGoToStateSensingOnStopSleeping() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    setup();
    loop();

    atoDispenser.setState(Switched::On);

    uint32_t sleepMs = 300000; // 5min
    atoStation.startSleeping(sleepMs);
    loop();

    assert(atoStation.isInState(AtoStationState::Sleeping));
    assert(atoDispenser.isInState(Switched::Off));

    /* when */
    atoStation.stopSleeping();

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));

    std::cout << "ok -> atoStationShouldGoToStateSensingOnStopSleeping\n";
}

static void atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Dispensing));
    assert(atoDispenser.isInState(Switched::On));

    std::cout << "ok -> atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLow\n";
}

static void atoStationShouldGoToStateSensingOnNormalLevelStateChangeToHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoNormalLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::High));
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldGoToStateSensingOnNormalLevelStateChangeToHigh\n";
}

static void atoStationShouldNotGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasNotPassed() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    atoNormalLevelSensor.setReading(Level::High);
    setup();
    loop();

    // should start dispensing
    atoNormalLevelSensor.setReading(Level::Low);
    loop();

    // should stop dispensing
    atoNormalLevelSensor.setReading(Level::High);
    loop();

    /* when */
    loop(atoSettings.minDispensingIntervalMs - 100);
    atoNormalLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldNotGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasNotPassed\n";
}

static void atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasPassed() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);

    atoNormalLevelSensor.setReading(Level::High);
    setup();
    loop();

    // should start dispensing
    atoNormalLevelSensor.setReading(Level::Low);
    loop();

    // should stop dispensing
    atoNormalLevelSensor.setReading(Level::High);
    loop();

    /* when */
    uint32_t deltaMs = 100;
    loop(atoSettings.minDispensingIntervalMs - deltaMs);
    atoNormalLevelSensor.setReading(Level::Low);
    loop(deltaMs);

    /* then */
    assert(atoStation.isInState(AtoStationState::Dispensing));
    assert(atoDispenser.isInState(Switched::On));

    std::cout << "ok -> atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasPassed\n";
}

static void atoStationShouldStopDispensingAfterMaxDispensePeriodAndGoToStateAlarming() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop(atoSettings.maxDispensingDurationMs + 1);

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoTopOffFailed));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldStopDispensingAfterMaxDispensePeriodAndGoToStateAlarming\n";
}

static void atoStationShouldGoToStateAlarmingOnReservoirLowLevelStateChangeToLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);
    Sensor<Level> atoReservoirLowLevelSensor(&reservoirLowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoReservoirLowLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoReservoirLowLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnReservoirLowLevelStateChangeToLow\n";
}

static void atoStationShouldGoToStateSensingOnReservoirLowAndReservoirRefill() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);
    Sensor<Level> atoReservoirLowLevelSensor(&reservoirLowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoReservoirLowLevelSensor.setReading(Level::Low);
    loop();

    /* when */
    /* user top offs the reservoir */
    atoReservoirLowLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(atoReservoirLowLevelSensor.isReading(Level::High));
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToStateSensingOnReservoirLowAndReservoirRefill\n";
}

static void atoStationShouldGoToStateSensingOnReservoirRefillAndManualUserReset() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoNormalLevelSensor.setReading(Level::Low);
    loop(atoSettings.maxDispensingDurationMs);
    assert(atoStation.isInState(AtoStationState::Dispensing));
    loop();
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoTopOffFailed));

    /* when and then */
    /* user top offs the reservoir */
    atoStation.reset(); // <- will delete ATO alarms
    loop();

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(alarmStation.alarmList.isEmpty());

    std::cout << "ok -> atoStationShouldGoToStateSensingOnReservoirRefillAndManualUserReset\n";
}

static void atoStationShouldGoToStateAlarmingAfterMinDispensingPeriodOnManualUserResetWithoutReservoirRefill() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoNormalLevelSensor.setReading(Level::Low);
    loop(atoSettings.maxDispensingDurationMs);
    assert(atoStation.isInState(AtoStationState::Dispensing));
    loop();
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoTopOffFailed));

    /* when and then */
    /* user top offs the reservoir */
    atoStation.reset(); // <- will delete ATO alarms
    loop();
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(alarmStation.alarmList.isEmpty());

    loop(atoSettings.minDispensingIntervalMs - 2); // plus two iterations since the firs fail
    assert(atoStation.isInState(AtoStationState::Dispensing));
    loop();

    /* then */
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoTopOffFailed));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingAfterMinDispensingPeriodOnManualUserResetWithoutReservoirRefill\n";
}

static void atoStationShouldDispenseForMaxDispenseDurationOnNormalLevelLowAndReservoirLowLevelLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);

    Sensor<Level> normalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> reservoirLowLevelSensor(&reservoirLowLevelSensorConnection, Level::Unknown);

    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    normalLevelSensor.setReading(Level::Low);
    loop();

    /* when */
    reservoirLowLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(normalLevelSensor.isReading(Level::Low));
    assert(reservoirLowLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Dispensing));
    loop(atoSettings.maxDispensingDurationMs);
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoTopOffFailed));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelLowAndReservoirLowLevelLow\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelHighAndReservoirLowLevelLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);
    Sensor<Level> atoMainLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoReservoirLowLevelSensor(&reservoirLowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoMainLevelSensor.setReading(Level::High);
    loop();

    /* when */
    atoReservoirLowLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoMainLevelSensor.isReading(Level::High));
    assert(atoReservoirLowLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelHighAndReservoirLowLevelLow\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelLowAndHighLevelHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, Level> levelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoHighLiquidLevelSensor(&levelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoHighLiquidLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoHighLiquidLevelSensor.isReading(Level::High));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelLowAndHighLevelHigh\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelHighAndHighLevelHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, Level> levelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoHighLiquidLevelSensor(&levelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::High);
    loop();
    atoHighLiquidLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::High));
    assert(atoHighLiquidLevelSensor.isReading(Level::High));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelHighAndHighLevelHigh\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelHighAndLowLevelLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::High);
    loop();
    atoLowLiquidLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::High));
    assert(atoLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelHighAndLowLevelLow\n";
}

static void atoStationShouldGoToStateDispensingOnNormalLevelLowAndLowLevelHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoLowLiquidLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoLowLiquidLevelSensor.isReading(Level::High));
    assert(atoStation.isInState(AtoStationState::Dispensing));
    assert(atoDispenser.isInState(Switched::On));
    assert(alarmStation.alarmList.isEmpty());

    std::cout << "ok -> atoStationShouldGoToStateDispensingOnNormalLevelLowAndLowLevelHigh\n";
}

static void atoStationShouldGoToStateAlarmingWhenMainSensorLowAndLowSensorLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoLowLiquidLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingWhenMainSensorLowAndLowSensorLow\n";
}

static void atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelLow() {
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, Level> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoHighLiquidLevelSensor(&highLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoHighLiquidLevelSensor.setReading(Level::High);
    loop();
    atoLowLiquidLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoHighLiquidLevelSensor.isReading(Level::High));
    assert(atoLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 2);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelLow\n";
}

static void atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelHigh() {
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, Level> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoHighLiquidLevelSensor(&highLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoHighLiquidLevelSensor.setReading(Level::High);
    loop();
    atoLowLiquidLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoHighLiquidLevelSensor.isReading(Level::High));
    assert(atoLowLiquidLevelSensor.isReading(Level::High));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 1);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelHigh\n";
}

static void atoStationShouldGoToAlarmingStateWhenAllSensorsNotSensing() {
    /* given*/
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, Level> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoHighLiquidLevelSensor(&highLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoReservoirLowLiquidLevelSensor(&reservoirLowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoHighLiquidLevelSensor.setReading(Level::Low);
    loop();
    atoNormalLevelSensor.setReading(Level::Low);
    loop();
    atoLowLiquidLevelSensor.setReading(Level::Low);
    loop();
    atoReservoirLowLiquidLevelSensor.setReading(Level::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoHighLiquidLevelSensor.isReading(Level::Low));
    assert(atoLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoReservoirLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 2);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToAlarmingStateWhenAllSensorsNotSensing\n";
}

static void atoStationShouldAcknowledgeAlarmsAndGoToStateSensing() {
    /* given*/
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, Level> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);
    Sensor<Level> atoNormalLevelSensor(&normalLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoHighLiquidLevelSensor(&highLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoLowLiquidLevelSensor(&lowLevelSensorConnection, Level::Unknown);
    Sensor<Level> atoReservoirLowLiquidLevelSensor(&reservoirLowLevelSensorConnection, Level::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoHighLiquidLevelSensor.setReading(Level::Low);
    atoNormalLevelSensor.setReading(Level::Low);
    atoLowLiquidLevelSensor.setReading(Level::Low);
    atoReservoirLowLiquidLevelSensor.setReading(Level::Low);
    loop(atoSettings.maxDispensingDurationMs);

    assert(atoNormalLevelSensor.isReading(Level::Low));
    assert(atoHighLiquidLevelSensor.isReading(Level::Low));
    assert(atoLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoReservoirLowLiquidLevelSensor.isReading(Level::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 2);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AtoLowLevel));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));


    /* when */
    atoReservoirLowLiquidLevelSensor.setReading(Level::High);
    atoLowLiquidLevelSensor.setReading(Level::High);
    loop();

    /* then */
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::AtoLowLevel));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldAcknowledgeAlarmsAndGoToStateSensing\n";
}

int main() {

    alarmNotifyConfigurations.put(AlarmSeverity::Critical, AlarmNotifyConfiguration(1, 7000));
    alarmNotifyConfigurations.put(AlarmSeverity::Major, AlarmNotifyConfiguration(5, 5000));
    alarmNotifyConfigurations.put(AlarmSeverity::Minor, AlarmNotifyConfiguration(15, 3000));
    alarmNotifyConfigurations.put(AlarmSeverity::NoSeverity, AlarmNotifyConfiguration(60, 1000));

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 3;

    for (int i = 0; i < repeat; ++i) {

        testAtoLiquidLevelSensorStateChange();

        mockAtoDispenserShouldChangeStateFromOffToOn();
        mockAtoDispenserShouldChangeStateFromOnToOff();

        mockAtoNormalLiquidLevelSensorShouldHaveDefaultStateUnknown();
        mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToHigh();
        mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToLow();
        mockAtoNormalLiquidLevelSensorShouldChangeStateFromHighToLow();
        mockAtoNormalLiquidLevelSensorShouldChangeStateFromLowToHigh();

        atoStationShouldBeInstanceOfAbstractSleepable();
        atoStationShouldBeInStateInvalidBeforeSetup();
        atoStationShouldBeInStateSensingAfterSetup();

        atoStationShouldGoToStateSleepingOnStartSleeping();
        atoStationShouldStopDispensingOnStartSleeping();
        atoStationShouldNotRaiseAlarmsWhileSleeping();
        atoStationShouldGoToStateSensingAfterSleepTimeHasPassed();
        atoStationShouldGoToStateSensingOnStopSleeping();

        atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLow();
        atoStationShouldGoToStateSensingOnNormalLevelStateChangeToHigh();

        atoStationShouldNotGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasNotPassed();
        atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasPassed();

        atoStationShouldStopDispensingAfterMaxDispensePeriodAndGoToStateAlarming();

        atoStationShouldGoToStateAlarmingOnReservoirLowLevelStateChangeToLow();
        atoStationShouldGoToStateSensingOnReservoirLowAndReservoirRefill();
        atoStationShouldGoToStateSensingOnReservoirRefillAndManualUserReset();
        atoStationShouldGoToStateAlarmingAfterMinDispensingPeriodOnManualUserResetWithoutReservoirRefill();

        atoStationShouldDispenseForMaxDispenseDurationOnNormalLevelLowAndReservoirLowLevelLow();
        atoStationShouldGoToStateAlarmingOnNormalLevelHighAndReservoirLowLevelLow();

        atoStationShouldGoToStateAlarmingOnNormalLevelLowAndHighLevelHigh();
        atoStationShouldGoToStateAlarmingOnNormalLevelHighAndHighLevelHigh();

        atoStationShouldGoToStateAlarmingOnNormalLevelHighAndLowLevelLow();
        atoStationShouldGoToStateDispensingOnNormalLevelLowAndLowLevelHigh();

        atoStationShouldGoToStateAlarmingWhenMainSensorLowAndLowSensorLow();

        atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelLow();
        atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelHigh();
        atoStationShouldGoToAlarmingStateWhenAllSensorsNotSensing();

        atoStationShouldAcknowledgeAlarmsAndGoToStateSensing();

        if (repeat > 1) {
            std::cout << "------------------------------------------------------------\n";
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\n"
                 "------------------------------------------------------------" << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout
            << "------------------------------------------------------------\n"
            << " >> TEST END\n"
            << "------------------------------------------------------------\n"
            << "\n";

    return 0;
}