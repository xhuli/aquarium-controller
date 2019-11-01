#include <iostream>
#include <cassert>
#include <chrono>

#include <Abstract/AbstractRunnable.h>
#include "Enums/AlarmCode.h"
#include "Enums/AlarmSeverity.h"
#include "AlarmStation/AlarmArray.h"

#include "../_Mocks/MockCommon.h"

static void loop() {
    AbstractRunnable::loopAll();
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static void shouldAddAlarmToTheArray() {

    /* when */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
    alarmArray.add(AlarmCode::AtoHighLevel, AlarmSeverity::Major);

    /* then */
    assert(alarmArray.contains(AlarmCode::AtoLowLevel));
    assert(alarmArray.contains(AlarmCode::AtoHighLevel));
    assert(alarmArray.size() == 2);
    assert(!alarmArray.contains(AlarmCode::AtoReservoirLow));

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldAddAlarmToTheArray" << "\n";
}

static void shouldDropAlarmFromTheArrayWithSizeOne() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    assert(alarmArray.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmArray.size() == 1);

    /* when */
    alarmArray.remove(AlarmCode::AmbientMaxHumidityReached);

    /* then */
    assert(!alarmArray.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmArray.isEmpty());

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldDropAlarmFromTheArrayWithSizeOne" << "\n";
}

static void shouldDropAlarmFromTheArrayWithSizeTwo() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmArray.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmArray.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmArray.size() == 2);

    /* when */
    alarmArray.remove(AlarmCode::AmbientMaxHumidityReached);
    alarmArray.remove(AlarmCode::WaterMaxTemperatureReached);

    /* then */
    assert(!alarmArray.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(!alarmArray.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmArray.isEmpty());

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldDropAlarmFromTheArrayWithSizeTwo" << "\n";
}

static void shouldDropAllAlarmsFromTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmArray.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmArray.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmArray.size() == 2);

    /* when */
    alarmArray.removeAll();

    /* then */
    assert(alarmArray.isEmpty());

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldDropAllAlarmsFromTheArray" << "\n";
}

static void shouldGetTheFirstAlarmByAlarmCodeFromTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmArray.get(AlarmCode::AmbientMaxHumidityReached).getCode() == AlarmCode::AmbientMaxHumidityReached);

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldGetTheFirstAlarmByAlarmCodeFromTheArray" << "\n";
}

static void shouldGetTheSecondAlarmByAlarmCodeFromTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmArray.get(AlarmCode::AtoReservoirLow).getCode() == AlarmCode::AtoReservoirLow);

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldGetTheSecondAlarmByAlarmCodeFromTheArray" << "\n";
}

static void shouldGetTheLastAlarmByAlarmCodeFromTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(alarmArray.get(AlarmCode::WaterMaxTemperatureReached).getCode() == AlarmCode::WaterMaxTemperatureReached);

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldGetTheLastAlarmByAlarmCodeFromTheArray" << "\n";
}

static void shouldGetNullPointerOnGetAlarmByAlarmCodeNotInTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when & then */
    assert(!alarmArray.get(AlarmCode::WaterMinTemperatureReached).isActive());

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldGetNullPointerOnGetAlarmByAlarmCodeNotInTheArray" << "\n";
}

static void shouldAcknowledgeByAlarmCodeTheFirstAlarmInTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when */
    alarmArray.acknowledge(AlarmCode::WaterMaxTemperatureReached);

    /* then */
    assert(alarmArray.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(!alarmArray.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmArray.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldAcknowledgeByAlarmCodeTheFirstAlarmInTheArray" << "\n";
}

static void shouldAcknowledgeByAlarmCodeTheSecondAlarmInTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);
    assert(!alarmArray.isEmpty());

    /* when */
    alarmArray.acknowledge(AlarmCode::AtoReservoirLow);

    /* then */
    assert(!alarmArray.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmArray.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmArray.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldAcknowledgeByAlarmCodeTheSecondAlarmInTheArray" << "\n";
}

static void shouldAcknowledgeByAlarmCodeTheLastAlarmInTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);

    /* when */
    alarmArray.acknowledge(AlarmCode::AtoReservoirLow);

    /* then */
    assert(!alarmArray.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmArray.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmArray.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldAcknowledgeByAlarmCodeTheLastAlarmInTheArray" << "\n";
}

static void shouldNotAcknowledgeAlarmByAlarmCodeNotInTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);
    alarmArray.add(AlarmCode::AtoReservoirLow, AlarmSeverity::Minor);
    alarmArray.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::NoSeverity);
    loop();

    /* when */
    alarmArray.acknowledge(AlarmCode::WaterMinTemperatureReached);

    /* then */
    assert(!alarmArray.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));
    assert(!alarmArray.isAcknowledged(AlarmCode::AtoReservoirLow));
    assert(!alarmArray.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldNotAcknowledgeAlarmByAlarmCodeNotInTheArray" << "\n";
}

static void shouldGetFalseOnCheckIfAcknowledgedForAlarmNotInTheArray() {

    /* given */
    AlarmArray<10> alarmArray{};
    alarmArray.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Critical);

    /* when & then */
    assert(!alarmArray.isAcknowledged(AlarmCode::WaterMinTemperatureReached));

    /* cleanup */
    alarmArray.removeAll();
    assert(alarmArray.isEmpty());

    std::cout << "ok -> shouldGetFalseOnCheckIfAcknowledgedForAlarmNotInTheArray" << "\n";
}

int main(int argc, char *argv[]) {

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1; ++i) {

        shouldAddAlarmToTheArray();

        shouldDropAlarmFromTheArrayWithSizeOne();
        shouldDropAlarmFromTheArrayWithSizeTwo();
        shouldDropAllAlarmsFromTheArray();

        shouldGetTheFirstAlarmByAlarmCodeFromTheArray();
        shouldGetTheSecondAlarmByAlarmCodeFromTheArray();
        shouldGetTheLastAlarmByAlarmCodeFromTheArray();
        shouldGetNullPointerOnGetAlarmByAlarmCodeNotInTheArray();

        shouldAcknowledgeByAlarmCodeTheFirstAlarmInTheArray();
        shouldAcknowledgeByAlarmCodeTheSecondAlarmInTheArray();
        shouldAcknowledgeByAlarmCodeTheLastAlarmInTheArray();
        shouldNotAcknowledgeAlarmByAlarmCodeNotInTheArray();

        shouldGetFalseOnCheckIfAcknowledgedForAlarmNotInTheArray();
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\n"
                 "------------------------------------------------------------" << "\n";
    std::cout << "Elapsed time: " << elapsed.

            count()

              << " s\n";
    std::cout << "------------------------------------------------------------" << "\n"
              << " >> TEST END" << "\n"
              << "------------------------------------------------------------" << "\n"
              << "\n";

    return 0;
}
