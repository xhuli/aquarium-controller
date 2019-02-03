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

static void should_NotStartWaterHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached() {
    // given
//    std::cout << "start -> should_NotStartWaterHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached" << std::endl;
    currentMillis = 0;
    mockedWaterHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
//    temperatureControlStation.attachWaterHeatingDevice(mockedWaterHeater);
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
    std::cout << "pass -> should_NotStartWaterHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached" << std::endl;
}

static void should_NotStartWaterCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached() {
    // given
//    std::cout << "start -> should_NotStartWaterCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached" << std::endl;
    currentMillis = 0;
    mockedWaterCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
//    temperatureControlStation.attachWaterCoolingDevice(mockedWaterCooler);
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
    std::cout << "pass -> should_NotStartWaterCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached" << std::endl;
}

static void should_NotStartWaterHeating_when_HeatControlDisabled() {
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
    std::cout << "pass -> should_NotStartWaterHeating_when_HeatControlDisabled" << std::endl;
}

static void should_NotStartWaterHeating_when_HeatControlEnabled_and_SensorReadingIsBelowZero() {
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
        assert(alarmStation.getAlarmByIndex(0)->critical == 1);
        assert(alarmStation.getAlarmByIndex(0)->acknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }
    std::cout << "pass -> should_NotStartWaterHeating_when_HeatControlEnabled_and_SensorReadingIsBelowZero" << std::endl;

    // reset
    temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0);
    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void should_StartWaterHeating_when_HeatControlEnabled_and_TemperatureBelow_StopHeatTempC() {
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
    std::cout << "pass -> should_StartWaterHeating_when_HeatControlEnabled_and_TemperatureBelow_StopHeatTempC" << std::endl;
}

static void should_NotIntervene_when_Heating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC() {
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
    std::cout << "pass -> should_NotIntervene_when_Heating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC" << std::endl;
}

static void should_NotIntervene_when_NotHeating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC() {
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
    std::cout << "pass -> should_NotIntervene_when_NotHeating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC" << std::endl;
}

static void should_StopWaterHeating_when_HeatControlEnabled_and_TemperatureAbove_StopHeatTempC() {
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
    std::cout << "pass -> should_StopWaterHeating_when_HeatControlEnabled_and_TemperatureAbove_StopHeatTempC" << std::endl;
}

static void should_NotStartWaterCooling_when_CoolControlDisabled() {
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
    std::cout << "pass -> should_NotStartWaterCooling_when_CoolControlDisabled" << std::endl;
}

static void should_StartWaterCooling_when_CoolControlEnabled_and_TemperatureAbove_StartCoolTempC() {
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
    std::cout << "pass -> should_StartWaterCooling_when_CoolControlEnabled_and_TemperatureAbove_StartCoolTempC" << std::endl;
}

static void should_NotIntervene_when_Cooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC() {
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
    std::cout << "pass -> should_NotIntervene_when_Cooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC" << std::endl;
}

static void should_NotIntervene_when_NotCooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC() {
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
    std::cout << "pass -> should_NotIntervene_when_NotCooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC" << std::endl;
}

static void should_StopWaterCooling_when_CoolControlEnabled_and_TemperatureBelow_StartCoolTempC() {
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
    std::cout << "pass -> should_StopWaterCooling_when_CoolControlEnabled_and_TemperatureBelow_StartCoolTempC" << std::endl;
}

static void should_RaiseAlarm_when_WaterTemperatureAbove_MaxTemp() {
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
        assert(alarmStation.getAlarmByIndex(0)->critical == 1);
        assert(alarmStation.getAlarmByIndex(0)->acknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }
    std::cout << "pass -> should_RaiseAlarm_when_WaterTemperatureAbove_MaxTemp" << std::endl;

    // reset
    temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0);
    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void should_RaiseAlarm_when_WaterTemperatureBelow_MinTemp() {
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
        assert(alarmStation.getAlarmByIndex(0)->critical == 1);
        assert(alarmStation.getAlarmByIndex(0)->acknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }

    std::cout << "pass -> should_RaiseAlarm_when_WaterTemperatureBelow_MinTemp" << std::endl;

    // reset
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopWaterHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0);
    assert(alarmStation.getNumberOfAlarms() == 0);
}


