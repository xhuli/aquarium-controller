#define __TEST_MODE__

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <random>

//#include "gtest"

#include "../_Mocks/MockBuzzer.h"
#include "../_Mocks/MockCommon.h"

#include "AlarmStation/Alarm.h"
#include "AlarmStation/AlarmCode.h"
#include "AlarmStation/AlarmSeverity.h"
#include "AlarmStation/AlarmStation.h"

using namespace std;

static void shouldRaiseAlarm() {
    // when
    auto alarmStation = AlarmStation();
    auto *mockBuzzerPointer = new MockBuzzer();
    currentTime = 5641;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);

    // then
    assert(alarmStation.getNumberOfAlarms() == 1);
    assert(alarmStation.getAlarmByCode(AlarmCode::AmbientMaxHumidityReached)->timeStamp == currentTime);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::AmbientMaxHumidityReached);
    assert(!alarmStation.getAlarmByIndex(0)->isAcknowledged);
    assert(alarmStation.getAlarmByIndex(0)->isCritical == AlarmSeverity::Major);

    // reset
    delete mockBuzzerPointer;

    cout << "pass -> shouldRaiseAlarm" << "\n";
}

// /*
static void shouldGetAlarmByIndex() {
    // given
    auto alarmStation = AlarmStation();

    // when
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Major);

    // then
    assert(alarmStation.getNumberOfAlarms() == 3);
    assert(alarmStation.getAlarmByIndex(1)->timeStamp == currentTime - 1);
    assert(alarmStation.getAlarmByIndex(1)->code == AlarmCode::WaterMinTemperatureReached);
    assert(!alarmStation.getAlarmByIndex(1)->isAcknowledged);
    assert(alarmStation.getAlarmByIndex(1)->isCritical == AlarmSeverity::Critical);

    cout << "pass -> shouldGetAlarmByIndex" << "\n";
}

static void shouldGetNullPointerOnGetAlarmByInvalidIndex() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Major);
    assert(alarmStation.getNumberOfAlarms() == 3);

    // when && then
    assert(alarmStation.getAlarmByIndex(4) == nullptr);

    cout << "pass -> shouldGetNullPointerOnGetAlarmByInvalidIndex" << "\n";
}

static void shouldGetAlarmByCode() {
    // when
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Major);

    // then
    assert(alarmStation.getNumberOfAlarms() == 3);
    assert(alarmStation.getAlarmByCode(AlarmCode::WaterMinTemperatureReached)->timeStamp == currentTime - 1);
    assert(alarmStation.getAlarmByCode(AlarmCode::WaterMinTemperatureReached)->code == AlarmCode::WaterMinTemperatureReached);
    assert(!alarmStation.getAlarmByCode(AlarmCode::WaterMinTemperatureReached)->isAcknowledged);
    assert(alarmStation.getAlarmByCode(AlarmCode::WaterMinTemperatureReached)->isCritical == AlarmSeverity::Critical);

    cout << "pass -> shouldGetAlarmByCode" << "\n";
}

static void shouldGetNullPointerOnGetAlarmByInvalidCode() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    assert(alarmStation.getNumberOfAlarms() == 1);

    // when && then
    assert(alarmStation.getAlarmByCode(AlarmCode::ReservoirLow) == nullptr);

    cout << "pass -> shouldGetNullPointerOnGetAlarmByInvalidCode" << "\n";
}

static void shouldSortRaisedAlarmsByTimeStampLatestFirst() {
    // when
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);

    // then
    assert(alarmStation.getNumberOfAlarms() == 2);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::WaterMaxTemperatureReached);
    assert(alarmStation.getAlarmByIndex(1)->code == AlarmCode::AmbientMaxHumidityReached);

    cout << "pass -> shouldSortRaisedAlarmsByTimeStampLatestFirst" << "\n";
}

static void shouldDeleteRaisedAlarmByIndex() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmStation.getNumberOfAlarms() == 3);

    // when
    alarmStation.deleteAlarmByIndex(1);

    // then
    assert(alarmStation.getNumberOfAlarms() == 2);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::WaterMaxTemperatureReached);
    assert(alarmStation.getAlarmByIndex(1)->code == AlarmCode::AmbientMaxHumidityReached);

    cout << "pass -> shouldDeleteRaisedAlarmByIndex" << "\n";
}

static void shouldNotDeleteAlarmByInvalidIndex() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmStation.getNumberOfAlarms() == 3);

    // when && then
    assert(!alarmStation.deleteAlarmByIndex(4));
    assert(alarmStation.getNumberOfAlarms() == 3);

    cout << "pass -> shouldNotDeleteAlarmByInvalidIndex" << "\n";
}

static void shouldNotDeleteAlarmByIndexWhenThereAreNoAlarms() {
    // given
    auto alarmStation = AlarmStation();
    while (alarmStation.getNumberOfAlarms() > 0) {
        alarmStation.deleteAlarmByIndex(0);
    }

    // when && then
    assert(!alarmStation.deleteAlarmByIndex(0));

    cout << "pass -> shouldNotDeleteAlarmByIndexWhenThereAreNoAlarms" << "\n";
}

