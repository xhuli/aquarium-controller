#ifndef __AQUARIUM_CONTROLLER_ALARM_CODE_H__
#define __AQUARIUM_CONTROLLER_ALARM_CODE_H__

enum AlarmCode {
    SystemMaxTemperatureReached = 1,
    WaterMinTemperatureReached = 2,
    WaterMaxTemperatureReached = 3,
    AmbientMaxTemperatureReached = 4,
    AmbientMaxHumidityReached = 5,
    TopOffFailed = 11,
    ReservoirLow = 12,
    BackupHighSensorOn = 13,
    BackupLowSensorOff = 14,
};

#endif
