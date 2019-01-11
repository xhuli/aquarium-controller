#include <Arduino.h>
#include <Time.h>       // standard Arduino time library
#include <Wire.h>       // standard Arduino i2c library
#include <avr/wdt.h>    // Arduino watchdog library
#include <stdint.h>     // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include "DS3232RTC.h"  // https://github.com/JChristensen/DS3232RTC
#include "DosingStation/DosingStation.h"

#define SERIAL_SPEED = 9600;

const char* dayNames[] = {"All", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

/*!
    @brief function to return the compile date and time as a time_t value
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

DosingStation dosingStation = DosingStation();
TemperatureControlStation temperatureControlStation = TemperatureControlStation();
bool minuteHeartbeat = false;

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
    // setSyncInterval(interval);  // set the number of seconds between re-sync

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

SIGNAL(TIMER0_COMPA_vect) {
    wdt_reset();  // reset watchdog so it won't "bite" - reset the board

    unsigned long currentMillis = millis();

    minuteHeartbeat = false;

    if (RTC.alarm(ALARM_2)) {
        minuteHeartbeat = true;
    }

    dosingStation.update(minuteHeartbeat, currentMillis);
    temperatureControlStation.update();
}

void loop() {
}
