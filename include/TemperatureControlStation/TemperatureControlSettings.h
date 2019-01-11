#ifndef __TEMPERATURE_CONTROL_SETTINGS_H__
#define __TEMPERATURE_CONTROL_SETTINGS_H__

typedef struct {
    uint8_t stopHeatingTemperatureCelsius;
    uint8_t startCoolingTemperatureCelsius;

    bool heatingControlEnabled;
    bool coolingControlEnabled;

    uint8_t triggerAlarmMinAuariumTemperatureCelsius;
    uint8_t triggerAlarmMaxAuariumTemperatureCelsius;
    uint8_t triggerAlarmMaxSystemTemperatureCelsius;
    uint8_t triggerAlarmMaxSystemHumidityPercent;

    bool triggerAlarmMinAuariumTemperatureEnabled;
    bool triggerAlarmMaxAuariumTemperatureCelsius;
    bool triggerAlarmMaxSystemTemperatureCelsius;
    bool triggerAlarmMaxSystemHumidityPercent;
} TemperatureControlSettings;

#endif