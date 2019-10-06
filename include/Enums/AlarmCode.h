#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_CODE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_CODE_H_
#pragma once

enum class AlarmCode : uint8_t {
    NoAlarm,
    SystemMaxTemperatureReached,
    WaterMinTemperatureReached,
    WaterMaxTemperatureReached,
    AmbientMaxTemperatureReached,
    AmbientMaxHumidityReached,
    AtoTopOffFailed,
    AtoReservoirLow,
    AtoHighLevel,
    AtoLowLevel,
};

#endif
