#ifndef _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_SETTINGS_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_SETTINGS_H_
#pragma once

/**
 * <br/>
 * Business logic uses no measurement units.<br/>
 * Measurement units (°C or °F) depend on concrete <tt>Sensor</tt> implementation.
 */
struct AmbientSettings {

    float stopWaterHeatingAtTemperature = 24.4;
    float startWaterCoolingAtTemperature = 25.6;
    float startSystemFanAtTemperature = 48;
    float startAmbientFanAtTemperature = 36;
    float startAmbientFanAtHumidity = 64;

    bool isWaterHeatingEnabled = true;
    bool isWaterCoolingEnabled = true;

    float waterMinTemperatureAlarmTrigger = 22;
    float waterMaxTemperatureAlarmTrigger = 28;
    float systemMaxTemperatureAlarmTrigger = 46;
    float ambientMaxTemperatureAlarmTrigger = 32;
    float ambientMaxHumidityAlarmTrigger = 90;


    /* § Section: Constructors */

    AmbientSettings() = default;

    AmbientSettings(
            float stopWaterHeatingAtTemperature,
            float startWaterCoolingAtTemperature,
            float startSystemFanAtTemperature,
            float startAmbientFanAtTemperature,
            float startAmbientFanAtHumidity,
            bool isWaterHeatingControlEnabled,
            bool isWaterCoolingControlEnabled
    ) :
            stopWaterHeatingAtTemperature(stopWaterHeatingAtTemperature),
            startWaterCoolingAtTemperature(startWaterCoolingAtTemperature),
            startSystemFanAtTemperature(startSystemFanAtTemperature),
            startAmbientFanAtTemperature(startAmbientFanAtTemperature),
            startAmbientFanAtHumidity(startAmbientFanAtHumidity),
            isWaterHeatingEnabled(isWaterHeatingControlEnabled),
            isWaterCoolingEnabled(isWaterCoolingControlEnabled) {}


    /* § Section: Getters and Setters */

    float getStopWaterHeatingAtTemperature() const {
        return stopWaterHeatingAtTemperature;
    }

    void setStopWaterHeatingAtTemperature(float stopWaterHeatingAtTemperature) {
        AmbientSettings::stopWaterHeatingAtTemperature = stopWaterHeatingAtTemperature;
    }

    float getStartWaterCoolingAtTemperature() const {
        return startWaterCoolingAtTemperature;
    }

    void setStartWaterCoolingAtTemperature(float startWaterCoolingAtTemperature) {
        AmbientSettings::startWaterCoolingAtTemperature = startWaterCoolingAtTemperature;
    }

    float getStartSystemFanAtTemperature() const {
        return startSystemFanAtTemperature;
    }

    void setStartSystemFanAtTemperature(float startSystemFanAtTemperature) {
        AmbientSettings::startSystemFanAtTemperature = startSystemFanAtTemperature;
    }

    float getStartAmbientFanAtTemperature() const {
        return startAmbientFanAtTemperature;
    }

    void setStartAmbientFanAtTemperature(float startAmbientFanAtTemperature) {
        AmbientSettings::startAmbientFanAtTemperature = startAmbientFanAtTemperature;
    }

    float getStartAmbientFanAtHumidity() const {
        return startAmbientFanAtHumidity;
    }

    void setStartAmbientFanAtHumidity(float startAmbientFanAtHumidity) {
        AmbientSettings::startAmbientFanAtHumidity = startAmbientFanAtHumidity;
    }

    bool isWaterHeatingEnabled1() const {
        return isWaterHeatingEnabled;
    }

    void setIsWaterHeatingEnabled(bool isWaterHeatingEnabled) {
        AmbientSettings::isWaterHeatingEnabled = isWaterHeatingEnabled;
    }

    bool isWaterCoolingEnabled1() const {
        return isWaterCoolingEnabled;
    }

    void setIsWaterCoolingEnabled(bool isWaterCoolingEnabled) {
        AmbientSettings::isWaterCoolingEnabled = isWaterCoolingEnabled;
    }

    float getWaterMinTemperatureAlarmTrigger() const {
        return waterMinTemperatureAlarmTrigger;
    }

    void setWaterMinTemperatureAlarmTrigger(float waterMinTemperatureAlarmTrigger) {
        AmbientSettings::waterMinTemperatureAlarmTrigger = waterMinTemperatureAlarmTrigger;
    }

    float getWaterMaxTemperatureAlarmTrigger() const {
        return waterMaxTemperatureAlarmTrigger;
    }

    void setWaterMaxTemperatureAlarmTrigger(float waterMaxTemperatureAlarmTrigger) {
        AmbientSettings::waterMaxTemperatureAlarmTrigger = waterMaxTemperatureAlarmTrigger;
    }

    float getSystemMaxTemperatureAlarmTrigger() const {
        return systemMaxTemperatureAlarmTrigger;
    }

    void setSystemMaxTemperatureAlarmTrigger(float systemMaxTemperatureAlarmTrigger) {
        AmbientSettings::systemMaxTemperatureAlarmTrigger = systemMaxTemperatureAlarmTrigger;
    }

    float getAmbientMaxTemperatureAlarmTrigger() const {
        return ambientMaxTemperatureAlarmTrigger;
    }

    void setAmbientMaxTemperatureAlarmTrigger(float ambientMaxTemperatureAlarmTrigger) {
        AmbientSettings::ambientMaxTemperatureAlarmTrigger = ambientMaxTemperatureAlarmTrigger;
    }

    float getAmbientMaxHumidityAlarmTrigger() const {
        return ambientMaxHumidityAlarmTrigger;
    }

    void setAmbientMaxHumidityAlarmTrigger(float ambientMaxHumidityAlarmTrigger) {
        AmbientSettings::ambientMaxHumidityAlarmTrigger = ambientMaxHumidityAlarmTrigger;
    }
};

#endif