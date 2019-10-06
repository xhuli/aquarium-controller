//#define __ATO_MODE_PRODUCTION__
//#define __ATO_MODE_TESTING_PLATFORMIO__
#define __ATO_MODE_EDITING_CLION__

#ifdef __ATO_MODE_PRODUCTION__
#define __PRODUCTION__
#include <Arduino.h>
#include <Streaming.h>
#include <avr/wdt.h>
#endif

#ifdef __ATO_MODE_TESTING_PLATFORMIO__
#define __SERIAL_DEBUG__

#include <Arduino.h>
#include <Streaming.h>
#include <avr/wdt.h>

#endif

#ifdef __ATO_MODE_EDITING_CLION__

#include <stdint.h>
#include "../lib/Arduino.h"

#endif

#include <Abstract/AbstractSwitchable.h>
#include <AmbientStation/AmbientStation.h>
#include <Common/CountDown.h>
#include <Common/LinkedHashMap.h>
#include <Common/RunnableFunction.h>
#include <Common/Sensor.h>

#include "../Common/ArduinoBuzzer.h"
#include "../Common/ArduinoSwitchable.h"

/**
 * <br/>
 * Specify hardware connected to arduino port connections.<br/>
 * Remove/Comment not implemented hardware.<br/>
 *
 * <a href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/">Arduino Reference :: digitalRead()</a><br/>
 * The analog input pins can be used as digital pins, referred to as A0, A1, etc.<br/>
 * The exception is the Arduino Nano, Pro Mini, and Mini’s A6 and A7 pins, which can only be used as analog inputs.<br/>
 *
 * <a href="https://www.arduino.cc/en/Tutorial/AnalogInputPins">Analog Input Pins</a><br/>
 * The ATmega datasheet also cautions against switching analog pins in close temporal proximity to making A/D readings (analogRead) on other analog pins.
 */
namespace McuPin {

    constexpr uint8_t AmbientFan = 12;  // <- to relay input
    constexpr uint8_t SystemFan = 11;   // <- to relay input
    constexpr uint8_t WaterCooler = 10; // <- to relay input
    constexpr uint8_t WaterHeater = 9;  // <- to relay input
    constexpr uint8_t Buzzer = 2;

    constexpr uint8_t AnalogSensor = PIN_A0; /* todo: remove */

};

/**
 * Create alarm station.
 * Remove/Comment not implemented hardware.
 */
ArduinoBuzzer buzzer{McuPin::Buzzer};
static LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};
AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

/**
 * Create appropriate switchable devices.
 * Remove/Comment not implemented hardware.
 */
ArduinoSwitchable ambientFan{McuPin::AmbientFan};
ArduinoSwitchable systemFan{McuPin::SystemFan};
ArduinoSwitchable waterCooler{McuPin::WaterCooler};
ArduinoSwitchable waterHeater{McuPin::WaterHeater};

/**
 * Create appropriate temperature and humidity sensors.
 * Remove/Comment not implemented hardware.
 */
Sensor<float> ambientHumiditySensor{-100.0f};
Sensor<float> ambientTemperatureSensor{-100.0f};
Sensor<float> systemTemperatureSensor{-100.0f};
Sensor<float> waterTemperatureSensor{-100.0f};

/**
 * Create ambient station.
 */
static AmbientSettings ambientSettings{
        24.2,
        25.4,
        42,
        32,
        66,
        true,
        true
};

/**
 * <br/>
 * <tt>RunnableGlobalFunction</tt> which will be executed by <tt>AbstractRunnable::loopAll();</tt>
 */

constexpr uint32_t delayMs = 30 * 1000ul;
CountDown ambientFanCountDown{};
RunnableFunction ambientFanRules{[]() -> void {
    /* what to do depending on sensors readings */
    if (ambientFanCountDown.isNotCounting()) {
        if (ambientHumiditySensor.getReading() >= ambientSettings.startAmbientFanAtHumidity ||
            ambientTemperatureSensor.getReading() >= ambientSettings.startAmbientFanAtTemperature) {
            //
            if (ambientFan.isInState(Switched::Off)) {
                ambientFan.setState(Switched::On);
                ambientFanCountDown.start(delayMs);
            }
        } else {
            if (ambientFan.isInState(Switched::On)) {
                ambientFan.setState(Switched::Off);
                ambientFanCountDown.start(delayMs);
            }
        }
    }

    /* what to do with alarms */
    if (ambientHumiditySensor.getReading() > ambientSettings.ambientMaxHumidityAlarmTrigger) {
        alarmStation.alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Minor);
    } else {
        alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxHumidityReached);
    }

    if (ambientTemperatureSensor.getReading() > ambientSettings.ambientMaxTemperatureAlarmTrigger) {
        alarmStation.alarmList.add(AlarmCode::AmbientMaxTemperatureReached, AlarmSeverity::Minor);
    } else {
        alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxTemperatureReached);
    }
}};

