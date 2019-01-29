#define __GDZU_DEBUG__
#define __TEST_MODE__

#include <Arduino.h>
#include <OneWire.h>    // https://github.com/PaulStoffregen/OneWire
#include <Time.h>       // standard Arduino time library
#include <Wire.h>       // standard Arduino i2c library
#include <avr/wdt.h>    // Arduino watchdog library
#include <stdint.h>     // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include "DS3232RTC.h"  // https://github.com/JChristensen/DS3232RTC
#include "DosingStation/DosingStation.h"
#include "Sensors/TemperatureAndHumiditySensorDHT.h"
#include "Sensors/TemperatureSensorDS18B20.h"
#include "Storage/Storage.h"
#include "TemperatureControlStation/TemperatureControlStation.h"

#define SERIAL_SPEED = 9600;

/*
    PROBLEMS:
    • implementation tied to specific hardware
    • direct dependency on libraries specific to hardware
*/

const char* dayNames[] = {"All", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

/*
>>  Get Compile Date and Time as a time_t value
*/
time_t getCompileTime() {
    const time_t FUDGE(12);  // fudge factor to allow for upload time, etc. (seconds, YMMV)
    const char *compDate = __DATE__, *compTime = __TIME__, *months = "JanFebMarAprMayJunJulAugSepOctNovDec";
    char compMon[3], *m;

    strncpy(compMon, compDate, 3);
    compMon[3] = '\0';
    m = strstr(months, compMon);

    tmElements_t tm;
    tm.Month = ((m - months) / 3 + 1);
    tm.Day = atoi(compDate + 4);
    tm.Year = atoi(compDate + 7) - 1970;
    tm.Hour = atoi(compTime);
    tm.Minute = atoi(compTime + 3);
    tm.Second = atoi(compTime + 6);

    time_t t = makeTime(tm);
    return t + FUDGE;  // add fudge factor to allow for compile time
}

/* 
>> Specify Arduino PIN usage 
*/
enum ArduinoPin {
    /* edit pin numbers according to your hardware setup */
    HeatingRelay = 1,
    CoolingRelay = 2,
    DHT22Sesnsor = 3,  // ambient temperature and humidity
    OneWireBus = 2,
    Buzzer = 5,
};

/* 
>> Declare and Initialize Hardware 
*/
OneWire oneWire(ArduinoPin::OneWireBus);

TemperatureAndHumiditySensorDHT temperatureAndHumiditySensor = TemperatureAndHumiditySensorDHT(DHT22, ArduinoPin::DHT22Sesnsor);
TemperatureSensorDS18B20 temperatureSensor = TemperatureSensorDS18B20(oneWire, 0);

Storage storage = Storage();

DosingStation dosingStation = DosingStation();
TemperatureControlStation temperatureControlStation = TemperatureControlStation(
    ArduinoPin::HeatingRelay,
    ArduinoPin::CoolingRelay,
    temperatureAndHumiditySensor,
    temperatureSensor);

uint32_t previousMillis = 0;
uint32_t currentMillis = 0;
bool minuteHeartbeat = false;
bool minuteHeartbeatFromRtc = false;
uint32_t minuteHeartbeatStartMillis = 0;
uint32_t minuteHeartbeatDelayMillis = 1000;

void setup() {
    /* Open Serial Port for Debugging */
    // Serial.begin(SERIAL_SPEED);
    // while (!Serial) {
    //     delay(100); // wait until Arduino Serial Monitor opens
    // }

    /* DS3232RTC */
    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    RTC.squareWave(SQWAVE_NONE);

    /*
        Set the RTC time and date to the compile time and date.
        Run once to set the RTC time, and then comment out the line.
    */
    // RTC.set(getCompileTime());

    /* Set the RTC alarm 2 to beat at every minute */
    RTC.setAlarm(ALM2_EVERY_MINUTE, 0, 0, 0, 0);
    RTC.alarm(ALARM_2);  // clear the alarm flag

    /* Connect/sync the Time library with the RTC */
    setSyncProvider(RTC.get);  // default re-sync interval is 5 minutes
    setSyncInterval(330);      // set re-sync interval to 5.5 minutes;

    /* set the time variables */
    currentMillis = millis();

    /* setup sensors */
    temperatureAndHumiditySensor.setup();
    temperatureSensor.setup();

    /* Dosing Station */
    dosingStation.setup();

    /* TemperatureControlStation */
    temperatureControlStation.setup();

    /*
        https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers

        Timer0 is already used for millis() - we'll just interrupt somewhere
        in the middle and call the "Compare A" function below
    */
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);

    /* Watchdog */
    wdt_enable(WDTO_4S);  // set the watchdog time out
}

/* 
>> Function: SIGNAL() run once every millisecond 
*/
SIGNAL(TIMER0_COMPA_vect) {
    currentMillis = millis();
}

void loop() {
    wdt_reset();  // reset watchdog so it won't "bite" - reset the board

    if (previousMillis != currentMillis) {
        previousMillis = currentMillis;
        minuteHeartbeat = false;

        if (RTC.alarm(ALARM_2)) {
            minuteHeartbeatFromRtc = true;
            minuteHeartbeatStartMillis = currentMillis;
        }

        /* 
        add delay to compensate for any difference between RTC minute() and MCU minute() 
        example: 
         - RTC time is 12:15:00:000
         - MCU time is 12:14:59:999
         - RTC ALARM_2 is raised -> minuteHeartbeat == true -> dosing pumps check for tasks at 12:14 and do not find any 
         - RTC ALARM_2 is cleared -> minuteHeartbeat == false;
         - one millisecond passes -> MCU time is 12:15:00:000
         - there are dosing tasks at 12:15, but minuteHeartbeat == false, -> dosing tasks are not executed - NOK
        */
        if (minuteHeartbeatFromRtc && ((currentMillis - minuteHeartbeatStartMillis) > minuteHeartbeatDelayMillis)) {
            minuteHeartbeat = true;
            minuteHeartbeatFromRtc = false;
        }

        dosingStation.update(minuteHeartbeat, currentMillis);
        temperatureControlStation.update(currentMillis);
    }
}