static void should_NotStartAmbientCooling_when_AmbientTemperatureBelow_AmbientStartCoolingTempC() {
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
    std::cout << "pass -> should_NotStartAmbientCooling_when_AmbientTemperatureBelow_AmbientStartCoolingTempC" << std::endl;
}

static void should_NotStartAmbientCooling_when_AmbientTemperatureEqualTo_AmbientStartCoolingTempC() {
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
    std::cout << "pass -> should_NotStartAmbientCooling_when_AmbientTemperatureEqualTo_AmbientStartCoolingTempC" << std::endl;
}

static void should_NotStopAmbientCooling_when_AmbientTemperatureEqualTo_AmbientStartCoolingTempC() {
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
    std::cout << "pass -> should_NotStopAmbientCooling_when_AmbientTemperatureEqualTo_AmbientStartCoolingTempC" << std::endl;
}

static void should_StartAmbientCooling_when_AmbientTemperatureAbove_AmbientStartCoolingTempC() {
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
    std::cout << "pass -> should_StartAmbientCooling_when_AmbientTemperatureAbove_AmbientStartCoolingTempC" << std::endl;
}

static void should_StopAmbientCooling_when_AmbientTemperatureBelow_AmbientStartCoolingTempC() {
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
    std::cout << "pass -> should_StopAmbientCooling_when_AmbientTemperatureBelow_AmbientStartCoolingTempC" << std::endl;
}

static void should_RaiseAlarm_when_AmbientTemperatureAbove_MaxAmbientTempC() {
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
        assert(alarmStation.getAlarmByIndex(0)->critical == 0);
        assert(alarmStation.getAlarmByIndex(0)->acknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }
    std::cout << "pass -> should_RaiseAlarm_when_AmbientTemperatureAbove_MaxAmbientTempC" << std::endl;

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockTemperatureCelsius(settings.ambientMaxTemperatureCelsiusAlarmTrigger - 1.0f);

    // reset
    temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0);
    assert(alarmStation.getNumberOfAlarms() == 0);
}


static void should_NotStartAmbientVenting_when_AmbientHumidityBelow_AmbientStartVentingHumidityPercent() {
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
    std::cout << "pass -> should_NotStartAmbientVenting_when_AmbientHumidityBelow_AmbientStartVentingHumidityPercent" << std::endl;
}

static void should_NotStartAmbientVenting_when_AmbientHumidityEqualTo_AmbientStartHumidityPercent() {
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
    std::cout << "pass -> should_NotStartAmbientVenting_when_AmbientHumidityEqualTo_AmbientStartHumidityPercent" << std::endl;
}

static void should_NotStopAmbientVenting_when_AmbientHumidityEqualTo_AmbientStartVentingHumidityPercent() {
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
    std::cout << "pass -> should_NotStopAmbientVenting_when_AmbientHumidityEqualTo_AmbientStartVentingHumidityPercent" << std::endl;
}

static void should_StartAmbientVenting_when_AmbientHumidityAbove_AmbientStartVentingHumidityPercent() {
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
    std::cout << "pass -> should_StartAmbientVenting_when_AmbientHumidityAbove_AmbientStartVentingHumidityPercent" << std::endl;
}

static void should_StopAmbientVenting_when_AmbientHumidityBelow_AmbientStartVentingHumidityPercent() {
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
    std::cout << "pass -> should_StopAmbientVenting_when_AmbientHumidityBelow_AmbientStartVentingHumidityPercent" << std::endl;
}

static void should_RaiseAlarm_when_AmbientHumidityAbove_MaxAmbientHumidityPercent() {
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
        assert(alarmStation.getAlarmByIndex(0)->critical == 0);
        assert(alarmStation.getAlarmByIndex(0)->acknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }
    std::cout << "pass -> should_RaiseAlarm_when_AmbientHumidityAbove_MaxAmbientHumidityPercent" << std::endl;

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger - 1.0f);
    temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByIndex(0);
    assert(alarmStation.getNumberOfAlarms() == 0);
}


