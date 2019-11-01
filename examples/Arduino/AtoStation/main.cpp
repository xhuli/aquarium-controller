#include <Arduino.h>
#include <avr/wdt.h>

#include <Abstract/AbstractRunnable.h>

#include "../Common/ArduinoSwitchable.h"
#include "../Common/ArduinoSleepPushButton.h"
#include "../Common/ArduinoBuzzer.h"
#include "ArduinoAtoLedController.h"
#include "ArduinoAtoLevelSensor.h"

#include <AtoStation/AtoSettings.h>
#include <AtoStation/AtoStation.h>
#include <AtoStation/HighLevelSensorConnection.h>
#include <AtoStation/NormalLevelSensorConnection.h>
#include <AtoStation/LowLevelSensorConnection.h>
#include <AtoStation/ReservoirLowLevelSensorConnection.h>

/**
 * <br/>
 * Specify hardware connected to arduino pins.<br/>
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

    constexpr uint8_t RedLed = 12;
    constexpr uint8_t YellowLed = 11;
    constexpr uint8_t GreenLed = 10;
    constexpr uint8_t AtoDispenser = 4;
    constexpr uint8_t SleepPushButton = 3;
    constexpr uint8_t Buzzer = 2;

    constexpr uint8_t NormalLiquidLevelSensor = PIN_A0;
    constexpr uint8_t LowLiquidLevelSensor = PIN_A1;
    constexpr uint8_t HighLiquidLevelSensor = PIN_A2;
    constexpr uint8_t ReservoirLowLevelSensor = PIN_A3;

};  // namespace McuPin

/**
 * Mandatory objects.
 */
AtoSettings atoSettings{(10 * 60 * 1000ul), (90 * 1000ul)};
ArduinoSwitchable atoDispenser(McuPin::AtoDispenser);
AtoStation atoStation(atoSettings, atoDispenser);

/**
 * Create appropriate liquid level sensor connections to the ato station.
 * Remove/Comment not implemented hardware.
 */
HighLevelSensorConnection<AtoStation, Level> highLevelSensorConnection(atoStation);
NormalLevelSensorConnection<AtoStation, Level> normalLevelSensorConnection(atoStation);
LowLevelSensorConnection<AtoStation, Level> lowLevelSensorConnection(atoStation);
ReservoirLowLevelSensorConnection<AtoStation, Level> reservoirLowLevelSensorConnection(atoStation);

/**
 * Create appropriate liquid level sensors.
 * Remove/Comment not implemented hardware.
 */
ArduinoAtoLevelSensor atoHighLiquidLevelSensor =
        ArduinoAtoLevelSensor(highLevelSensorConnection, Level::Unknown, McuPin::HighLiquidLevelSensor, HIGH);

ArduinoAtoLevelSensor atoNormalLiquidLevelSensor =
        ArduinoAtoLevelSensor(normalLevelSensorConnection, Level::Unknown, McuPin::NormalLiquidLevelSensor, HIGH);

ArduinoAtoLevelSensor atoLowLiquidLevelSensor =
        ArduinoAtoLevelSensor(lowLevelSensorConnection, Level::Unknown, McuPin::LowLiquidLevelSensor, HIGH);

ArduinoAtoLevelSensor atoReservoirLowLiquidLevelSensor =
        ArduinoAtoLevelSensor(reservoirLowLevelSensorConnection, Level::Unknown, McuPin::ReservoirLowLevelSensor, HIGH);

/**
 * Create signalling led controller.
 * Remove/Comment not implemented hardware.
 */
ArduinoAtoLedController atoLedController(atoStation, McuPin::RedLed, McuPin::YellowLed, McuPin::GreenLed);

/**
 * Create alarm station.
 * Remove/Comment not implemented hardware.
 */
ArduinoBuzzer buzzer(McuPin::Buzzer);
static LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};
AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

/**
 * Sleep button.
 * Remove/Comment not implemented hardware.
 */
constexpr uint8_t atoSleepMinutes = 90;
ArduinoSleepPushButton sleepPushButton{15, 2000, atoSleepMinutes, atoStation, McuPin::SleepPushButton};

void setup() {
    /* Disable watchdog timer first thing, in case it is misconfigured */
    wdt_disable();

    /**
     * Remove/Comment if alarms not desired or not implemented.
     */
    alarmNotifyConfigurations.put(AlarmSeverity::NoSeverity, AlarmNotifyConfiguration(60, 1000));
    alarmNotifyConfigurations.put(AlarmSeverity::Minor, AlarmNotifyConfiguration(15, 3000));
    alarmNotifyConfigurations.put(AlarmSeverity::Major, AlarmNotifyConfiguration(5, 5000));
    alarmNotifyConfigurations.put(AlarmSeverity::Critical, AlarmNotifyConfiguration(1, 7000));

    /**
     * Remove/Comment if alarms not desired or not implemented.
     */
    atoStation.attachAlarmStation(&alarmStation);

    /* Do not edit! */
    AbstractRunnable::setupAll();

    /**
     * Set watchdog time out
     */
    wdt_enable(WDTO_2S);
}

void loop() {
    /* Reset watchdog timer */
    wdt_reset();

    /* Do not edit! */
    AbstractRunnable::loopAll();
}