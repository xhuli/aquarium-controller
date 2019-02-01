#ifndef __TEMPERATURE_CONTROL_STATION_H__
#define __TEMPERATURE_CONTROL_STATION_H__

#ifdef __TEST_MODE__

#include <iostream>
#include "Abstract/AbstractDevice.h"
#include "MockMillis.h"
#include "MockCommon.h"

#endif

#include "Abstract/AbstractHumiditySensor.h"
#include "Abstract/AbstractTemperatureSensor.h"
#include "Abstract/AbstractConfigurationStorage.h"
#include "TemperatureControlSettings.h"

class AmbientFanDoubleSwitch {
private:
    bool isHumiditySwitchOn = false;
    bool isTemperatureSwitchOn = false;

public:

    void setHumiditySwitchOn() {
        isHumiditySwitchOn = true;
    }

    void setHumiditySwitchOff() {
        isHumiditySwitchOn = false;
    }

    void setTemperatureSwitchOn() {
        isTemperatureSwitchOn = true;
    }

    void setTemperatureSwitchOff() {
        isTemperatureSwitchOn = false;
    }

    bool isOn() {
        return (isHumiditySwitchOn || isTemperatureSwitchOn);
    }
};

class TemperatureControlStation {
private:
    AbstractConfigurationStorage *storagePointer;

    AbstractHumiditySensor *ambientHumiditySensorPointer = nullptr;
    AbstractTemperatureSensor *ambientTemperatureSensorPointer = nullptr;
    AbstractTemperatureSensor *systemTemperatureSensorPointer = nullptr;
    AbstractTemperatureSensor *waterTemperatureSensorPointer = nullptr;

    bool hasAmbientHumiditySensor = false;
    bool hasAmbientTemperatureSensor = false;
    bool hasSystemTemperatureSensor = false;
    bool hasWaterTemperatureSensor = false;

    AbstractDevice *waterHeatingDevicePointer = nullptr;
    AbstractDevice *waterCoolingDevicePointer = nullptr;
    AbstractDevice *systemFanDevicePointer = nullptr;
    AbstractDevice *ambientFanDevicePointer = nullptr;

    bool hasWaterHeatingDevice = false;
    bool hasWaterCoolingDevice = false;
    bool hasSystemFanDevice = false;
    bool hasAmbientFanDevice = false;

    float ambientHumidityPercent = -1.0f;
    float ambientTemperatureCelsius = -1.0f;
    float systemTemperatureCelsius = -1.0f;
    float waterTemperatureCelsius = -1.0f;

    uint32_t sleepStartMillis = 0;
    uint32_t sleepPeriodMillis = 0;

    TemperatureControlSettings settings;  // holds default data
    AmbientFanDoubleSwitch ambientFanDoubleSwitch = AmbientFanDoubleSwitch();

    enum TemperatureControlStationState {
        SLEEPING = 0,
        ACTIVE = 1,
    } state = ACTIVE;

    void setWaterHeatingControl(bool isControlled) {
        settings.isWaterHeatingControlEnabled = isControlled;
        storagePointer->saveTemperatureControlSettings(settings);
    }

    void setWaterCoolingControl(bool isControlled) {
        settings.isWaterCoolingControlEnabled = isControlled;
        storagePointer->saveTemperatureControlSettings(settings);
    }


public:
    explicit TemperatureControlStation(AbstractConfigurationStorage *configurationStoragePointer) :
            storagePointer(configurationStoragePointer) {}

    uint8_t getCurrentState() {
        return state;
    }

    /* attach sensors */
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

    /* attach heaters and coolers */
    void attachWaterHeatingDevice(AbstractDevice *devicePointer) {
        waterHeatingDevicePointer = devicePointer;
        hasWaterHeatingDevice = true;
    }

    void attachWaterCoolingDevice(AbstractDevice *devicePointer) {
        waterCoolingDevicePointer = devicePointer;
        hasWaterCoolingDevice = true;
    }

    void attachSystemCoolingDevice(AbstractDevice *devicePointer) {
        systemFanDevicePointer = devicePointer;
        hasSystemFanDevice = true;
    }

    void attachAmbientCoolingDevice(AbstractDevice *devicePointer) {
        ambientFanDevicePointer = devicePointer;
        hasAmbientFanDevice = true;
    }

