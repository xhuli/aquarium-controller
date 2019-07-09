#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "AlarmStation/AlarmCode.h"
#include "AlarmStation/AlarmStation.h"
#include "TemperatureControlStation/TemperatureControlStation.h"

#include "../_Mocks/MockTemperatureSensor.h"
#include "../_Mocks/MockTemperatureAndHumiditySensor.h"
#include "../_Mocks/MockStorage.h"
#include "../_Mocks/MockDevice.h"
#include "../_Mocks/MockHeater.h"

using namespace std;

AlarmStation alarmStation = AlarmStation();

MockTemperatureSensor *mockSystemTemperatureSensor = new MockTemperatureSensor();
MockTemperatureSensor *mockWaterTemperatureSensor = new MockTemperatureSensor();
MockTemperatureAndHumiditySensor *mockAmbientTemperatureAndHumiditySensor = new MockTemperatureAndHumiditySensor();

MockStorage *mockStoragePointer = new MockStorage();

MockHeater *mockedWaterHeater = new MockHeater();
MockDevice *mockedWaterCooler = new MockDevice();
MockDevice *mockedAmbientFanCooler = new MockDevice();
MockDevice *mockedSystemFanCooler = new MockDevice();

TemperatureControlSettings settings = TemperatureControlSettings();
TemperatureControlStation temperatureControlStation = TemperatureControlStation();

static void should_NotStartWaterHeatingWhenHeatControlEnabledAndNoHeatingDeviceAttached() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.enableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOff());

    cout << "pass -> should_NotStartWaterHeatingWhenHeatControlEnabledAndNoHeatingDeviceAttached\n";
}

static void should_NotStartWaterCoolingWhenCoolControlEnabledAndNoCoolingDeviceAttached() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOff());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOff());

    cout << "pass -> should_NotStartWaterCoolingWhenCoolControlEnabledAndNoCoolingDeviceAttached\n";
}

static void should_NotStartWaterHeatingWhenHeatControlDisabled() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.disableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOff());

    cout << "pass -> should_NotStartWaterHeatingWhenHeatControlDisabled\n";
}

static void should_NotStartWaterHeatingWhenHeatControlEnabledAndSensorReadingIsBelowZero() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.enableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(-1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOff());
    if (temperatureControlStation.hasAlarmStation) {
        assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::WaterMinTemperatureReached);
        assert(alarmStation.getAlarmByIndex(0)->isCritical == 1);
        assert(alarmStation.getAlarmByIndex(0)->isAcknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    cout << "pass -> should_NotStartWaterHeatingWhenHeatControlEnabledAndSensorReadingIsBelowZero\n";

    // reset
    (temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0));
    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void should_StartWaterHeatingWhenHeatControlEnabledAndTemperatureBelowStopHeatTempC() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.enableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOn());

    cout << "pass -> should_StartWaterHeatingWhenHeatControlEnabledAndTemperatureBelowStopHeatTempC\n";
}

static void should_NotInterveneWhenHeatingAndHeatControlEnabledAndTemperatureEqualToStopHeatTempC() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOn());

    // when
    temperatureControlStation.enableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOn());

    cout << "pass -> should_NotInterveneWhenHeatingAndHeatControlEnabledAndTemperatureEqualToStopHeatTempC\n";
}

static void should_NotInterveneWhenNotHeatingAndHeatControlEnabledAndTemperatureEqualToStopHeatTempC() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.enableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOff());

    cout << "pass -> should_NotInterveneWhenNotHeatingAndHeatControlEnabledAndTemperatureEqualToStopHeatTempC\n";
}

static void should_StopWaterHeatingWhenHeatControlEnabledAndTemperatureAboveStopHeatTempC() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.enableWaterHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOff());

    cout << "pass -> should_StopWaterHeatingWhenHeatControlEnabledAndTemperatureAboveStopHeatTempC\n";
}

static void should_NotStartWaterCoolingWhenCoolControlDisabled() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOff());

    // when
    temperatureControlStation.disableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOff());

    cout << "pass -> should_NotStartWaterCoolingWhenCoolControlDisabled\n";
}

