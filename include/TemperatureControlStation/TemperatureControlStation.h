#ifndef __TEMPERATURE_CONTROL_STATION_H__
#define __TEMPERATURE_CONTROL_STATION_H__

#define REQUIRESALARMS false  // 'DallasTemperature' exclude code for alarm search functions

#include "Adafruit_Sensor.h"    // https://github.com/adafruit/Adafruit_Sensor
#include "DHT.h"                // https://github.com/adafruit/DHT-sensor-library
#include "DHT_U.h"              // https://github.com/adafruit/DHT-sensor-library
#include "DallasTemperature.h"  // https://github.com/milesburton/Arduino-Temperature-Control-Library
#include "TemperatureControlSettings.h"

class TemperatureControlStation {
   private:
    DHT_Unified dht;
    uint8_t dhtPin;
    OneWire oneWire;
    uint8_t temperatureProbePin;
    uint8_t heatingRelayPin;
    uint8_t coolingRelayPin;

    float systemTemperatureCelsius = -1.0;
    float systemHumidityPercent = 0.0;
    float aquariumTemperatureCelsius = -1.0;

    bool isHeaterOn = false;
    bool isCoolingOn = false;

    TemperatureControlSettings settings;

    uint64_t sleepStartMilis;
    uint64_t sleepPeriodMilis;

    sensors_event_t event;  // adafruit sensor event, used for DHT

    DallasTemperature sensors;  // for the DS18B20 temperature probe
    DeviceAddress temparatureProbeAddress;
    uint8_t temperatureProbeResolution = 9;
    uint64_t readOneWireMilis;
    uint32_t readOneWirePeriodMilis = 750 / (1 << (12 - temperatureProbeResolution));

    enum State {
        SLEEPING = false,
        ACTIVE = true,
    } state;

   public:
    TemperatureControlStation(
        uint8_t dhtType,  // see DHT.h define section
        uint8_t dhtAttachToPin,
        OneWire oneWireAttach,
        uint8_t heatingRelayAttachToPin,
        uint8_t coolingRelayAttachToPin) : dht(dhtAttachToPin, dhtType),
                                           oneWire(oneWireAttach),
                                           heatingRelayPin(heatingRelayAttachToPin),
                                           coolingRelayPin(coolingRelayAttachToPin) {
        DallasTemperature sensors(&oneWire);
        settings = TemperatureControlSettings();

        /* default values, overwrite from configuration */

        settings.stopHeatingTemperatureCelsius = 24;
        settings.startCoolingTemperatureCelsius = 28;

        settings.triggerAlarmMinAquariumTemperatureCelsius = 21;
        settings.triggerAlarmMaxAquariumTemperatureCelsius = 30;
        settings.triggerAlarmMaxSystemTemperatureCelsius = 54;
        settings.triggerAlarmMaxSystemHumidityPercent = 72;

        settings.isHeatingControlEnabled = false;
        settings.isCoolingControlEnabled = false;

        settings.triggerAlarmMinAquariumTemperatureEnabled = true;
        settings.triggerAlarmMaxAquariumTemperatureEnabled = true;
        settings.triggerAlarmMaxSystemTemperatureEnabled = true;
        settings.triggerAlarmMaxSystemHumidityEnabled = true;
    }

    void sleep(unsigned long sleepMinutes) {
        state = SLEEPING;
        sleepPeriodMilis = sleepMinutes * 60ul * 1000ul;
        sleepStartMilis = millis();
    }

    void setHeatingControlEnabled(bool enabled) {
        settings.isHeatingControlEnabled = enabled;
        // todo: persist
    }

    void setCoolingControlEnabled(bool enabled) {
        settings.isCoolingControlEnabled = enabled;
        // todo: persist
    }

    bool setStopHeatingTemperatureCelsius(uint8_t stopHeatingTemperatureCelsius) {
        if (temperatureCelsius < settings.startCoolingTemperatureCelsius) {
            settings.stopHeatingTemperatureCelsius = stopHeatingTemperatureCelsius;
            // todo: persist
            return true;
        } else {
            return false;
        }
    }

