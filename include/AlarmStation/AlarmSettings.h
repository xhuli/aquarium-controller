#ifndef __AQUARIUM_CONTROLLER_ALARM_SETTINGS_H__
#define __AQUARIUM_CONTROLLER_ALARM_SETTINGS_H__

#include <cstdint>

typedef struct {
    uint8_t criticalAlarmNotifyPeriodMinutes = 15;
    uint8_t majorAlarmNotifyPeriodMinutes = 60;
} AlarmSettings;

#endif
