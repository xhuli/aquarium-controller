#ifndef __TEMPERATURE_CONTROL_STATION_H__
#define __TEMPERATURE_CONTROL_STATION_H__

#ifdef __TEST_MODE__

#include <AbstractDevice.h>
#include <iostream>
#include "MockMillis.h"
#include "MockCommon.h"

#endif

#include "Sensors/AbstractHumiditySensor.h"
#include "Sensors/AbstractTemperatureSensor.h"
#include "Storage/AbstractConfigurationStorage.h"
#include "TemperatureControlSettings.h"

class TemperatureControlStation {
#ifdef __TEST_MODE__
public:
#else
    private:
#endif
    AbstractConfigurationStorage *storagePointer;

    AbstractHumiditySensor *ambientHumiditySensorPointer = nullptr;
    AbstractTemperatureSensor *ambientTemperatureSensorPointer = nullptr;
    AbstractTemperatureSensor *systemTemperatureSensorPointer = nullptr;
    AbstractTemperatureSensor *waterTemperatureSensorPointer = nullptr;

    bool hasAmbientHumiditySensor = false;
    bool hasAmbientTemperatureSensor = false;
    bool hasSystemTemperatureSensor = false;
    bool hasWaterTemperatureSensor = false;

    AbstractDevice *heatingDevicePointer = nullptr;
    AbstractDevice *coolingDevicePointer = nullptr;

    bool hasHeatingDevice = false;
    bool hasCoolingDevice = false;

    float ambientHumidityPercent = -1.0f;
    float ambientTemperatureCelsius = -1.0f;
    float systemTemperatureCelsius = -1.0f;
    float waterTemperatureCelsius = -1.0f;

    uint32_t sleepStartMillis = 0;
    uint32_t sleepPeriodMillis = 0;

    TemperatureControlSettings settings;  // holds default data

    enum TemperatureControlStationState {
        SLEEPING = false,
        ACTIVE = true,
    } state = ACTIVE;

public:
    explicit TemperatureControlStation(AbstractConfigurationStorage *configurationStoragePointer) :
            storagePointer(configurationStoragePointer) {}

    void attachAmbientHumiditySensor(AbstractHumiditySensor *sensorPointer) {
        ambientHumiditySensorPointer = sensorPointer;
        hasAmbientHumiditySensor = true;
    }

    void attachAmbientTemperatureSensor(AbstractTemperatureSensor *sensorPointer) {
        ambientTemperatureSensorPointer = sensorPointer;
        hasAmbientTemperatureSensor = true;
    }

    void attachSystemTemperatureSensor(AbstractTemperatureSensor *sensorPointer) {
        systemTemperatureSensorPointer = sensorPointer;
        hasSystemTemperatureSensor = true;
    }

    void attachWaterTemperatureSensor(AbstractTemperatureSensor *sensorPointer) {
        waterTemperatureSensorPointer = sensorPointer;
        hasWaterTemperatureSensor = true;
    }

    void attachCoolingDevice(AbstractDevice *devicePointer) {
        coolingDevicePointer = devicePointer;
        hasCoolingDevice = true;
    }

    void attachHeatingDevice(AbstractDevice *devicePointer) {
        heatingDevicePointer = devicePointer;
        hasHeatingDevice = true;
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

    void enableHeatingControl() {
        //
        settings.isHeatingControlEnabled = true;
        storagePointer->saveTemperatureControlSettings(settings);
    }

    void disableHeatingControl() {
        //
        settings.isHeatingControlEnabled = false;
        storagePointer->saveTemperatureControlSettings(settings);
    }

    void enableCoolingControl() {
        //
        settings.isCoolingControlEnabled = true;
        storagePointer->saveTemperatureControlSettings(settings);
    }

    void disableCoolingControl() {
        //
        settings.isCoolingControlEnabled = false;
        storagePointer->saveTemperatureControlSettings(settings);
    }

    bool setStopHeatingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        if (temperatureCelsius <= settings.startCoolingTemperatureCelsius) {
            settings.stopHeatingTemperatureCelsius = temperatureCelsius;
            storagePointer->saveTemperatureControlSettings(settings);
            return true;
        }
        return false;
    }

    bool setStartCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        if (temperatureCelsius >= settings.stopHeatingTemperatureCelsius) {
            settings.startCoolingTemperatureCelsius = temperatureCelsius;
            storagePointer->saveTemperatureControlSettings(settings);
            return true;
        }
        return false;
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

    void setup() {
        //
        /* read configuration from storage */
        if (storagePointer->isStoredDataValid()) {
            settings = storagePointer->readTemperatureControlSettings(settings);
        }

        state = TemperatureControlStationState::ACTIVE;
    }

    bool raiseAlarmSystemMaxTemperatureReached() {
        if (settings.isSystemMaxTemperatureAlarmEnabled) {
            std::cout << "Alarm! " << "System Max Temperature Reached: " << (int) settings.systemMaxTemperatureCelsiusAlarmTrigger << std::endl;
            return true;
        }
        return false;
    }

    bool raiseAlarmWaterMinTemperatureReached() {
        if (settings.isWaterMinTemperatureAlarmEnabled) {
            std::cout << "Alarm! " << "Water Min Temperature Reached: " << (int) settings.waterMinTemperatureCelsiusAlarmTrigger << std::endl;
            return true;
        }
        return false;
    }

