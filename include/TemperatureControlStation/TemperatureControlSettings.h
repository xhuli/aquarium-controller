#ifndef __TEMPERATURE_CONTROL_SETTINGS_H__
#define __TEMPERATURE_CONTROL_SETTINGS_H__

typedef struct {
    uint8_t stopHeatingTemperatureCelsius = 24;
    uint8_t startCoolingTemperatureCelsius = 26;

    uint8_t systemMaxTemperatureCelsiusAlarmTrigger = 64;
    uint8_t waterMinTemperatureCelsiusAlarmTrigger = 22;
    uint8_t waterMaxTemperatureCelsiusAlarmTrigger = 28;
    uint8_t ambientMaxTemperatureCelsiusAlarmTrigger = 48;
    uint8_t ambientMaxHumidityPercentAlarmTrigger = 72;

    bool isHeatingControlEnabled = false;
    bool isCoolingControlEnabled = false;

    bool isSystemMaxTemperatureAlarmEnabled = true;
    bool isWaterMinTemperatureAlarmEnabled = true;
    bool isWaterMaxTemperatureAlarmEnabled = true;
    bool isAmbientMaxTemperatureAlarmEnabled = true;
    bool isAmbientMaxHumidityAlarmEnabled = true;
} TemperatureControlSettings;

#endif