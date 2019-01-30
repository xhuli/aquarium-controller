#define __TEST_MODE__

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include "TemperatureControlStation/TemperatureControlStation.h"

#include "MockTemperatureSensor.h"
#include "MockTemperatureAndHumiditySensor.h"
#include "Storage/MockStorage.h"
#include "MockDevice.h"

MockTemperatureSensor *mockSystemTemperatureSensor = new MockTemperatureSensor();
MockTemperatureSensor *mockWaterTemperatureSensor = new MockTemperatureSensor();
MockTemperatureAndHumiditySensor *mockAmbientTemperatureAndHumiditySensor = new MockTemperatureAndHumiditySensor();
MockStorage *mockStoragePointer = new MockStorage();

MockDevice *mockedHeater = new MockDevice();
MockDevice *mockedCooler = new MockDevice();

TemperatureControlSettings settings = TemperatureControlSettings();
TemperatureControlStation temperatureControlStation = TemperatureControlStation(mockStoragePointer);

//void beforeTest() {
//    temperatureControlStation.attachAmbientHumiditySensor(mockAmbientTemperatureAndHumiditySensor);
//    temperatureControlStation.attachAmbientTemperatureSensor(mockAmbientTemperatureAndHumiditySensor);
//    temperatureControlStation.attachSystemTemperatureSensor(mockSystemTemperatureSensor);
//    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
//
//    temperatureControlStation.attachHeatingDevice(mockedHeater);
//    temperatureControlStation.attachCoolingDevice(mockedCooler);
//
//    temperatureControlStation.setup();
//}

static void should_NotStartHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached() {
    // given
//    std::cout << "start -> should_NotStartHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached" << std::endl;
    currentMillis = 0;
    mockedHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
//    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOff());

    // when
    temperatureControlStation.enableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOff());
    std::cout << "pass -> should_NotStartHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached" << std::endl;
}

static void should_NotStartCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached() {
    // given
//    std::cout << "start -> should_NotStartCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached" << std::endl;
    currentMillis = 0;
    mockedCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
//    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOff());

    // when
    temperatureControlStation.enableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOff());
    std::cout << "pass -> should_NotStartCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached" << std::endl;
}

static void should_NotStartHeating_when_HeatControlDisabled() {
    // given
    currentMillis = 0;
    mockedHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOff());

    // when
    temperatureControlStation.disableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOff());
    std::cout << "pass -> should_NotStartHeating_when_HeatControlDisabled" << std::endl;
}

static void should_NotStartHeating_when_HeatControlEnabled_and_SensorReadingIsBelowZero() {
    // given
    currentMillis = 0;
    mockedHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOff());

    // when
    temperatureControlStation.enableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(-1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOff());
    std::cout << "pass -> should_NotStartHeating_when_HeatControlEnabled_and_SensorReadingIsBelowZero" << std::endl;
}

static void should_StartHeating_when_HeatControlEnabled_and_TemperatureBelow_StopHeatTempC() {
    // given
    currentMillis = 0;
    mockedHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOff());

    // when
    temperatureControlStation.enableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOn());
    std::cout << "pass -> should_StartHeating_when_HeatControlEnabled_and_TemperatureBelow_StopHeatTempC" << std::endl;
}

static void should_NotIntervene_when_Heating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC() {
    // given
    currentMillis = 0;
    mockedHeater->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOn());

    // when
    temperatureControlStation.enableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOn());
    std::cout << "pass -> should_NotIntervene_when_Heating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC" << std::endl;
}

static void should_NotIntervene_when_NotHeating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC() {
    // given
    currentMillis = 0;
    mockedHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOff());

    // when
    temperatureControlStation.enableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOff());
    std::cout << "pass -> should_NotIntervene_when_NotHeating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC" << std::endl;
}

static void should_StopHeating_when_HeatControlEnabled_and_TemperatureAbove_StopHeatTempC() {
    // given
    currentMillis = 0;
    mockedHeater->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachHeatingDevice(mockedHeater);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedHeater->isOff());

    // when
    temperatureControlStation.enableHeatingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.stopHeatingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedHeater->isOff());
    std::cout << "pass -> should_StopHeating_when_HeatControlEnabled_and_TemperatureAbove_StopHeatTempC" << std::endl;
}

