#ifndef __TEMPERATURE_CONTROL_STATION_H__
#define __TEMPERATURE_CONTROL_STATION_H__

#include "Sensors/TemperatureAndHumiditySensor.h"
#include "Sensors/TemperatureSensorDS18B20.h"
#include "Storage/Storage.h"
#include "TemperatureControlSettings.h"

extern OneWire oneWire;
extern Storage storage;

class TemperatureControlStation {
   private:
    uint8_t heatingRelayPin;
    uint8_t coolingRelayPin;
    TemperatureAndHumiditySensor& cabinetTemperatureAndHumiditySensor;
    TemperatureSensorDS18B20& sumpTemperatureSensor;

    TemperatureControlSettings settings;  // holds defult data

    float systemTemperatureCelsius = -1.0;
    float systemHumidityPercent = -1.0;
    float aquariumTemperatureCelsius = -1.0;

    bool isHeaterOn = false;
    bool isCoolingOn = false;

    uint64_t sleepStartMilis;
    uint64_t sleepPeriodMilis;

    enum class TemperatureControlStationState {
        SLEEPING = false,
        ACTIVE = true,
    } state;

   public:
    TemperatureControlStation(
        uint8_t heatingRelayAttachToPin,
        uint8_t coolingRelayAttachToPin,
        TemperatureAndHumiditySensor& temperatureAndHumiditySensorToAttach,
        TemperatureSensorDS18B20& temperatureSensorToAttach) : heatingRelayPin(heatingRelayAttachToPin),
                                                               coolingRelayPin(coolingRelayAttachToPin),
                                                               cabinetTemperatureAndHumiditySensor(temperatureAndHumiditySensorToAttach),
                                                               sumpTemperatureSensor(temperatureSensorToAttach) {
        //
        DallasTemperature sensors(&oneWire);
    }

    void sleep(unsigned long sleepMinutes) {
        state = TemperatureControlStationState::SLEEPING;
        sleepPeriodMilis = sleepMinutes * 60ul * 1000ul;
        sleepStartMilis = millis();
    }

    void setHeatingControlEnabled(bool isEnabled) {
        settings.isHeatingControlEnabled = isEnabled;
        storage.saveTemperatureControlSettings(settings);
    }

    void setCoolingControlEnabled(bool isEnabled) {
        settings.isCoolingControlEnabled = isEnabled;
        storage.saveTemperatureControlSettings(settings);
    }

    bool setStopHeatingTemperatureCelsius(uint8_t temperatureCelsius) {
        if (temperatureCelsius <= settings.startCoolingTemperatureCelsius) {
            settings.stopHeatingTemperatureCelsius = temperatureCelsius;
            storage.saveTemperatureControlSettings(settings);
            return true;
        } else {
            return false;
        }
    }

    bool setStartCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        if (temperatureCelsius >= settings.stopHeatingTemperatureCelsius) {
            settings.startCoolingTemperatureCelsius = temperatureCelsius;
            storage.saveTemperatureControlSettings(settings);
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
                cabinetTemperatureAndHumiditySensor.update();
                systemTemperatureCelsius = cabinetTemperatureAndHumiditySensor.getTemperatureCelsius();
                systemHumidityPercent = cabinetTemperatureAndHumiditySensor.getHumidityPercent();

                /* read aquarium temparature sensor */
                aquariumTemperatureCelsius = sumpTemperatureSensor.getTemperatureCelsius();

                /* heater control */
                if (settings.isHeatingControlEnabled) {
                    if ((aquariumTemperatureCelsius > settings.stopHeatingTemperatureCelsius) && isHeaterOn) {
                        stopHeating();
                    } else if ((aquariumTemperatureCelsius < settings.stopHeatingTemperatureCelsius) && !isHeaterOn) {
                        startHeating();
                    }
                }

                /* cooling control */
                if (settings.isCoolingControlEnabled) {
                    if ((aquariumTemperatureCelsius > settings.startCoolingTemperatureCelsius) && !isCoolingOn) {
                        startCooling();
                    } else if ((aquariumTemperatureCelsius < settings.startCoolingTemperatureCelsius) && isCoolingOn) {
                        stopCooling();
                    }
                }

                break;

            case TemperatureControlStationState::SLEEPING:

                if ((currentMillis - sleepStartMilis) > sleepPeriodMilis) {
                    state = TemperatureControlStationState::ACTIVE;
                }

                break;
        }
    }
};

#endif
