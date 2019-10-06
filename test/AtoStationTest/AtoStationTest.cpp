#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <chrono>

#include "../_Mocks/MockCommon.h"

#include <Abstract/AbstractRunnable.h>

#include <Enums/AlarmCode.h>
#include <Enums/LiquidLevelState.h>

#include <AtoStation/AtoStation.h>
#include <AtoStation/NormalLevelSensorConnection.h>
#include <AtoStation/LowLevelSensorConnection.h>
#include <AtoStation/HighLevelSensorConnection.h>
#include <AtoStation/ReservoirLowLevelSensorConnection.h>

#include <AlarmStation/AlarmStation.h>
#include <Common/Sensor.h>

#include "../_Mocks/Switchable.h"
#include "../_Mocks/MockBuzzer.h"

AtoSettings atoSettings{};
static LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};

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

static void testAtoLiquidLevelSensorStateChange() {

    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

//    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Unknown));

    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::High));

    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));

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
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

/* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Unknown));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldHaveDefaultStateUnknown\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::High);

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::High));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToHigh\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromUnknownToLow\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromHighToLow() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    atoNormalLevelSensor.setReading(LiquidLevelState::High);

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromHighToLow\n";
}

static void mockAtoNormalLiquidLevelSensorShouldChangeStateFromLowToHigh() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    atoNormalLevelSensor.setReading(LiquidLevelState::Low);

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::High);

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::High));

    std::cout << "ok -> mockAtoNormalLiquidLevelSensorShouldChangeStateFromLowToHigh\n";
}

static void atoStationShouldBeInstanceOfAbstractSleepable() {
    /* given */
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    /* when & then */
    assert(&atoStation == dynamic_cast<ISleepable *>(&atoStation));

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
    currentMillis = getRandomUint32();
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
    currentMillis = getRandomUint32();
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
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> highLevelSensorConnection(atoStation);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> highLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> normalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::High);
    Sensor<LiquidLevelState> lowLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> reservoirLowLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    //when
    atoStation.startSleeping(300000); // 5min
    loop();
    highLevelSensor.setReading(LiquidLevelState::High);
    loop();
    lowLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    reservoirLowLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(alarmStation.alarmList.isEmpty());

    std::cout << "ok -> atoStationShouldNotRaiseAlarmsWhileSleeping\n";
}

static void atoStationShouldGoToStateSensingAfterSleepTimeHasPassed() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    setup();
    loop();

    atoDispenser.setState(Switched::On);

    uint32_t sleepDurationMs = 300000; // 5min
    atoStation.startSleeping(sleepDurationMs);
    loop();

    assert(atoStation.isInState(AtoStationState::Sleeping));
    assert(atoDispenser.isInState(Switched::Off));

    //when
    loop(sleepDurationMs);

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldGoToStateSensingAfterSleepTimeHasPassed\n";
}

static void atoStationShouldGoToStateSensingOnStopSleeping() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    setup();
    loop();

    atoDispenser.setState(Switched::On);

    uint32_t sleepDurationMs = 300000; // 5min
    atoStation.startSleeping(sleepDurationMs);
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
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Dispensing));
    assert(atoDispenser.isInState(Switched::On));

    std::cout << "ok -> atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLow\n";
}

static void atoStationShouldGoToStateSensingOnNormalLevelStateChangeToHigh() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::High));
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldGoToStateSensingOnNormalLevelStateChangeToHigh\n";
}

static void atoStationShouldNotGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasNotPassed() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    setup();
    loop();

    // should start dispensing
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    // should stop dispensing
    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* when */
    loop(atoSettings.minDispensingIntervalMs - 100);
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldNotGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasNotPassed\n";
}

static void atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasPassed() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);

    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    setup();
    loop();

    // should start dispensing
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    // should stop dispensing
    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* when */
    uint32_t deltaMs = 100;
    loop(atoSettings.minDispensingIntervalMs - deltaMs);
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop(deltaMs);

    /* then */
    assert(atoStation.isInState(AtoStationState::Dispensing));
    assert(atoDispenser.isInState(Switched::On));

    std::cout << "ok -> atoStationShouldGoToStateDispensingOnNormalLevelStateChangeToLowWhenMinDispensingPeriodHasPassed\n";
}

static void atoStationShouldStopDispensingAfterMaxDispensePeriodAndGoToStateAlarming() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop(atoSettings.maxDispensingDurationMs + 1);

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoTopOffFailed));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldStopDispensingAfterMaxDispensePeriodAndGoToStateAlarming\n";
}

static void atoStationShouldGoToStateAlarmingOnReservoirLowLevelStateChangeToLow() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoReservoirLowLevelSensor(&reservoirLowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoReservoirLowLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoReservoirLowLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));
    assert(atoDispenser.isInState(Switched::Off));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnReservoirLowLevelStateChangeToLow\n";
}

static void atoStationShouldGoToStateSensingOnReservoirLowAndReservoirRefill() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoReservoirLowLevelSensor(&reservoirLowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoReservoirLowLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* when */
    /* user top offs the reservoir */
    atoReservoirLowLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* then */
    assert(atoReservoirLowLevelSensor.isReading(LiquidLevelState::High));
    assert(atoStation.isInState(AtoStationState::Sensing));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToStateSensingOnReservoirLowAndReservoirRefill\n";
}

