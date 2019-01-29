#ifndef __TEMPERATURE_CONTROL_STATION_H__
#define __TEMPERATURE_CONTROL_STATION_H__

#include "Sensors/AbstractTemperatureAndHumiditySensor.h"
#include "Sensors/AbstractTemperatureSensor.h"
#include "Storage/Storage.h"
#include "TemperatureControlSettings.h"

extern Storage storage;

class TemperatureControlStation {
   private:
    uint8_t heatingRelayPin;
    uint8_t coolingRelayPin;

    AbstractTemperatureAndHumiditySensor& ambientTemperatureAndHumiditySensor;
    AbstractTemperatureSensor& waterTemperatureSensor;

    TemperatureControlSettings settings;  // holds default data

    float ambientTemperatureCelsius = -1.0;
    float ambientHumidityPercent = -1.0;
    float waterTemperatureCelsius = -1.0;

    bool isHeaterOn = false;
    bool isCoolingOn = false;

    uint32_t sleepStartMillis;
    uint32_t sleepPeriodMillis;

    enum class TemperatureControlStationState {
        SLEEPING = false,
        ACTIVE = true,
    } state;

   public:
    TemperatureControlStation(
        uint8_t heatingRelayAttachToPin,
        uint8_t coolingRelayAttachToPin,
        AbstractTemperatureAndHumiditySensor& temperatureAndHumiditySensorToAttach,
        AbstractTemperatureSensor& temperatureSensorToAttach) : heatingRelayPin(heatingRelayAttachToPin),
                                                                coolingRelayPin(coolingRelayAttachToPin),
                                                                ambientTemperatureAndHumiditySensor(temperatureAndHumiditySensorToAttach),
                                                                waterTemperatureSensor(temperatureSensorToAttach) {
    }

    void sleep(uint32_t sleepMinutes) {
        //
        state = TemperatureControlStationState::SLEEPING;
        sleepPeriodMillis = sleepMinutes * 60ul * 1000ul;
        sleepStartMillis = millis();
    }

    void wake() {
        state = TemperatureControlStationState::ACTIVE;
    }

    void setHeatingControlEnabled(bool isEnabled) {
        //
        settings.isHeatingControlEnabled = isEnabled;
        storage.saveTemperatureControlSettings(settings);
    }

    void setCoolingControlEnabled(bool isEnabled) {
        //
        settings.isCoolingControlEnabled = isEnabled;
        storage.saveTemperatureControlSettings(settings);
    }

    bool setStopHeatingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        if (temperatureCelsius <= settings.startCoolingTemperatureCelsius) {
            settings.stopHeatingTemperatureCelsius = temperatureCelsius;
            storage.saveTemperatureControlSettings(settings);
            return true;
        } else {
            return false;
        }
    }

    bool setStartCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        if (temperatureCelsius >= settings.stopHeatingTemperatureCelsius) {
            settings.startCoolingTemperatureCelsius = temperatureCelsius;
            storage.saveTemperatureControlSettings(settings);
            return true;
        } else {
            return false;
        }
    }

    float getWaterTemperatureCelsius() {
        return waterTemperatureCelsius;
    }

    float getAmbientTemperatureCelsius() {
        return ambientTemperatureCelsius;
    }

    float getAmbientHumidityPercent() {
        return ambientHumidityPercent;
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
        //
        /* read configuration from storage */
        if (storage.isStoredDataValid()) {
            settings = storage.readTemperatureControlSettings(settings);
        }

        /* 
            configure the heating relay pin
            heater is ON by default, wire it to the normally closed - NC port of the relay
            reverse the logic if local climate requires otherwise
        */
        pinMode(heatingRelayPin, OUTPUT);
        digitalWrite(heatingRelayPin, HIGH);
        isHeaterOn = true;

        /* 
            configure the cooling relay pin
            cooling is OFF by default, wire it to the normally open - NO port of the relay
            reverse the logic if local climate requires otherwise
        */
        pinMode(coolingRelayPin, OUTPUT);
        digitalWrite(coolingRelayPin, HIGH);
        isCoolingOn = false;

        state = TemperatureControlStationState::ACTIVE;
    }

    void update(uint32_t currentMillis) {
        //
        switch (state) {
            //
            case TemperatureControlStationState::ACTIVE:

                /* read system temperature and humidity sensor */
                ambientTemperatureAndHumiditySensor.update(currentMillis);
                ambientTemperatureCelsius = ambientTemperatureAndHumiditySensor.getTemperatureCelsius();
                ambientHumidityPercent = ambientTemperatureAndHumiditySensor.getHumidityPercent();

                /* read sump water temperature sensor */
                waterTemperatureSensor.update(currentMillis);
                waterTemperatureCelsius = waterTemperatureSensor.getTemperatureCelsius();

                /* heater control */
                if (settings.isHeatingControlEnabled) {
                    if ((waterTemperatureCelsius > settings.stopHeatingTemperatureCelsius) && isHeaterOn) {
                        stopHeating();
                    } else if ((waterTemperatureCelsius < settings.stopHeatingTemperatureCelsius) && !isHeaterOn) {
                        startHeating();
                    }
                }

                /* cooling control */
                if (settings.isCoolingControlEnabled) {
                    if ((waterTemperatureCelsius > settings.startCoolingTemperatureCelsius) && !isCoolingOn) {
                        startCooling();
                    } else if ((waterTemperatureCelsius < settings.startCoolingTemperatureCelsius) && isCoolingOn) {
                        stopCooling();
                    }
                }

                break;

            case TemperatureControlStationState::SLEEPING:

                if ((currentMillis - sleepStartMillis) > sleepPeriodMillis) {
                    state = TemperatureControlStationState::ACTIVE;
                }

                break;
        }
    }
};

#endif
