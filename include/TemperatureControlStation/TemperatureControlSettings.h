#ifndef __TEMPERATURE_CONTROL_SETTINGS_H__
#define __TEMPERATURE_CONTROL_SETTINGS_H__

typedef struct {
    uint8_t stopHeatingTemperatureCelsius = 26;
    uint8_t startCoolingTemperatureCelsius = 28;

    uint8_t minAquariumTemperatureCelsiusAlarmTrigger = 22;
    uint8_t maxAquariumTemperatureCelsiusAlarmTrigger = 30;
    uint8_t maxSystemTemperatureCelsiusAlarmTrigger = 48;
    uint8_t maxSystemHumidityPercentAlarmTrigger = 72;

    bool isHeatingControlEnabled = false;
    bool isCoolingControlEnabled = false;

    bool isMinAquariumTemperatureAlarmEnabled = true;
    bool isAlarmMaxAquariumTemperatureAlarmEnabled = true;
    bool isMaxSystemTemperatureEnabled = true;
    bool isMaxSystemHumidityAlarmEnabled = true;
} TemperatureControlSettings;

#endif