    void sleep(uint32_t sleepMinutes) {
        //
        sleepPeriodMillis = sleepMinutes * 60ul * 1000ul;
        sleepStartMillis = millis();

        if (hasAmbientFanDevice) { ambientFanDevicePointer->release(); }
        if (hasSystemFanDevice) { systemFanDevicePointer->release(); }
        if (hasWaterCoolingDevice) { waterCoolingDevicePointer->release(); }
        if (hasWaterHeatingDevice) { waterHeatingDevicePointer->release(); }

        state = TemperatureControlStationState::SLEEPING;
    }

    void wake() {
        state = TemperatureControlStationState::ACTIVE;
    }

    void enableWaterHeatingControl() {
        setWaterHeatingControl(true);
    }

    void disableWaterHeatingControl() {
        setWaterHeatingControl(false);
    }

    void enableWaterCoolingControl() {
        setWaterCoolingControl(true);
    }

    void disableWaterCoolingControl() {
        setWaterCoolingControl(false);
    }

    bool setStopWaterHeatingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        if (temperatureCelsius <= settings.startWaterCoolingTemperatureCelsius) {
            settings.stopWaterHeatingTemperatureCelsius = temperatureCelsius;
            storagePointer->saveTemperatureControlSettings(settings);
            return true;
        }
        return false;
    }

    bool setStartWaterCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        if (temperatureCelsius >= settings.stopWaterHeatingTemperatureCelsius) {
            settings.startWaterCoolingTemperatureCelsius = temperatureCelsius;
            storagePointer->saveTemperatureControlSettings(settings);
            return true;
        }
        return false;
    }

    bool setStartSystemCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        settings.startSystemCoolingTemperatureCelsius = temperatureCelsius;
        storagePointer->saveTemperatureControlSettings(settings);
        return true;
    }

    bool setStartAmbientCoolingTemperatureCelsius(uint8_t temperatureCelsius) {
        //
        settings.startAmbientCoolingTemperatureCelsius = temperatureCelsius;
        storagePointer->saveTemperatureControlSettings(settings);
        return true;
    }

    bool setStartAmbientVentingHumidityPercent(uint8_t humidityPercent) {
        //
        settings.startAmbientVentingHumidityPercent = humidityPercent;
        storagePointer->saveTemperatureControlSettings(settings);
        return true;
    }

    uint8_t getStopWaterHeatingTemperatureCelsius() {
        return settings.stopWaterHeatingTemperatureCelsius;
    }

    uint8_t getStartWaterCoolingTemperatureCelsius() {
        return settings.startWaterCoolingTemperatureCelsius;
    }

    uint8_t getStartSystemCoolingTemperatureCelsius() {
        return settings.startSystemCoolingTemperatureCelsius;
    }

    uint8_t getStartAmbientCoolingTemperatureCelsius() {
        return settings.startAmbientCoolingTemperatureCelsius;
    }

    uint8_t getStartAmbientVentingHumidityPercent() {
        return settings.startAmbientVentingHumidityPercent;
    }

    float getSystemTemperatureCelsius() {
        return systemTemperatureCelsius;
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

    void setup() {
        //
        /* read configuration from storage */
        if (storagePointer->isStoredDataValid()) {
            settings = storagePointer->readTemperatureControlSettings(settings);
        }

        if (hasWaterHeatingDevice) { waterHeatingDevicePointer->setup(); }
        if (hasWaterCoolingDevice) { waterCoolingDevicePointer->setup(); }
        if (hasSystemFanDevice) { systemFanDevicePointer->setup(); };
        if (hasAmbientFanDevice) { ambientFanDevicePointer->setup(); }

        state = TemperatureControlStationState::ACTIVE;
    }

    void update(uint32_t currentMillis) {
        //
        if (hasWaterHeatingDevice) { waterHeatingDevicePointer->update(currentMillis); }
        if (hasWaterCoolingDevice) { waterCoolingDevicePointer->update(currentMillis); }
        if (hasSystemFanDevice) { systemFanDevicePointer->update(currentMillis); };
        if (hasAmbientFanDevice) { ambientFanDevicePointer->update(currentMillis); }

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

                    if (hasSystemFanDevice) {
                        if ((systemTemperatureCelsius > settings.startSystemCoolingTemperatureCelsius) && systemFanDevicePointer->isOff()) {
                            systemFanDevicePointer->start();
                        }

                        if ((systemTemperatureCelsius < settings.startSystemCoolingTemperatureCelsius) && systemFanDevicePointer->isOn()) {
                            systemFanDevicePointer->stop();
                        }
                    }
                }

                /* read ambient temperature and humidity sensor */
                if (hasAmbientHumiditySensor) {
                    ambientHumiditySensorPointer->update(currentMillis);
                    ambientHumidityPercent = ambientHumiditySensorPointer->getHumidityPercent();

                    if (ambientHumidityPercent > settings.ambientMaxHumidityPercentAlarmTrigger) {
                        raiseAlarmAmbientMaxHumidityReached();
                    }

                    if (hasAmbientFanDevice) {
                        if (ambientHumidityPercent > settings.startAmbientVentingHumidityPercent) {
                            ambientFanDoubleSwitch.setHumiditySwitchOn();
                        }

                        if (ambientHumidityPercent < settings.startAmbientVentingHumidityPercent) {
                            ambientFanDoubleSwitch.setHumiditySwitchOff();
                        }
                    }
                }

                if (hasAmbientTemperatureSensor) {
                    ambientTemperatureSensorPointer->update(currentMillis);
                    ambientTemperatureCelsius = ambientTemperatureSensorPointer->getTemperatureCelsius();

                    if (ambientTemperatureCelsius > settings.ambientMaxTemperatureCelsiusAlarmTrigger) {
                        raiseAlarmAmbientMaxTemperatureReached();
                    }

                    if (hasAmbientFanDevice) {
                        if (ambientTemperatureCelsius > settings.startAmbientCoolingTemperatureCelsius) {
                            ambientFanDoubleSwitch.setTemperatureSwitchOn();
                        }

                        if (ambientTemperatureCelsius < settings.startAmbientCoolingTemperatureCelsius) {
                            ambientFanDoubleSwitch.setTemperatureSwitchOff();
                        }
                    }
                }

                if (hasAmbientFanDevice) {
                    if (ambientFanDoubleSwitch.isOn()) {
                        if (ambientFanDevicePointer->isOff()) {
                            ambientFanDevicePointer->start();
                        }
                    } else {
                        if (ambientFanDevicePointer->isOn()) {
                            ambientFanDevicePointer->stop();
                        }
                    }
                }

                /* read water temperature sensor */
                if (hasWaterTemperatureSensor) {
                    waterTemperatureSensorPointer->update(currentMillis);
                    waterTemperatureCelsius = waterTemperatureSensorPointer->getTemperatureCelsius();

                    if (waterTemperatureCelsius < settings.waterMinTemperatureCelsiusAlarmTrigger) {
                        raiseAlarmWaterMinTemperatureReached();
                    }

                    if (waterTemperatureCelsius > settings.waterMaxTemperatureCelsiusAlarmTrigger) {
                        raiseAlarmWaterMaxTemperatureReached();
                    }

                    /* heater control */
                    if (settings.isWaterHeatingControlEnabled && hasWaterHeatingDevice && (waterTemperatureCelsius > 0.0f)) {
                        if ((waterTemperatureCelsius > settings.stopWaterHeatingTemperatureCelsius) && waterHeatingDevicePointer->isOn()) {
                            waterHeatingDevicePointer->stop();
                        } else if ((waterTemperatureCelsius < settings.stopWaterHeatingTemperatureCelsius) && waterHeatingDevicePointer->isOff()) {
                            waterHeatingDevicePointer->start();
                        }
                    }

                    /* cooling control */
                    if (settings.isWaterCoolingControlEnabled && hasWaterCoolingDevice) {
                        if ((waterTemperatureCelsius > settings.startWaterCoolingTemperatureCelsius) && waterCoolingDevicePointer->isOff()) {
                            waterCoolingDevicePointer->start();
                        } else if ((waterTemperatureCelsius < settings.startWaterCoolingTemperatureCelsius) && waterCoolingDevicePointer->isOn()) {
                            waterCoolingDevicePointer->stop();
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
