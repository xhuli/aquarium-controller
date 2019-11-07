#define __TEST_MODE__

#include <assert.h>
#include <iostream>

#include "../_Mocks/MockCommon.h"

#include <Abstract/AbstractRunnable.h>
#include <chrono>

#include "Enums/AlarmCode.h"
#include "Enums/AlarmSeverity.h"
#include "AlarmStation/AlarmStation.h"
#include "AlarmStation/AlarmNotifyConfiguration.h"

#include "../_Mocks/MockBuzzer.h"

static LinkedMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};
static AlarmNotifyConfiguration defaultConfiguration{5, 5000};

static void setup() {
    AbstractRunnable::setupAll();
}

static void loop() {
    AbstractRunnable::loopAll();
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        // if (ms % 1000 == 0) std::cout << "ms: " << ms << "\n";
        loop();
    }
}

static void shouldStartSoundNotificationOnRaiseAlarmWhenNoAlarmsInQueue() {
    /* given */
    MockBuzzer mockBuzzer{};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);

    assert(!mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));

    /* when */
    alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);
    loop();

    /* then */
    assert(mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::On));

    std::cout << "ok -> shouldStartSoundNotificationOnRaiseAlarmWhenNoAlarmsInQueue\n";
}

static void shouldStopSoundNotificationOnRaisedAlarmAfterSoundNotifyDuration() {
    /* given */
    uint16_t buzzerRestPeriodMs = 5000;
    MockBuzzer mockBuzzer{buzzerRestPeriodMs};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);

    assert(!mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));

    /* when */
    alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);

    Alarm *firstAlarmInQueue = alarmStation.alarmList.getFirst();
    AlarmNotifyConfiguration alarmNotifyConfiguration = alarmNotifyConfigurations.getOrDefault(firstAlarmInQueue->getSeverity(), defaultConfiguration);
    loop(alarmNotifyConfiguration.getSoundDurationMs());

    /* then */
    assert(mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::On));
    loop();
    assert(mockBuzzer.isBusy()); // <- in rest
    assert(mockBuzzer.isInState(Switched::Off));

    std::cout << "ok -> shouldStopSoundNotificationOnRaisedAlarmAfterSoundNotifyDuration\n";
}

static void shouldNotStartSoundNotificationOnRaisedAlarmWhenBuzzerInRestPeriod() {
    /* given */
    uint16_t buzzerRestPeriodMs = 5000;
    MockBuzzer mockBuzzer{buzzerRestPeriodMs};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);

    assert(!mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));

    alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);

    Alarm *firstAlarmInQueue = alarmStation.alarmList.getFirst();
    AlarmNotifyConfiguration alarmNotifyConfiguration = alarmNotifyConfigurations.getOrDefault(firstAlarmInQueue->getSeverity(), defaultConfiguration);
    loop(alarmNotifyConfiguration.getSoundDurationMs());
    loop();

    /* when */
    alarmStation.alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);

    /* then */
    assert(mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));

    std::cout << "ok -> shouldNotStartSoundNotificationOnRaisedAlarmWhenBuzzerInRestPeriod\n";
}

static void shouldStartNextSoundNotificationAfterBuzzerRestPeriod() {
    /* given */
    uint16_t buzzerRestPeriodMs = 5000;

    MockBuzzer mockBuzzer{buzzerRestPeriodMs};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);

    assert(!mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));

    alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);
    alarmStation.alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Minor);

    Alarm *firstAlarmInQueue = alarmStation.alarmList.getFirst();
    AlarmNotifyConfiguration alarmNotifyConfiguration = alarmNotifyConfigurations.getOrDefault(firstAlarmInQueue->getSeverity(), defaultConfiguration);
    loop(alarmNotifyConfiguration.getSoundDurationMs());

    /* when */
    loop(buzzerRestPeriodMs);
    assert(mockBuzzer.isBusy());
    loop();
    assert(!mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));
    loop();

    /* then */
    assert(mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::On));

    std::cout << "ok -> shouldStartNextSoundNotificationAfterBuzzerRestPeriod\n";
}