static void atoStationShouldGoToStateSensingOnReservoirRefillAndManualUserReset() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
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
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
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
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);

    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);

    Sensor<LiquidLevelState> normalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> reservoirLowLevelSensor(&reservoirLowLevelSensorConnection, LiquidLevelState::Unknown);

    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    normalLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* when */
    reservoirLowLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(normalLevelSensor.isReading(LiquidLevelState::Low));
    assert(reservoirLowLevelSensor.isReading(LiquidLevelState::Low));
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
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoMainLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoReservoirLowLevelSensor(&reservoirLowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoMainLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* when */
    atoReservoirLowLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoMainLevelSensor.isReading(LiquidLevelState::High));
    assert(atoReservoirLowLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelHighAndReservoirLowLevelLow\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelLowAndHighLevelHigh() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> levelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoHighLiquidLevelSensor(&levelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoHighLiquidLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoHighLiquidLevelSensor.isReading(LiquidLevelState::High));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelLowAndHighLevelHigh\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelHighAndHighLevelHigh() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> levelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoHighLiquidLevelSensor(&levelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    loop();
    atoHighLiquidLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::High));
    assert(atoHighLiquidLevelSensor.isReading(LiquidLevelState::High));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelHighAndHighLevelHigh\n";
}

static void atoStationShouldGoToStateAlarmingOnNormalLevelHighAndLowLevelLow() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::High);
    loop();
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::High));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnNormalLevelHighAndLowLevelLow\n";
}

static void atoStationShouldGoToStateDispensingOnNormalLevelLowAndLowLevelHigh() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::High));
    assert(atoStation.isInState(AtoStationState::Dispensing));
    assert(atoDispenser.isInState(Switched::On));
    assert(alarmStation.alarmList.isEmpty());

    std::cout << "ok -> atoStationShouldGoToStateDispensingOnNormalLevelLowAndLowLevelHigh\n";
}

static void atoStationShouldGoToStateAlarmingWhenMainSensorLowAndLowSensorLow() {
    /* given */
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingWhenMainSensorLowAndLowSensorLow\n";
}

static void atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelLow() {
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoHighLiquidLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoHighLiquidLevelSensor.setReading(LiquidLevelState::High);
    loop();
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoHighLiquidLevelSensor.isReading(LiquidLevelState::High));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 2);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelLow\n";
}

static void atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelHigh() {
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoHighLiquidLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoHighLiquidLevelSensor.setReading(LiquidLevelState::High);
    loop();
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::High);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoHighLiquidLevelSensor.isReading(LiquidLevelState::High));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::High));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 1);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoHighLevel));

    std::cout << "ok -> atoStationShouldGoToStateAlarmingOnHighLevelHighAndLowLevelHigh\n";
}

static void atoStationShouldGoToAlarmingStateWhenAllSensorsNotSensing() {
    /* given*/
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoHighLiquidLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoReservoirLowLiquidLevelSensor(&reservoirLowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    /* when */
    atoHighLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop();
    atoReservoirLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop();

    /* then */
    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoHighLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoReservoirLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 2);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));

    std::cout << "ok -> atoStationShouldGoToAlarmingStateWhenAllSensorsNotSensing\n";
}

static void atoStationShouldAcknowledgeAlarmsAndGoToStateSensing() {
    /* given*/
    currentMillis = getRandomUint32();
    Switchable atoDispenser{};
    AtoStation atoStation(atoSettings, atoDispenser);
    NormalLevelSensorConnection<AtoStation, LiquidLevelState> normalLevelSensorConnection(atoStation);
    HighLevelSensorConnection<AtoStation, LiquidLevelState> highLevelSensorConnection(atoStation);
    LowLevelSensorConnection<AtoStation, LiquidLevelState> lowLevelSensorConnection(atoStation);
    ReservoirLowLevelSensorConnection<AtoStation, LiquidLevelState> reservoirLowLevelSensorConnection(atoStation);
    Sensor<LiquidLevelState> atoNormalLevelSensor(&normalLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoHighLiquidLevelSensor(&highLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoLowLiquidLevelSensor(&lowLevelSensorConnection, LiquidLevelState::Unknown);
    Sensor<LiquidLevelState> atoReservoirLowLiquidLevelSensor(&reservoirLowLevelSensorConnection, LiquidLevelState::Unknown);
    MockBuzzer buzzer{};
    AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

    atoStation.attachAlarmStation(&alarmStation);
    setup();
    loop();

    atoHighLiquidLevelSensor.setReading(LiquidLevelState::Low);
    atoNormalLevelSensor.setReading(LiquidLevelState::Low);
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    atoReservoirLowLiquidLevelSensor.setReading(LiquidLevelState::Low);
    loop(atoSettings.maxDispensingDurationMs);

    assert(atoNormalLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoHighLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoReservoirLowLiquidLevelSensor.isReading(LiquidLevelState::Low));
    assert(atoStation.isInState(AtoStationState::Alarming));
    assert(atoDispenser.isInState(Switched::Off));
    assert(alarmStation.alarmList.size() == 2);
    assert(alarmStation.alarmList.contains(AlarmCode::AtoLowLevel));
    assert(alarmStation.alarmList.contains(AlarmCode::AtoReservoirLow));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AtoLowLevel));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));


    /* when */
    atoReservoirLowLiquidLevelSensor.setReading(LiquidLevelState::High);
    atoLowLiquidLevelSensor.setReading(LiquidLevelState::High);
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