    bool setStartCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        if (temperatureCelsius > settings.stopHeatingTemperatureCelsius) {
            settings.startCoolingTemperatureCelsius = temperatureCelsius;
            // todo: persist
            return true;
        } else {
            return false;
        }
    }

    float getAquariumTemparatureCelsius() {
        return aquariumTemperatureCelsius;
    }

    float getSystemTemperatureCelsius() {
        return systemTemperatureCelsius;
    }

    float getSystemHumidityPercent() {
        return systemHumidityPercent;
    }

    void startHeating() {
        digitalWrite(heatingRelayPin, HIGH);
        isHeaterOn = true;
    }

    void stopHeating() {
        digitalWrite(heatingRelayPin, LOW);
        isHeaterOn = false;
    }

    void startCooling() {
        digitalWrite(coolingRelayPin, LOW);
        isCoolingOn = true;
    }

    void stopCooling() {
        digitalWrite(coolingRelayPin, HIGH);
        isCoolingOn = false;
    }

    void setup() {
        /* todo: load configuration from flash */

        /* 
            setup the heating relay 
            heater is ON by default, wire it to the normally closed - NC port of the relay
            reverse the logic if local climate requires otherwise
        */
        pinMode(heatingRelayPin, OUTPUT);
        digitalWrite(heatingRelayPin, LOW);
        isHeaterOn = settings.isHeatingControlEnabled ? true : false;

        /* 
            setup the cooling relay 
            cooling is OFF by default, wire it to the normally open - NO port of the relay
            reverse the logic if local climate requires otherwise
        */
        pinMode(coolingRelayPin, OUTPUT);
        digitalWrite(coolingRelayPin, HIGH);
        isCoolingOn = settings.isCoolingControlEnabled ? true : false;

        /* setup for the DHT sensor */
        dht.begin();

        /* setup for the DS18B20 aquarium tempearature probe */
        sensors.begin();
        sensors.getAddress(temparatureProbeAddress, 0);
        sensors.setResolution(temparatureProbeAddress, temperatureProbeResolution);
        sensors.setWaitForConversion(false);
        sensors.requestTemperatures();
        readOneWireMilis = millis();

        state = ACTIVE;
    }

    void update(uint64_t currentMillis) {
        switch (state) {
            case ACTIVE:

                /* read the DHT sensor */
                dht.temperature().getEvent(&event);
                systemTemperatureCelsius = event.temperature;

                dht.humidity().getEvent(&event);
                systemHumidityPercent = event.relative_humidity;

                /* ASYNC read the DS18B20 aquarium tempearature probe */
                if ((currentMillis - readOneWireMilis) > readOneWirePeriodMilis) {
                    aquariumTemperatureCelsius = sensors.getTempCByIndex(0);
                    readOneWireMilis = currentMillis;
                    sensors.requestTemperatures();
                }

                /* heater control */
                if (settings.isHeatingControlEnabled) {
                    if ((aquariumTemperatureCelsius > stopHeatingTemperatureCelsius) && isHeaterOn) {
                        stopHeating();
                    } else if ((aquariumTemperatureCelsius < stopHeatingTemperatureCelsius) && !isHeaterOn) {
                        startHeating();
                    }
                }

                /* cooling control */
                if (settings.isCoolingControlEnabled) {
                    if ((aquariumTemperatureCelsius > startCoolingTemperatureCelsius) && !isCoolingOn) {
                        startCooling();
                    } else if ((aquariumTemperatureCelsius < startCoolingTemperatureCelsius) && isCoolingOn) {
                        stopCooling();
                    }
                }

                break;

            case SLEEPING:

                if ((currentMillis - sleepStartMilis) > sleepPeriodMilis) {
                    state = ACTIVE;
                }

                break;
        }
    }
};

#endif