    bool raiseAlarmWaterMaxTemperatureReached() {
        if (settings.isWaterMaxTemperatureAlarmEnabled) {
            std::cout << "Alarm! " << "Water Max Temperature Reached: " << (int) settings.waterMaxTemperatureCelsiusAlarmTrigger << std::endl;
            return true;
        }
        return false;
    }

    bool raiseAlarmAmbientMaxTemperatureReached() {
        if (settings.isAmbientMaxTemperatureAlarmEnabled) {
            std::cout << "Alarm! " << "Ambient Max Temperature Reached: " << (int) settings.ambientMaxTemperatureCelsiusAlarmTrigger << std::endl;
            return true;
        }
        return false;
    }

    bool raiseAlarmAmbientMaxHumidityReached() {
        if (settings.isAmbientMaxHumidityAlarmEnabled) {
            std::cout << "Alarm! " << "Ambient Max Humidity Reached: " << (int) settings.ambientMaxHumidityPercentAlarmTrigger << std::endl;
            return true;
        }
        return false;
    }

    void update(uint32_t currentMillis) {
        //
        switch (state) {
            //
            case TemperatureControlStationState::ACTIVE:

                /* read system temperature sensor */
                if (hasSystemTemperatureSensor) {
                    systemTemperatureSensorPointer->update(currentMillis);
                    systemTemperatureCelsius = systemTemperatureSensorPointer->getTemperatureCelsius();

                    if (systemTemperatureCelsius > settings.systemMaxTemperatureCelsiusAlarmTrigger) {
                        raiseAlarmSystemMaxTemperatureReached();
                    }

                    std::cout << "systemTemperatureCelsius: " << systemTemperatureCelsius << std::endl;
                }

                /* read ambient temperature and humidity sensor */
                if (hasAmbientHumiditySensor) {
                    ambientHumiditySensorPointer->update(currentMillis);
                    ambientHumidityPercent = ambientHumiditySensorPointer->getHumidityPercent();

                    if (ambientHumidityPercent > settings.ambientMaxHumidityPercentAlarmTrigger) {
                        raiseAlarmAmbientMaxHumidityReached();
                    }

                    std::cout << "ambientHumidityPercent: " << ambientHumidityPercent << std::endl;
                }

                if (hasAmbientTemperatureSensor) {
                    ambientTemperatureSensorPointer->update(currentMillis);
                    ambientTemperatureCelsius = ambientTemperatureSensorPointer->getTemperatureCelsius();

                    if (ambientTemperatureCelsius > settings.ambientMaxTemperatureCelsiusAlarmTrigger) {
                        raiseAlarmAmbientMaxTemperatureReached();
                    }

                    std::cout << "ambientTemperatureCelsius: " << ambientTemperatureCelsius << std::endl;
                }

                /* read water temperature sensor */
                if (hasWaterTemperatureSensor) {
                    waterTemperatureSensorPointer->update(currentMillis);
                    waterTemperatureCelsius = waterTemperatureSensorPointer->getTemperatureCelsius();

//                    std::cout << "waterTemperatureCelsius: " << waterTemperatureCelsius << std::endl;

                    (waterTemperatureCelsius < settings.waterMinTemperatureCelsiusAlarmTrigger) && raiseAlarmWaterMinTemperatureReached();
                    (waterTemperatureCelsius > settings.waterMaxTemperatureCelsiusAlarmTrigger) && raiseAlarmWaterMaxTemperatureReached();

//                    if (waterTemperatureCelsius < settings.waterMinTemperatureCelsiusAlarmTrigger) {
//                        raiseAlarmWaterMinTemperatureReached();
//                    }
//
//                    if (waterTemperatureCelsius > settings.waterMaxTemperatureCelsiusAlarmTrigger) {
//                        raiseAlarmWaterMaxTemperatureReached();
//                    }

                    /* heater control */
                    if (settings.isHeatingControlEnabled && hasHeatingDevice && (waterTemperatureCelsius > 0.0f)) {
//                        std::cout << "TempControl assert cooling" << std::endl;
                        if ((waterTemperatureCelsius > settings.stopHeatingTemperatureCelsius) && heatingDevicePointer->isOn()) {
                            heatingDevicePointer->stop();
                        } else if ((waterTemperatureCelsius < settings.stopHeatingTemperatureCelsius) && heatingDevicePointer->isOff()) {
                            heatingDevicePointer->start();
                        }
                    }

                    /* cooling control */
                    if (settings.isCoolingControlEnabled && hasCoolingDevice) {
//                        std::cout << "TempControl assert cooling" << std::endl;
//                        std::cout << "settings.startCoolingTemperatureCelsius: " << (int) settings.startCoolingTemperatureCelsius << std::endl;
//                        std::cout << "coolingDevicePointer->isOff(): " << coolingDevicePointer->isOff() << std::endl;
                        if ((waterTemperatureCelsius > settings.startCoolingTemperatureCelsius) && coolingDevicePointer->isOff()) {
//                            std::cout << "TempControl should start cooling" << std::endl;
                            coolingDevicePointer->start();
                        } else if ((waterTemperatureCelsius < settings.startCoolingTemperatureCelsius) && coolingDevicePointer->isOn()) {
//                            std::cout << "TempControl should stop cooling" << std::endl;
                            coolingDevicePointer->stop();
                        }
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