CountDown systemFanCountDown{};
RunnableFunction systemFanRules{[]() -> void {
    /* what to do depending on sensors readings */
    if (systemFanCountDown.isNotCounting()) {
        if (systemTemperatureSensor.getReading() >= ambientSettings.startSystemFanAtTemperature) {
            if (systemFan.isInState(Switched::Off)) {
                systemFan.setState(Switched::On);
                systemFanCountDown.start(delayMs);
            }
        } else {
            if (systemFan.isInState(Switched::On)) {
                systemFan.setState(Switched::Off);
                systemFanCountDown.start(delayMs);
            }
        }
    }

    /* what to do with alarms */
    if (systemTemperatureSensor.getReading() > ambientSettings.systemMaxTemperatureAlarmTrigger) {
        alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Minor);
    } else {
        alarmStation.alarmList.acknowledge(AlarmCode::SystemMaxTemperatureReached);
    }
}};

CountDown waterCoolerCountDown{};
RunnableFunction waterCoolingRules{[]() -> void {
    /* what to do if disabled */
    if (!ambientSettings.isWaterCoolingEnabled) {
        waterCooler.setState(Switched::Off);
        alarmStation.alarmList.acknowledge(AlarmCode::WaterMaxTemperatureReached);
        return;
    }

    /* what to do depending on sensors readings */
    if (waterCoolerCountDown.isNotCounting()) {
        if (waterTemperatureSensor.getReading() >= ambientSettings.startWaterCoolingAtTemperature) {
            if (waterCooler.isInState(Switched::Off)) {
                waterCooler.setState(Switched::On);
                waterCoolerCountDown.start(delayMs);
            }
        } else {
            if (waterCooler.isInState(Switched::On)) {
                waterCooler.setState(Switched::Off);
                waterCoolerCountDown.start(delayMs);
            }
        }
    }

    /* what to do with alarms */
    if (waterTemperatureSensor.getReading() > ambientSettings.waterMaxTemperatureAlarmTrigger) {
        alarmStation.alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Major);
    } else {
        alarmStation.alarmList.acknowledge(AlarmCode::WaterMaxTemperatureReached);
    }
}};

CountDown waterHeaterCountDown{};
RunnableFunction waterHeatingRules{[]() -> void {
    /* what to do if disabled */
    if (!ambientSettings.isWaterHeatingEnabled) {
        waterHeater.setState(Switched::Off);
        alarmStation.alarmList.acknowledge(AlarmCode::WaterMinTemperatureReached);
        return;
    }

    /* what to do depending on sensors readings */
    if (waterHeaterCountDown.isNotCounting()) {
        if (waterTemperatureSensor.getReading() < ambientSettings.stopWaterHeatingAtTemperature) {
            if (waterHeater.isInState(Switched::Off)) {
                waterHeater.setState(Switched::On);
                waterHeaterCountDown.start(delayMs);
            }
        } else {
            if (waterHeater.isInState(Switched::On)) {
                waterHeater.setState(Switched::Off);
                waterHeaterCountDown.start(delayMs);
            }
        }
    }

    /* what to do with alarms */
    if (waterTemperatureSensor.getReading() < ambientSettings.waterMinTemperatureAlarmTrigger &&
        waterTemperatureSensor.getReading() > -99.0f) {
        //
        alarmStation.alarmList.add(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Major);
    } else {
        alarmStation.alarmList.acknowledge(AlarmCode::WaterMinTemperatureReached);
    }
}};

void setup() {
    /* Disable watchdog timer first thing, in case it is misconfigured */
#ifdef __PRODUCTION__
    wdt_disable();
#endif

#ifdef __SERIAL_DEBUG__
    /* Set serial console and logging functionality */
    Serial.begin(9600);
    while (!Serial && !Serial.available()) {
    }
    delay(500);

    Serial << "\n\nmain::setup()\n";
#endif

    /**
     * Remove/Comment if alarms not desired or not implemented.
     */
#ifdef __PRODUCTION__
    alarmNotifyConfigurations.put(AlarmSeverity::NoSeverity, AlarmNotifyConfiguration(60, 1000));
    alarmNotifyConfigurations.put(AlarmSeverity::Minor, AlarmNotifyConfiguration(15, 3000));
    alarmNotifyConfigurations.put(AlarmSeverity::Major, AlarmNotifyConfiguration(5, 5000));
    alarmNotifyConfigurations.put(AlarmSeverity::Critical, AlarmNotifyConfiguration(1, 7000));
#else
    alarmNotifyConfigurations.put(AlarmSeverity::NoSeverity, AlarmNotifyConfiguration(4, 1000));
    alarmNotifyConfigurations.put(AlarmSeverity::Minor, AlarmNotifyConfiguration(3, 2000));
    alarmNotifyConfigurations.put(AlarmSeverity::Major, AlarmNotifyConfiguration(2, 3000));
    alarmNotifyConfigurations.put(AlarmSeverity::Critical, AlarmNotifyConfiguration(1, 4000));
#endif

    /* Do not edit! */
    AbstractRunnable::setupAll();

    /**
     * Set watchdog time out
     */
#ifdef __PRODUCTION__
    wdt_enable(WDTO_2S);
#endif
}

void loop() {
    /* Reset watchdog timer */
#ifdef __PRODUCTION__
    wdt_reset();
#endif

    /* Do not edit! */
    AbstractRunnable::loopAll();
}