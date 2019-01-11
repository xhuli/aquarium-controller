
#ifndef __TEMPERATURE_CONTROL_STATION_H__
#define __TEMPERATURE_CONTROL_STATION_H__

#include "Adafruit_Sensor.h"    // https://github.com/adafruit/Adafruit_Sensor
#include "DHT_U.h"              // https://github.com/adafruit/DHT-sensor-library
#include "DallasTemperature.h"  // https://github.com/milesburton/Arduino-Temperature-Control-Library
#include "OneWire.h"            // https://github.com/PaulStoffregen/OneWire

#ifndef DHT_TYPE
#define DHT_TYPE DHT22
#endif

class TemperatureControlStation {
   private:
    /* default values, overwrite from configuration */

    TemperatureControlSettings settings;

    settings.stopHeatingTemperatureCelsius = 23;
    settings.startCoolingTemperatureCelsius = 28;

    settings.triggerAlarmMinAquariumTemperatureCelsius = 21;
    settings.triggerAlarmMaxAquariumTemperatureCelsius = 30;
    settings.triggerAlarmMaxSystemTemperatureCelsius = 54;
    settings.triggerAlarmMaxSystemHumidityPercent = 72;

    settings.heatingControlEnabled = true;
    settings.coolingControlEnabled = true;

    settings.triggerAlarmMinAquariumTemperatureEnabled = true;
    settings.triggerAlarmMaxAquariumTemperatureCelsius = true;
    settings.triggerAlarmMaxSystemTemperatureCelsius = true;
    settings.triggerAlarmMaxSystemHumidityPercent = true;

    enum State {
        SLEEPING = false,
        ACTIVE = true,
    } state;

    uint32_t sleepStartMilis;
    uint32_t sleepPeriodMilis;

    float systemTemperatureCelsius;
    float systemHumidityPercent;

    uint8_t dhtPin;
    uint8_t heatingRelayPin;
    uint8_t coolingRelayPin;
    uint8_t temperatureProbePin;

    bool isValidHeatingTemperature(uint8_t stopHeatingTemperatureCelsius) {
        if (stopHeatingTemperatureCelsius < settings.startCoolingTemperatureCelsius) {
            return true;
        } else {
            return false;
        }
    }

    bool isValidCoolingTemperature(uint8_t startCoolingTemperatureCelsius) {
        if (startCoolingTemperatureCelsius > settings.stopHeatingTemperatureCelsius) {
            return true;
        } else {
            return false;
        }
    }

   public:
    TemperatureControlStation(dhtAttachToPin, heatingRelayAttachToPin, coolingRealyAttachToPin, temperatureProbeAttachToPin) : dhtPin(dhtAttachToPin),
                                                                                                                               heatingRelayPin(heatingRelayAttachToPin),
                                                                                                                               coolingRelayPin(coolingRelayAttachToPin),
                                                                                                                               temperatureProbePin(temperatureProbeAttachToPin) {
        DHT_Unified dht(dhtPin, DHT_TYPE);
    }

    void sleep(unsigned long sleepMinutes) {
        state = SLEEPING;
        sleepPeriodMilis = sleepMinutes * 60ul * 1000ul;
        sleepStartMilis = millis();
    }

    float convertCelsiusToFahrenheit(float temperatureCelsius) {
        return (temperatureCelsius * 1.8) + 32;
    }

    void enableAlarms();
    void disableAlarms();
    void acknowledgeAlarm();

    void startHeating() {
        digitalWrite(heatingRelayPin, HIGH);
    }

    void stopHeating() {
        digitalWrite(heatingRelayPin, LOW);
    }

    void startCooling() {
        digitalWrite(coolingRelayPin, LOW);
    }

    void stopCooling() {
        digitalWrite(coolingRelayPin, HIGH);
    }

    void setup() {
        /* todo: load configuration from flash */

        /* 
            configure heating relay 
            heater is on by default, it has to be wired to the normally closed - NC port of the relay
        */
        pinMode(heatingRelayPin, OUTPUT);
        digitalWrite(heatingRelayPin, LOW);

        /* 
            configure cooling relay 
            cooling is off by default, it has to be wired to the normally open - NO port of the relay
        */
        pinMode(coolingRelayPin, OUTPUT);
        digitalWrite(coolingRelayPin, HIGH);

        dht.begin();

        state = ACTIVE;
    }

    void update() {
        switch (state) {
            case ACTIVE:

                dht.temperature().getEvent(&event);
                systemTemperatureCelsius = event.temperature;

                dht.humidity().getEvent(&event);
                systemHumidityPercent = event.relative_humidity;

                break;

            case SLEEPING:

                if ((unsigned long)(millis() - sleepStartMilis) > sleepPeriodMilis) {
                    state = ACTIVE;
                }

                break;
        }
    }
};

#endif