static void should_NotStopAmbientFan_when_HighAmbientHumidityAbove_and_LowAmbientTemperature() {
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
    std::cout << "pass -> should_NotStopAmbientFan_when_HighAmbientHumidityAbove_and_LowAmbientTemperature" << std::endl;

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger - 1.0f);
}

static void should_NotStopAmbientFan_when_LowAmbientHumidityAbove_and_HighAmbientTemperature() {
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
    std::cout << "pass -> should_NotStopAmbientFan_when_LowAmbientHumidityAbove_and_HighAmbientTemperature" << std::endl;

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.startAmbientVentingHumidityPercent - 1.0f);
}

static void should_StopAmbientFan_when_LowAmbientHumidityAbove_and_LowAmbientTemperature() {
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
    std::cout << "pass -> should_StopAmbientFan_when_LowAmbientHumidityAbove_and_LowAmbientTemperature" << std::endl;

    // reset
    mockAmbientTemperatureAndHumiditySensor->mockHumidityPercent(settings.ambientMaxHumidityPercentAlarmTrigger - 1.0f);
}


static void should_NotStartSystemCooling_when_SystemTemperatureBelow_SystemStartCoolingTempC() {
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
    std::cout << "pass -> should_NotStartSystemCooling_when_SystemTemperatureBelow_SystemStartCoolingTempC" << std::endl;
}

static void should_NotStartSystemCooling_when_SystemTemperatureEqualTo_SystemStartCoolingTempC() {
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
    std::cout << "pass -> should_NotStartSystemCooling_when_SystemTemperatureEqualTo_SystemStartCoolingTempC" << std::endl;
}

static void should_NotStopSystemCooling_when_SystemTemperatureEqualTo_SystemStartCoolingTempC() {
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
    std::cout << "pass -> should_NotStopSystemCooling_when_SystemTemperatureEqualTo_SystemStartCoolingTempC" << std::endl;
}

static void should_StartSystemCooling_when_SystemTemperatureAbove_SystemStartCoolingTempC() {
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
    std::cout << "pass -> should_StartSystemCooling_when_SystemTemperatureAbove_SystemStartCoolingTempC" << std::endl;
}

static void should_StopSystemCooling_when_SystemTemperatureBelow_SystemStartCoolingTempC() {
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
    std::cout << "pass -> should_StopSystemCooling_when_SystemTemperatureBelow_SystemStartCoolingTempC" << std::endl;
}

static void should_RaiseAlarm_when_SystemTemperatureAbove_MaxSystemTempC() {
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
        assert(alarmStation.getAlarmByIndex(0)->critical == 0);
        assert(alarmStation.getAlarmByIndex(0)->acknowledged == 0);
        assert(alarmStation.getAlarmByIndex(0)->timeStamp == currentTime);
    }
    std::cout << "pass -> should_RaiseAlarm_when_SystemTemperatureAbove_MaxSystemTempC" << std::endl;

    // reset
    mockSystemTemperatureSensor->mockTemperatureCelsius(settings.systemMaxTemperatureCelsiusAlarmTrigger - 1.0f);
    temperatureControlStation.hasAlarmStation && alarmStation.deleteAlarmByCode(1);
    assert(alarmStation.getNumberOfAlarms() == 0);
}

static void should_GoToSleepState_on_Sleep() {
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
    std::cout << "pass -> should_GoToSleepState_on_Sleep" << std::endl;
}

static void should_GoToActiveState_after_SetSleepTime() {
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
    std::cout << "pass -> should_GoToActiveState_after_SetSleepTime" << std::endl;
}

static void should_GoToActiveState_on_ManualWake() {
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
    std::cout << "pass -> should_GoToActiveState_on_ManualWake" << std::endl;
}