static void shouldDeleteRaisedAlarmByCode() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmStation.getNumberOfAlarms() == 3);

    // when
    alarmStation.deleteAlarmByCode(SystemMaxTemperatureReached);
//    alarmStation.deleteAlarmByCode(AmbientMaxHumidityReached);
//    alarmStation.deleteAlarmByCode(WaterMaxTemperatureReached);

    // then
    assert(alarmStation.getNumberOfAlarms() == 2);
    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::WaterMaxTemperatureReached);
//    assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::SystemMaxTemperatureReached);
//    assert(alarmStation.getAlarmByIndex(1)->code == AlarmCode::AmbientMaxHumidityReached);

    cout << "pass -> shouldDeleteRaisedAlarmByCode" << "\n";
}

static void shouldNotDeleteAlarmByInvalidCode() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);

    // when && then
    assert(!alarmStation.deleteAlarmByCode(AlarmCode::SystemMaxTemperatureReached));

    cout << "pass -> shouldDeleteRaisedAlarmByCode" << "\n";
}

static void shouldAcknowledgeRaisedAlarmByIndex() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmStation.getNumberOfAlarms() == 3);

    // when
    alarmStation.acknowledgeAlarmByIndex(1);

    // then
    assert(!alarmStation.getAlarmByIndex(0)->isAcknowledged);
    assert(alarmStation.getAlarmByIndex(1)->isAcknowledged);
    assert(!alarmStation.getAlarmByIndex(2)->isAcknowledged);

    cout << "pass -> shouldAcknowledgeRaisedAlarmByIndex" << "\n";
}

static void shouldNotAcknowledgeAlarmByInvalidIndex() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    assert(alarmStation.getNumberOfAlarms() == 1);

    // when && then
    assert(!alarmStation.acknowledgeAlarmByIndex(2));
    assert(!alarmStation.getAlarmByIndex(0)->isAcknowledged);

    cout << "pass -> shouldNotAcknowledgeAlarmByInvalidIndex" << "\n";
}

static void shouldAcknowledgeRaisedAlarmByCode() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Critical);
    assert(alarmStation.getNumberOfAlarms() == 3);

    // when
    alarmStation.acknowledgeAlarmByCode(AlarmCode::WaterMinTemperatureReached);

    // then
    assert(!alarmStation.getAlarmByCode(AlarmCode::AmbientMaxHumidityReached)->isAcknowledged);
    assert(alarmStation.getAlarmByCode(AlarmCode::WaterMinTemperatureReached)->isAcknowledged);
    assert(!alarmStation.getAlarmByCode(AlarmCode::WaterMaxTemperatureReached)->isAcknowledged);

    cout << "pass -> shouldAcknowledgeRaisedAlarmByCode" << "\n";
}

static void shouldNotAcknowledgeAlarmByInvalidCode() {
    // given
    auto alarmStation = AlarmStation();
    currentTime += 1;
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    assert(alarmStation.getNumberOfAlarms() == 1);

    // when && then
    assert(!alarmStation.acknowledgeAlarmByCode(AlarmCode::WaterMinTemperatureReached));
    assert(!alarmStation.getAlarmByIndex(0)->isAcknowledged);

    cout << "pass -> shouldNotAcknowledgeAlarmByInvalidCode" << "\n";
}

static void shouldNotifyListenersOnRaiseAlarm() {
    // given
    auto alarmStation = AlarmStation();
    auto *mockBuzzerPointer = new MockBuzzer();
    alarmStation.attachListener(mockBuzzerPointer);
    mockBuzzerPointer->mute();
    currentMillis += 1;

    // when
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);

    // then
    assert(alarmStation.getNumberOfAlarms() == 1);
    assert(mockBuzzerPointer->getIsBuzzing());

    cout << "pass -> shouldNotifyListenersOnRaiseAlarm" << "\n";

    // reset
    mockBuzzerPointer->mute();
    delete mockBuzzerPointer;
}

static void shouldNotifyListenersAboutCriticalAlarmsEvery_15Minutes() {
    // given
    auto alarmStation = AlarmStation();
    auto *mockBuzzerPointer = new MockBuzzer();
    alarmStation.attachListener(mockBuzzerPointer);
    mockBuzzerPointer->mute();
    currentMillis = getRandomUint32();
    alarmStation.update(currentMillis);
    alarmStation.raiseAlarm(AlarmCode::BackupHighSensorOn, AlarmSeverity::Critical);
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    assert(mockBuzzerPointer->getIsBuzzing());
    mockBuzzerPointer->mute();
    assert(mockBuzzerPointer->getIsNotBuzzing());

    // when && then
    currentMillis += (15ul * 60ul * 1000ul);
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsNotBuzzing());

    currentMillis += 1;
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsBuzzing());
    mockBuzzerPointer->mute();

    currentMillis += 5000; // let 5 seconds to pass
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsNotBuzzing());

    currentMillis += 1;
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsBuzzing());// -> buzz for the second alarm

    cout << "pass -> shouldNotifyListenersAboutCriticalAlarmsEvery_15Minutes" << "\n";

    // reset
    mockBuzzerPointer->mute();
    delete mockBuzzerPointer;
}

