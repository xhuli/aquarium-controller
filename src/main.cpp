 #define __MODE_PRODUCTION__
//#define __MODE_TESTING_PLATFORMIO__
// #define __MODE_EDITING_CLION__

#ifdef __MODE_PRODUCTION__
#define __PRODUCTION__
#include <Arduino.h>
#include <avr/wdt.h>
#endif

#ifdef __MODE_TESTING_PLATFORMIO__
#define __SERIAL_DEBUG__

#include <Arduino.h>
#include <Streaming.h>
#include <avr/wdt.h>

#endif

#ifdef __MODE_EDITING_CLION__

#include <stdint.h>
#include "../lib/Arduino.h"

#endif

#include "../examples/Arduino/Common/ArduinoSwitchable.h"
#include "../examples/Arduino/AtoStation/ArduinoAtoLedController.h"
#include "../examples/Arduino/AtoStation/ArduinoAtoLevelSensor.h"
#include "../examples/Arduino/Common/ArduinoSleepPushButton.h"
#include "../examples/Arduino/Common/ArduinoBuzzer.h"

#include <Abstract/AbstractRunnable.h>

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
#ifdef __PRODUCTION__
AtoSettings atoSettings{(10 * 60 * 1000ul), (90 * 1000ul)};
#else
AtoSettings atoSettings{(1 * 60 * 1000ul), (10 * 1000ul)};
#endif

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
 constexpr bool notInvertedInput = true;
ArduinoAtoLevelSensor atoHighLiquidLevelSensor =
        ArduinoAtoLevelSensor(highLevelSensorConnection, Level::Unknown, McuPin::HighLiquidLevelSensor, notInvertedInput);

ArduinoAtoLevelSensor atoNormalLiquidLevelSensor =
        ArduinoAtoLevelSensor(normalLevelSensorConnection, Level::Unknown, McuPin::NormalLiquidLevelSensor, notInvertedInput);

ArduinoAtoLevelSensor atoLowLiquidLevelSensor =
        ArduinoAtoLevelSensor(lowLevelSensorConnection, Level::Unknown, McuPin::LowLiquidLevelSensor, notInvertedInput);

ArduinoAtoLevelSensor atoReservoirLowLiquidLevelSensor =
        ArduinoAtoLevelSensor(reservoirLowLevelSensorConnection, Level::Unknown, McuPin::ReservoirLowLevelSensor, notInvertedInput);

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
static LinkedMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};
AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

/**
 * Sleep button.
 * Remove/Comment not implemented hardware.
 */
#ifdef __PRODUCTION__
constexpr uint8_t atoSleepMinutes = 90;
#else
constexpr uint8_t atoSleepMinutes = 1;
#endif
ArduinoSleepPushButton sleepPushButton{15, 2000, atoSleepMinutes, atoStation, McuPin::SleepPushButton};

void setup() {
#ifdef __PRODUCTION__
    /* Disable watchdog timer first thing, in case it is misconfigured */
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

    /**
     * Remove/Comment if alarms not desired or not implemented.
     */
    atoStation.attachAlarmStation(&alarmStation);

    /* Do not edit! */
    AbstractRunnable::setupAll();

#ifdef __PRODUCTION__
    /**
     * Set watchdog time out
     */
    wdt_enable(WDTO_2S);
#endif
}

void loop() {
#ifdef __PRODUCTION__
    /* Reset watchdog timer */
    wdt_reset();
#endif

    /* Do not edit! */
    AbstractRunnable::loopAll();
}