static void should_StartWaterCoolingWhenCoolControlEnabledAndTemperatureAboveStartCoolTempC() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOff());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOn());

    cout << "pass -> should_StartWaterCoolingWhenCoolControlEnabledAndTemperatureAboveStartCoolTempC\n";
}

static void should_NotInterveneWhenCoolingAndCoolControlEnabledAndTemperatureEqualToStartCoolTempC() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOn());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOn());

    cout << "pass -> should_NotInterveneWhenCoolingAndCoolControlEnabledAndTemperatureEqualToStartCoolTempC\n";
}

static void should_NotInterveneWhenNotCoolingAndCoolControlEnabledAndTemperatureEqualToStartCoolTempC() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOff());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOff());

    cout << "pass -> should_NotInterveneWhenNotCoolingAndCoolControlEnabledAndTemperatureEqualToStartCoolTempC\n";
}

static void should_StopWaterCoolingWhenCoolControlEnabledAndTemperatureBelowStartCoolTempC() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOn());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOff());

    cout << "pass -> should_StopWaterCoolingWhenCoolControlEnabledAndTemperatureBelowStartCoolTempC\n";
}

static void should_RaiseAlarmWhenWaterTemperatureAboveMaxTemp() {
    // given
    currentMillis = 0;
    mockedWaterCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterCooler->isOff());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.waterMaxTemperatureCelsiusAlarmTrigger + 1.0f);
    currentMillis += 1;
    currentTime += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterCooler->isOn());
    if (temperatureControlStation.hasAlarmStation) {
        assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::WaterMaxTemperatureReached);
        assert(alarmStation.getAlarmByIndex(0)->isCritical == 1);
        assert(alarmStation.getAlarmByIndex(0)->isAcknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    cout << "pass -> should_RaiseAlarmWhenWaterTemperatureAboveMaxTemp\n";

    // reset
    (temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0));
    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void should_RaiseAlarmWhenWaterTemperatureBelowMinTemp() {
    // given
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.enableWaterCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.waterMinTemperatureCelsiusAlarmTrigger - 1.0f);
    currentMillis += 1;
    currentTime += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedWaterHeater->isOn());
    if (temperatureControlStation.hasAlarmStation) {
        assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::WaterMinTemperatureReached);
        assert(alarmStation.getAlarmByIndex(0)->isCritical == 1);
        assert(alarmStation.getAlarmByIndex(0)->isAcknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    cout << "pass -> should_RaiseAlarmWhenWaterTemperatureBelowMinTemp\n";

    // reset
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    (temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0));
    assert(alarmStation.getNumberOfAlarms() == 0);
}


static void should_NotStartAmbientCoolingWhenAmbientTemperatureBelowAmbientStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOff());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_NotStartAmbientCoolingWhenAmbientTemperatureBelowAmbientStartCoolingTempC\n";
}

static void should_NotStartAmbientCoolingWhenAmbientTemperatureEqualToAmbientStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOff());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_NotStartAmbientCoolingWhenAmbientTemperatureEqualToAmbientStartCoolingTempC\n";
}

static void should_NotStopAmbientCoolingWhenAmbientTemperatureEqualToAmbientStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());

    cout << "pass -> should_NotStopAmbientCoolingWhenAmbientTemperatureEqualToAmbientStartCoolingTempC\n";
}

static void should_StartAmbientCoolingWhenAmbientTemperatureAboveAmbientStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOff());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());

    cout << "pass -> should_StartAmbientCoolingWhenAmbientTemperatureAboveAmbientStartCoolingTempC\n";
}

static void should_StopAmbientCoolingWhenAmbientTemperatureBelowAmbientStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_StopAmbientCoolingWhenAmbientTemperatureBelowAmbientStartCoolingTempC\n";
}

static void should_RaiseAlarmWhenAmbientTemperatureAboveMaxAmbientTempC() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.ambientMaxTemperatureCelsiusAlarmTrigger + 1.0f);
    currentMillis += 1;
    currentTime += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());
    if (temperatureControlStation.hasAlarmStation) {
        assert(alarmStation.getAlarmByIndex(0)->code == AlarmCode::AmbientMaxTemperatureReached);
        assert(alarmStation.getAlarmByIndex(0)->isCritical == 0);
        assert(alarmStation.getAlarmByIndex(0)->isAcknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    cout << "pass -> should_RaiseAlarmWhenAmbientTemperatureAboveMaxAmbientTempC\n";

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.ambientMaxTemperatureCelsiusAlarmTrigger - 1.0f);

    // reset
    (temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0));
    assert(alarmStation.getNumberOfAlarms() == 0);
}