static void shouldPeriodicallySoundAlarms() {
    /* given */
    uint16_t buzzerRestPeriodMs = 5000;

    MockBuzzer mockBuzzer{buzzerRestPeriodMs};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);

    assert(!mockBuzzer.isBusy());
    assert(mockBuzzer.isInState(Switched::Off));

    alarmStation.alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Major);
    alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);

    int majorAlarmNotifyPeriodMinutes = static_cast<int>(alarmNotifyConfigurations
            .getOrDefault(AlarmSeverity::Major, defaultConfiguration)
            .getSoundPeriodMinutes());

    /* when & then */
    Switched buzzerSwitchedState = mockBuzzer.getState();
    bool buzzerBusyState = mockBuzzer.isBusy();
    for (int ms = 0; ms < majorAlarmNotifyPeriodMinutes * 60 * 1000; ++ms) {
        int seconds = ms / 1000;
        if (mockBuzzer.getState() != buzzerSwitchedState) {
            buzzerSwitchedState = mockBuzzer.getState();
            if (mockBuzzer.isInState(Switched::On)) {
                //std::cout << (seconds) << "\t\t switched :: ON\n";
                assert(
                        seconds == 0 || // <- the major alarm
                        seconds == 10 || seconds == 70 || seconds == 130 || seconds == 190 || seconds == 250 || // <- the critical alarm, 1 min period
                        seconds == 300 || // <- the major alarm, 5min period
                        seconds == 310 || seconds == 370 || seconds == 430 || seconds == 490 || seconds == 550  // <- the critical alarm
                );
            } else {
                //std::cout << (seconds) << "\t\t switched :: OFF\n";
                assert(
                        seconds == 5 || // <- the major alarm
                        seconds == 17 || seconds == 77 || seconds == 137 || seconds == 197 || seconds == 257 || // <- the critical alarm, 1 min period
                        seconds == 305 || // <- the major alarm, 5min period
                        seconds == 317 || seconds == 377 || seconds == 437 || seconds == 497 || seconds == 557 // <- the critical alarm, 1 min period
                );
            }
        }

        if (mockBuzzer.isBusy() != buzzerBusyState) {
            buzzerBusyState = mockBuzzer.isBusy();
            if (buzzerBusyState) {
                //std::cout << (seconds) << "\tbusy :: " << static_cast<int>(buzzerBusyState) << "\n";
                assert(
                        seconds == 0 || // <- the major alarm
                        seconds == 10 || seconds == 70 || seconds == 130 || seconds == 190 || seconds == 250 || // <- the critical alarm, 1 min period
                        seconds == 300 || // <- the major alarm, 5min period
                        seconds == 310 || seconds == 370 || seconds == 430 || seconds == 490 || seconds == 550  // <- the critical alarm
                );
            } else {
                //std::cout << (seconds) << "\tbusy :: " << static_cast<int>(buzzerBusyState) << "\n";
                assert(
                        seconds == 10 || // <- end of the major alarm notification + rest period
                        seconds == 22 || seconds == 82 || seconds == 142 || seconds == 202 || seconds == 262 ||
                        // <- end of the critical alarm notification + rest period
                        seconds == 310 || // <- end of the major alarm notification + rest period, 5min period
                        seconds == 322 || seconds == 382 || seconds == 442 || seconds == 502 || seconds == 562 // <- the critical alarm, 1 min period
                );
            }
        }
        loop();
    }

    std::cout << "ok -> shouldPeriodicallySoundAlarms\n";
}

static void shouldGoToStateSleepingOnStartSleeping() {
    /* given */
    MockBuzzer mockBuzzer{1000};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);
    alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);

    setup();
    loop();

    //when
    assert(mockBuzzer.isInState(Switched::On));
    alarmStation.startSleeping(10);
    loop();

    /* then */
    assert(mockBuzzer.isInState(Switched::Off));
    assert(alarmStation.isInState(State::Sleeping));

    std::cout << "ok -> shouldGoToStateSleepingOnStartSleeping\n";
}

static void shouldGoToStateActiveAfterSleepPeriod() {
    /* given */
    MockBuzzer mockBuzzer{1000};
    AlarmStation alarmStation(mockBuzzer, alarmNotifyConfigurations);

    setup();
    loop();

    uint32_t sleepMs = 3;
    alarmStation.startSleeping(sleepMs);

    //when
    loop(sleepMs);
    assert(alarmStation.isInState(State::Sleeping));
    loop();

    /* then */
    assert(alarmStation.isInState(State::Active));

    std::cout << "ok -> shouldGoToStateActiveAfterSleepPeriod\n";
}

int main(int argc, char *argv[]) {

    alarmNotifyConfigurations.put(AlarmSeverity::Critical, AlarmNotifyConfiguration(1, 7000));
    alarmNotifyConfigurations.put(AlarmSeverity::Major, AlarmNotifyConfiguration(5, 5000));
    alarmNotifyConfigurations.put(AlarmSeverity::Minor, AlarmNotifyConfiguration(15, 3000));
    alarmNotifyConfigurations.put(AlarmSeverity::NoSeverity, AlarmNotifyConfiguration(60, 1000));

    std::cout << "\n"
              << "------------------------------------------------------------\n"
              << " >> TEST START\n"
              << "------------------------------------------------------------\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldStartSoundNotificationOnRaiseAlarmWhenNoAlarmsInQueue();
        shouldStopSoundNotificationOnRaisedAlarmAfterSoundNotifyDuration();
        shouldNotStartSoundNotificationOnRaisedAlarmWhenBuzzerInRestPeriod();
        shouldStartNextSoundNotificationAfterBuzzerRestPeriod();
        shouldPeriodicallySoundAlarms();

        shouldGoToStateSleepingOnStartSleeping();
        shouldGoToStateActiveAfterSleepPeriod();

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
