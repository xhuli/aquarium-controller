#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_SEVERITY_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_SEVERITY_H_
#pragma once

enum class AlarmSeverity : uint8_t {
    NoSeverity, // 0
    Minor,      // 1
    Major,      // 2
    Critical,   // 3
};

#endif