static void should_NotStartAmbientVentingWhenAmbientHumidityBelowAmbientStartVentingHumidityPercent() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientHumiditySensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOff());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_NotStartAmbientVentingWhenAmbientHumidityBelowAmbientStartVentingHumidityPercent\n";
}

static void should_NotStartAmbientVentingWhenAmbientHumidityEqualToAmbientStartHumidityPercent() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientHumiditySensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOff());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_NotStartAmbientVentingWhenAmbientHumidityEqualToAmbientStartHumidityPercent\n";
}

static void should_NotStopAmbientVentingWhenAmbientHumidityEqualToAmbientStartVentingHumidityPercent() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());

    cout << "pass -> should_NotStopAmbientVentingWhenAmbientHumidityEqualToAmbientStartVentingHumidityPercent\n";
}

static void should_StartAmbientVentingWhenAmbientHumidityAboveAmbientStartVentingHumidityPercent() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOff());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());

    cout << "pass -> should_StartAmbientVentingWhenAmbientHumidityAboveAmbientStartVentingHumidityPercent\n";
}

static void should_StopAmbientVentingWhenAmbientHumidityBelowAmbientStartVentingHumidityPercent() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_StopAmbientVentingWhenAmbientHumidityBelowAmbientStartVentingHumidityPercent\n";
}

static void should_RaiseAlarmWhenAmbientHumidityAboveMaxAmbientHumidityPercent() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger + 1.0f);
    currentMillis += 1;
    currentTime += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());
    if (temperatureControlStation.hasAlarmStation) {
        assert(alarmStation.getAlarmByCode(5)->code == AlarmCode::AmbientMaxHumidityReached);
        assert(alarmStation.getAlarmByIndex(0)->isCritical == 0);
        assert(alarmStation.getAlarmByIndex(0)->isAcknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    cout << "pass -> should_RaiseAlarmWhenAmbientHumidityAboveMaxAmbientHumidityPercent\n";

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger - 1.0f);
    (temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0));
    assert(alarmStation.getNumberOfAlarms() == 0);
}


static void should_NotStopAmbientFanWhenHighAmbientHumidityAboveAndLowAmbientTemperature() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.ambientMaxTemperatureCelsiusAlarmTrigger - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());

    cout << "pass -> should_NotStopAmbientFanWhenHighAmbientHumidityAboveAndLowAmbientTemperature\n";

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger - 1.0f);
}

static void should_NotStopAmbientFanWhenLowAmbientHumidityAboveAndHighAmbientTemperature() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOn());

    cout << "pass -> should_NotStopAmbientFanWhenLowAmbientHumidityAboveAndHighAmbientTemperature\n";

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
}

static void should_StopAmbientFanWhenLowAmbientHumidityAboveAndLowAmbientTemperature() {
    // given
    currentMillis = 0;
    mockedAmbientFanCooler->mockNotStarted();
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());

    // when
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedAmbientFanCooler->isOn());
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedAmbientFanCooler->isOff());

    cout << "pass -> should_StopAmbientFanWhenLowAmbientHumidityAboveAndLowAmbientTemperature\n";

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger - 1.0f);
}


static void should_NotStartSystemCoolingWhenSystemTemperatureBelowSystemStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockNotStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOff());

    // when
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedSystemFanCooler->isOff());

    cout << "pass -> should_NotStartSystemCoolingWhenSystemTemperatureBelowSystemStartCoolingTempC\n";
}

static void should_NotStartSystemCoolingWhenSystemTemperatureEqualToSystemStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockNotStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOff());

    // when
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedSystemFanCooler->isOff());

    cout << "pass -> should_NotStartSystemCoolingWhenSystemTemperatureEqualToSystemStartCoolingTempC\n";
}

