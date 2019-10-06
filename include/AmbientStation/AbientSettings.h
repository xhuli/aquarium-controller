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
};

#endif