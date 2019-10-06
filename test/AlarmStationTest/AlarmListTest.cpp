#include <iostream>
#include <cassert>
#include <chrono>

#include <Abstract/AbstractRunnable.h>
#include "Enums/AlarmCode.h"
#include "Enums/AlarmSeverity.h"
#include "AlarmStation/AlarmList.h"

#include "../_Mocks/MockCommon.h"

static void loop() {
    AbstractRunnable::loopAll();
    ++currentMillis;
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static void shouldAddAlarmToTheList() {

    /* when */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
    alarmList.add(AlarmCode::AtoHighLevel, AlarmSeverity::Major);

    /* then */
    assert(alarmList.contains(AlarmCode::AtoLowLevel));
    assert(alarmList.contains(AlarmCode::AtoHighLevel));
    assert(alarmList.size() == 2);
    assert(!alarmList.contains(AlarmCode::AtoReservoirLow));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldAddAlarmToTheList" << "\n";
}

static void shouldDropAlarmFromTheListWithSizeOne() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    assert(alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmList.size() == 1);

    /* when */
    alarmList.remove(AlarmCode::AmbientMaxHumidityReached);

    /* then */
    assert(!alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmList.isEmpty());

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldDropAlarmFromTheListWithSizeOne" << "\n";
}

static void shouldDropAlarmFromTheListWithSizeTwo() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmList.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmList.size() == 2);

    /* when */
    alarmList.remove(AlarmCode::AmbientMaxHumidityReached);
    alarmList.remove(AlarmCode::WaterMaxTemperatureReached);

    /* then */
    assert(!alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(!alarmList.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmList.isEmpty());

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldDropAlarmFromTheListWithSizeTwo" << "\n";
}

static void shouldNotDropAlarmNotInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmList.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmList.size() == 2);

    /* when */
    alarmList.remove(AlarmCode::SystemMaxTemperatureReached);

    /* then */
    assert(alarmList.size() == 2);
    assert(alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmList.contains(AlarmCode::WaterMaxTemperatureReached));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldNotDropAlarmNotInTheList" << "\n";
}

static void shouldDropAllAlarmsFromTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmList.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmList.size() == 2);

    /* when */
    alarmList.removeAll();

    /* then */
    assert(alarmList.isEmpty());

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldDropAllAlarmsFromTheList" << "\n";
}

static void shouldGetByAlarmCodeTheFirstAlarmFromTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmList.get(AlarmCode::AmbientMaxHumidityReached)->getCode() == AlarmCode::AmbientMaxHumidityReached);

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldGetByAlarmCodeTheFirstAlarmFromTheList" << "\n";
}

static void shouldGetByAlarmCodeTheSecondAlarmFromTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmList.get(AlarmCode::AtoReservoirLow)->getCode() == AlarmCode::AtoReservoirLow);

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldGetByAlarmCodeTheSecondAlarmFromTheList" << "\n";
}

static void shouldGetByAlarmCodeTheLastAlarmFromTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmList.get(AlarmCode::WaterMaxTemperatureReached)->getCode() == AlarmCode::WaterMaxTemperatureReached);

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldGetByAlarmCodeTheLastAlarmFromTheList" << "\n";
}

static void shouldGetNullPointerOnGetAlarmBylarmCodeNotInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmList.get(AlarmCode::WaterMinTemperatureReached) == nullptr);

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldGetNullPointerOnGetAlarmBylarmCodeNotInTheList" << "\n";
}

static void shouldAcknowledgeByAlarmCodeTheFirstAlarmInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when */
    alarmList.acknowledge(AlarmCode::WaterMaxTemperatureReached);

    /* then */
    assert(alarmList.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(!alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldAcknowledgeByAlarmCodeTheFirstAlarmInTheList" << "\n";
}

static void shouldAcknowledgeByAlarmCodeTheSecondAlarmInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);
    assert(!alarmList.isEmpty());

    /* when */
    alarmList.acknowledge(AlarmCode::AtoReservoirLow);

    /* then */
    assert(!alarmList.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldAcknowledgeByAlarmCodeTheSecondAlarmInTheList" << "\n";
}

static void shouldAcknowledgeByAlarmCodeTheLastAlarmInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when */
    alarmList.acknowledge(AlarmCode::AtoReservoirLow);

    /* then */
    assert(!alarmList.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldAcknowledgeByAlarmCodeTheLastAlarmInTheList" << "\n";
}

static void shouldNotAcknowledgeAlarmByAlarmCodeNotInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmList.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);
    loop();

    /* when */
    alarmList.acknowledge(AlarmCode::WaterMinTemperatureReached);

    /* then */
    assert(!alarmList.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(!alarmList.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldNotAcknowledgeAlarmByAlarmCodeNotInTheList" << "\n";
}

static void shouldGetFalseOnCheckIfAcknowledgedForAlarmNotInTheList() {

    /* given */
    currentMillis = getRandomUint32();
    AlarmList alarmList{};
    alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);

    /* when & then */
    assert(!alarmList.isAcknowledged(AlarmCode::WaterMinTemperatureReached));

    /* cleanup */
    alarmList.removeAll();
    assert(alarmList.isEmpty());

    std::cout << "ok -> shouldGetFalseOnCheckIfAcknowledgedForAlarmNotInTheList" << "\n";
}

int main(int argc, char *argv[]) {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    int repeat = 1;

    for (int i = 0; i < repeat; ++i) {
        shouldAddAlarmToTheList();

        shouldDropAlarmFromTheListWithSizeOne();
        shouldDropAlarmFromTheListWithSizeTwo();
        shouldNotDropAlarmNotInTheList();
        shouldDropAllAlarmsFromTheList();

        shouldGetByAlarmCodeTheFirstAlarmFromTheList();
        shouldGetByAlarmCodeTheSecondAlarmFromTheList();
        shouldGetByAlarmCodeTheLastAlarmFromTheList();
        shouldGetNullPointerOnGetAlarmBylarmCodeNotInTheList();

        shouldAcknowledgeByAlarmCodeTheFirstAlarmInTheList();
        shouldAcknowledgeByAlarmCodeTheSecondAlarmInTheList();
        shouldAcknowledgeByAlarmCodeTheLastAlarmInTheList();
        shouldNotAcknowledgeAlarmByAlarmCodeNotInTheList();

        shouldGetFalseOnCheckIfAcknowledgedForAlarmNotInTheList();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\n"
                 "------------------------------------------------------------" << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout << "------------------------------------------------------------" << "\n"
              << " >> TEST END" << "\n"
              << "------------------------------------------------------------" << "\n"
              << "\n";

    return 0;
}