static void should_NotStopSystemCoolingWhenSystemTemperatureEqualToSystemStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOn());

    // when
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedSystemFanCooler->isOn());

    cout << "pass -> should_NotStopSystemCoolingWhenSystemTemperatureEqualToSystemStartCoolingTempC\n";
}

static void should_StartSystemCoolingWhenSystemTemperatureAboveSystemStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockNotStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOff());

    // when
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedSystemFanCooler->isOn());

    cout << "pass -> should_StartSystemCoolingWhenSystemTemperatureAboveSystemStartCoolingTempC\n";
}

static void should_StopSystemCoolingWhenSystemTemperatureBelowSystemStartCoolingTempC() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOn());

    // when
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedSystemFanCooler->isOff());

    cout << "pass -> should_StopSystemCoolingWhenSystemTemperatureBelowSystemStartCoolingTempC\n";
}

static void should_RaiseAlarmWhenSystemTemperatureAboveMaxSystemTempC() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    currentTime += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOn());

    // when
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.systemMaxTemperatureCelsiusAlarmTrigger + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedSystemFanCooler->isOn());
    if (temperatureControlStation.hasAlarmStation) {
        assert(alarmStation.getAlarmByCode(1)->code == AlarmCode::SystemMaxTemperatureReached);
        assert(alarmStation.getAlarmByIndex(0)->isCritical == 0);
        assert(alarmStation.getAlarmByIndex(0)->isAcknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    cout << "pass -> should_RaiseAlarmWhenSystemTemperatureAboveMaxSystemTempC\n";

    // reset
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.systemMaxTemperatureCelsiusAlarmTrigger - 1.0f);
    (temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByCode(AlarmCode::SystemMaxTemperatureReached));
    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void should_GoToSleepStateOnSleep() {
    // given
    currentMillis = 0;
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    temperatureControlStation.attachAmbientCoolingDevice(mockedAmbientFanCooler);
    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.startAmbientCoolingTemperatureCelsius + 1.0f);
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent + 1.0f);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startWaterCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOn());
    assert(mockedAmbientFanCooler->isOn());
    assert(mockedWaterCooler->isOn());
    assert(mockedWaterHeater->isOff());

    // when
    temperatureControlStation.sleep(2);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(temperatureControlStation.getCurrentState() == TemperatureControlStation::State::SLEEPING);
    assert(mockedSystemFanCooler->isOff());
    assert(mockedAmbientFanCooler->isOff());
    assert(mockedWaterCooler->isOff());
    assert(mockedWaterHeater->isOn()); // release state is specified in the device class

    cout << "pass -> should_GoToSleepStateOnSleep\n";
}

static void should_GoToActiveStateAfterSetSleepTime() {
    // given
    uint8_t sleepMinutes = 2;
    currentMillis = 0;
    mockedSystemFanCooler->mockNotStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOn());
    temperatureControlStation.sleep(sleepMinutes);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // when
    currentMillis += sleepMinutes * 60 * 1000ul;
    temperatureControlStation.update(currentMillis);

    // then
    assert(temperatureControlStation.getCurrentState() == TemperatureControlStation::State::ACTIVE);

    cout << "pass -> should_GoToActiveStateAfterSetSleepTime\n";
}

static void should_GoToActiveStateOnManualWake() {
    // given
    currentMillis = 0;
    mockedSystemFanCooler->mockNotStarted();
    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
    temperatureControlStation.attachSystemCoolingDevice(mockedSystemFanCooler);
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.startSystemCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedSystemFanCooler->isOn());
    temperatureControlStation.sleep(2);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // when
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    currentMillis += 1;
    temperatureControlStation.wake();
    temperatureControlStation.update(currentMillis);

    // then
    assert(temperatureControlStation.getCurrentState() == TemperatureControlStation::State::ACTIVE);

    cout << "pass -> should_GoToActiveStateOnManualWake\n";
}