static void should_NotStartCooling_when_CoolControlDisabled() {
    // given
    currentMillis = 0;
    mockedCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOff());

    // when
    temperatureControlStation.disableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOff());
    std::cout << "pass -> should_NotStartCooling_when_CoolControlDisabled" << std::endl;
}

static void should_StartCooling_when_CoolControlEnabled_and_TemperatureAbove_StartCoolTempC() {
    // given
    currentMillis = 0;
    mockedCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOff());

    // when
    temperatureControlStation.enableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOn());
    std::cout << "pass -> should_StartCooling_when_CoolControlEnabled_and_TemperatureAbove_StartCoolTempC" << std::endl;
}

static void should_NotIntervene_when_Cooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC() {
    // given
    currentMillis = 0;
    mockedCooler->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOn());

    // when
    temperatureControlStation.enableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOn());
    std::cout << "pass -> should_NotIntervene_when_Cooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC" << std::endl;
}

static void should_NotIntervene_when_NotCooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC() {
    // given
    currentMillis = 0;
    mockedCooler->mockNotStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius - 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOff());

    // when
    temperatureControlStation.enableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOff());
    std::cout << "pass -> should_NotIntervene_when_NotCooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC" << std::endl;
}

static void should_StopCooling_when_CoolControlEnabled_and_TemperatureBelow_StartCoolTempC() {
    // given
    currentMillis = 0;
    mockedCooler->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOn());

    // when
    temperatureControlStation.enableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius - 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOff());
    std::cout << "pass -> should_StopCooling_when_CoolControlEnabled_and_TemperatureBelow_StartCoolTempC" << std::endl;
}

static void should_RaiseAlarm_when_TemperatureAbove_MaxTemp() {
    // given
    currentMillis = 0;
    mockedCooler->mockStarted();
    temperatureControlStation.attachWaterTemperatureSensor(mockWaterTemperatureSensor);
    temperatureControlStation.attachCoolingDevice(mockedCooler);
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.startCoolingTemperatureCelsius + 1.0f);
    temperatureControlStation.setup();
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);
    assert(mockedCooler->isOn());

    // when
    temperatureControlStation.enableCoolingControl();
    mockWaterTemperatureSensor->mockTemperatureCelsius(settings.waterMaxTemperatureCelsiusAlarmTrigger + 1.0f);
    currentMillis += 1;
    temperatureControlStation.update(currentMillis);

    // then
    assert(mockedCooler->isOn());
    std::cout << "pass -> should_RaiseAlarm_when_TemperatureAbove_MaxTemp" << std::endl;
}

int main() {
    should_NotStartHeating_when_HeatControlEnabled_and_NoHeatingDeviceAttached();
    should_NotStartCooling_when_CoolControlEnabled_and_NoCoolingDeviceAttached();
    should_NotStartHeating_when_HeatControlDisabled();
    should_NotStartHeating_when_HeatControlEnabled_and_SensorReadingIsBelowZero();
    should_StartHeating_when_HeatControlEnabled_and_TemperatureBelow_StopHeatTempC();
    should_NotIntervene_when_Heating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC();
    should_NotIntervene_when_NotHeating_and_HeatControlEnabled_and_TemperatureEqualTo_StopHeatTempC();
    should_StopHeating_when_HeatControlEnabled_and_TemperatureAbove_StopHeatTempC();
    should_NotStartCooling_when_CoolControlDisabled();
    should_StartCooling_when_CoolControlEnabled_and_TemperatureAbove_StartCoolTempC();
    should_NotIntervene_when_Cooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC();
    should_NotIntervene_when_NotCooling_and_CoolControlEnabled_and_TemperatureEqualTo_StartCoolTempC();
    should_StopCooling_when_CoolControlEnabled_and_TemperatureBelow_StartCoolTempC();
    should_RaiseAlarm_when_TemperatureAbove_MaxTemp();

    return 0;
}