int main() {
    currentTime = 110;
//    temperatureControlStation.attachAlarmStation(&alarmStation);
    temperatureControlStation.attachStorage(mockStoragePointer);

    std::cout << std::endl
              << "------------------------------------------------------------" << std::endl
              << " >> TEST START" << std::endl
              << "------------------------------------------------------------" << std::endl;

    should_NotStartWaterHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached();
    should_NotStartWaterCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached();
    should_NotStartWaterHeating_when_HeatControlDisabled();
    should_NotStartWaterHeating_when_HeatControlEnabled_and_SensorReadingIsBelowZero();
    should_StartWaterHeating_when_HeatControlEnabled_and_TemperatureBelow_StopHeatTempC();
    should_NotIntervene_when_Heating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC();
    should_NotIntervene_when_NotHeating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC();
    should_StopWaterHeating_when_HeatControlEnabled_and_TemperatureAbove_StopHeatTempC();
    should_NotStartWaterCooling_when_CoolControlDisabled();
    should_StartWaterCooling_when_CoolControlEnabled_and_TemperatureAbove_StartCoolTempC();
    should_NotIntervene_when_Cooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC();
    should_NotIntervene_when_NotCooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC();
    should_StopWaterCooling_when_CoolControlEnabled_and_TemperatureBelow_StartCoolTempC();
    should_RaiseAlarm_when_WaterTemperatureAbove_MaxTemp();
    should_RaiseAlarm_when_WaterTemperatureBelow_MinTemp();

    should_NotStartAmbientCooling_when_AmbientTemperatureBelow_AmbientStartCoolingTempC();
    should_NotStartAmbientCooling_when_AmbientTemperatureEqualTo_AmbientStartCoolingTempC();
    should_NotStopAmbientCooling_when_AmbientTemperatureEqualTo_AmbientStartCoolingTempC();
    should_StartAmbientCooling_when_AmbientTemperatureAbove_AmbientStartCoolingTempC();
    should_StopAmbientCooling_when_AmbientTemperatureBelow_AmbientStartCoolingTempC();
    should_RaiseAlarm_when_AmbientTemperatureAbove_MaxAmbientTempC();

    should_NotStartAmbientVenting_when_AmbientHumidityBelow_AmbientStartVentingHumidityPercent();
    should_NotStartAmbientVenting_when_AmbientHumidityEqualTo_AmbientStartHumidityPercent();
    should_NotStopAmbientVenting_when_AmbientHumidityEqualTo_AmbientStartVentingHumidityPercent();
    should_StartAmbientVenting_when_AmbientHumidityAbove_AmbientStartVentingHumidityPercent();
    should_StopAmbientVenting_when_AmbientHumidityBelow_AmbientStartVentingHumidityPercent();
    should_RaiseAlarm_when_AmbientHumidityAbove_MaxAmbientHumidityPercent();

    should_NotStopAmbientFan_when_HighAmbientHumidityAbove_and_LowAmbientTemperature();
    should_NotStopAmbientFan_when_LowAmbientHumidityAbove_and_HighAmbientTemperature();
    should_StopAmbientFan_when_LowAmbientHumidityAbove_and_LowAmbientTemperature();

    should_NotStartSystemCooling_when_SystemTemperatureBelow_SystemStartCoolingTempC();
    should_NotStartSystemCooling_when_SystemTemperatureEqualTo_SystemStartCoolingTempC();
    should_NotStopSystemCooling_when_SystemTemperatureEqualTo_SystemStartCoolingTempC();
    should_StartSystemCooling_when_SystemTemperatureAbove_SystemStartCoolingTempC();
    should_StopSystemCooling_when_SystemTemperatureBelow_SystemStartCoolingTempC();
    should_RaiseAlarm_when_SystemTemperatureAbove_MaxSystemTempC();

    should_GoToSleepState_on_Sleep();
    should_GoToActiveState_after_SetSleepTime();
    should_GoToActiveState_on_ManualWake();

    std::cout
            << "------------------------------------------------------------" << std::endl
            << " >> TEST END" << std::endl
            << "------------------------------------------------------------" << std::endl
            << std::endl;

    return 0;
}