int main() {
    currentTime = 110;
    temperatureControlStation.attachStorage(mockStoragePointer);

    cout << std::endl
         << "------------------------------------------------------------\n"
         << " >> TEST START\n"
         << "------------------------------------------------------------\n";

    should_NotStartWaterHeatingWhenHeatControlEnabledAndNoHeatingDeviceAttached();
    should_NotStartWaterCoolingWhenCoolControlEnabledAndNoCoolingDeviceAttached();
    should_NotStartWaterHeatingWhenHeatControlDisabled();
    should_NotStartWaterHeatingWhenHeatControlEnabledAndSensorReadingIsBelowZero();
    should_StartWaterHeatingWhenHeatControlEnabledAndTemperatureBelowStopHeatTempC();
    should_NotInterveneWhenHeatingAndHeatControlEnabledAndTemperatureEqualToStopHeatTempC();
    should_NotInterveneWhenNotHeatingAndHeatControlEnabledAndTemperatureEqualToStopHeatTempC();
    should_StopWaterHeatingWhenHeatControlEnabledAndTemperatureAboveStopHeatTempC();
    should_NotStartWaterCoolingWhenCoolControlDisabled();
    should_StartWaterCoolingWhenCoolControlEnabledAndTemperatureAboveStartCoolTempC();
    should_NotInterveneWhenCoolingAndCoolControlEnabledAndTemperatureEqualToStartCoolTempC();
    should_NotInterveneWhenNotCoolingAndCoolControlEnabledAndTemperatureEqualToStartCoolTempC();
    should_StopWaterCoolingWhenCoolControlEnabledAndTemperatureBelowStartCoolTempC();
    should_RaiseAlarmWhenWaterTemperatureAboveMaxTemp();
    should_RaiseAlarmWhenWaterTemperatureBelowMinTemp();

    should_NotStartAmbientCoolingWhenAmbientTemperatureBelowAmbientStartCoolingTempC();
    should_NotStartAmbientCoolingWhenAmbientTemperatureEqualToAmbientStartCoolingTempC();
    should_NotStopAmbientCoolingWhenAmbientTemperatureEqualToAmbientStartCoolingTempC();
    should_StartAmbientCoolingWhenAmbientTemperatureAboveAmbientStartCoolingTempC();
    should_StopAmbientCoolingWhenAmbientTemperatureBelowAmbientStartCoolingTempC();
    should_RaiseAlarmWhenAmbientTemperatureAboveMaxAmbientTempC();

    should_NotStartAmbientVentingWhenAmbientHumidityBelowAmbientStartVentingHumidityPercent();
    should_NotStartAmbientVentingWhenAmbientHumidityEqualToAmbientStartHumidityPercent();
    should_NotStopAmbientVentingWhenAmbientHumidityEqualToAmbientStartVentingHumidityPercent();
    should_StartAmbientVentingWhenAmbientHumidityAboveAmbientStartVentingHumidityPercent();
    should_StopAmbientVentingWhenAmbientHumidityBelowAmbientStartVentingHumidityPercent();
    should_RaiseAlarmWhenAmbientHumidityAboveMaxAmbientHumidityPercent();

    should_NotStopAmbientFanWhenHighAmbientHumidityAboveAndLowAmbientTemperature();
    should_NotStopAmbientFanWhenLowAmbientHumidityAboveAndHighAmbientTemperature();
    should_StopAmbientFanWhenLowAmbientHumidityAboveAndLowAmbientTemperature();

    should_NotStartSystemCoolingWhenSystemTemperatureBelowSystemStartCoolingTempC();
    should_NotStartSystemCoolingWhenSystemTemperatureEqualToSystemStartCoolingTempC();
    should_NotStopSystemCoolingWhenSystemTemperatureEqualToSystemStartCoolingTempC();
    should_StartSystemCoolingWhenSystemTemperatureAboveSystemStartCoolingTempC();
    should_StopSystemCoolingWhenSystemTemperatureBelowSystemStartCoolingTempC();
    should_RaiseAlarmWhenSystemTemperatureAboveMaxSystemTempC();

    should_GoToSleepStateOnSleep();
    should_GoToActiveStateAfterSetSleepTime();
    should_GoToActiveStateOnManualWake();

    cout
            << "------------------------------------------------------------\n"
            << " >> TEST END\n"
            << "------------------------------------------------------------\n"
            << std::endl;

    return 0;
}