static void shouldNotifyListenersAboutMajorAlarmsEveryHour() {
    // given
    auto alarmStation = AlarmStation();
    auto *mockBuzzerPointer = new MockBuzzer();
    alarmStation.attachListener(mockBuzzerPointer);
    mockBuzzerPointer->mute();
    currentMillis = getRandomUint32();
    alarmStation.update(currentMillis);
    alarmStation.raiseAlarm(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Major);
    alarmStation.raiseAlarm(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Major);
    assert(mockBuzzerPointer->getIsBuzzing());
    mockBuzzerPointer->mute();
    assert(mockBuzzerPointer->getIsNotBuzzing());

    // when && then
    currentMillis += (60ul * 60ul * 1000ul);
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsNotBuzzing());

    currentMillis += 1;
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsBuzzing()); // -> buzz for the first alarm
    mockBuzzerPointer->mute();
    assert(mockBuzzerPointer->getIsNotBuzzing());

    currentMillis += 5000; // let 5 seconds to pass
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsNotBuzzing());
    currentMillis += 1;
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsBuzzing());// -> buzz for the second alarm

    cout << "pass -> shouldNotifyListenersAboutMajorAlarmsEveryHour" << "\n";

    // reset
    mockBuzzerPointer->mute();
    delete mockBuzzerPointer;
}

static void shouldNotifyListenersAboutCriticalAndMajorAlarms() {
    // given
    auto alarmStation = AlarmStation();
    auto *mockBuzzerPointer = new MockBuzzer();
    alarmStation.attachListener(mockBuzzerPointer);
    mockBuzzerPointer->mute();
    currentMillis = getRandomUint32();
    alarmStation.update(currentMillis);
    alarmStation.raiseAlarm(AlarmCode::BackupHighSensorOn, AlarmSeverity::Major);
    alarmStation.raiseAlarm(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Critical);
    assert(mockBuzzerPointer->getIsBuzzing());
    mockBuzzerPointer->mute();
    assert(mockBuzzerPointer->getIsNotBuzzing());

    // when && then
    for (int count = 0; count < 3; count++) {
        currentMillis += (15ul * 60ul * 1000ul) + 1;
        alarmStation.update(currentMillis);
        assert(mockBuzzerPointer->getIsBuzzing()); // critical notification after 15 min
        mockBuzzerPointer->mute();

        currentMillis += 5001; // let 5 seconds to pass
        alarmStation.update(currentMillis);
        assert(mockBuzzerPointer->getIsNotBuzzing()); // no major notification
    }

    currentMillis += (30ul * 60ul * 1000ul) + 1; // 60 minutes after alarms are raised
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsBuzzing()); // critical notification received
    mockBuzzerPointer->mute();

    currentMillis += 5001; // let 5 seconds to pass
    alarmStation.update(currentMillis);
    assert(mockBuzzerPointer->getIsBuzzing()); // major notification received

    cout << "pass -> shouldNotifyListenersAboutCriticalAndMajorAlarms" << "\n";

    // reset
    mockBuzzerPointer->mute();
    delete mockBuzzerPointer;
}

// */

int main(int argc, char *argv[]) {

    cout << "\n"
         << "------------------------------------------------------------" << "\n"
         << " >> TEST START" << "\n"
         << "------------------------------------------------------------" << "\n";

    shouldRaiseAlarm();
    shouldSortRaisedAlarmsByTimeStampLatestFirst();
    shouldGetAlarmByIndex();
    shouldGetNullPointerOnGetAlarmByInvalidIndex();
    shouldGetAlarmByCode();
    shouldGetNullPointerOnGetAlarmByInvalidCode();
    shouldDeleteRaisedAlarmByIndex();
    shouldNotDeleteAlarmByInvalidIndex();
    shouldNotDeleteAlarmByIndexWhenThereAreNoAlarms();
    shouldDeleteRaisedAlarmByCode();
    shouldNotDeleteAlarmByInvalidCode();
    shouldAcknowledgeRaisedAlarmByIndex();
    shouldNotAcknowledgeAlarmByInvalidIndex();
    shouldAcknowledgeRaisedAlarmByCode();
    shouldNotAcknowledgeAlarmByInvalidCode();
    shouldNotifyListenersOnRaiseAlarm();
    shouldNotifyListenersAboutCriticalAlarmsEvery_15Minutes();
    shouldNotifyListenersAboutMajorAlarmsEveryHour();
    shouldNotifyListenersAboutCriticalAndMajorAlarms();

    cout << "------------------------------------------------------------" << "\n"
         << " >> TEST END" << "\n"
         << "------------------------------------------------------------" << "\n"
         << "\n";

    return 0;
}
