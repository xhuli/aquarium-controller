#ifndef __AQUARIUM_CONTROLLER_ALARM_CODE_H__
#define __AQUARIUM_CONTROLLER_ALARM_CODE_H__

enum AlarmCode {
    SystemMaxTemperatureReached,
    WaterMinTemperatureReached,
    WaterMaxTemperatureReached,
    AmbientMaxTemperatureReached,
    AmbientMaxHumidityReached,
    TopOffFailed,
    ReservoirLow,
    BackupHighSensorOn,
    BackupLowSensorOff,
};

#endif
