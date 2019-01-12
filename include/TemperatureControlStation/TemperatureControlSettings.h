#ifndef __TEMPERATURE_CONTROL_SETTINGS_H__
#define __TEMPERATURE_CONTROL_SETTINGS_H__

typedef struct {
    uint8_t stopHeatingTemperatureCelsius;
    uint8_t startCoolingTemperatureCelsius;

    uint8_t triggerAlarmMinAquariumTemperatureCelsius;
    uint8_t triggerAlarmMaxAquariumTemperatureCelsius;
    uint8_t triggerAlarmMaxSystemTemperatureCelsius;
    uint8_t triggerAlarmMaxSystemHumidityPercent;

    bool isHeatingControlEnabled;
    bool isCoolingControlEnabled;

    bool triggerAlarmMinAquariumTemperatureEnabled;
    bool triggerAlarmMaxAquariumTemperatureEnabled;
    bool triggerAlarmMaxSystemTemperatureEnabled;
    bool triggerAlarmMaxSystemHumidityEnabled;
} TemperatureControlSettings;

#endif