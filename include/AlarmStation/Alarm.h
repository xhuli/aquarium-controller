#ifndef __AQUARIUM_CONTROLLER_ALARM_H__
#define __AQUARIUM_CONTROLLER_ALARM_H__

#include <cstdint>

typedef struct {

    time_t timeStamp = 0; // 4 bytes
    uint8_t code = 0;
    bool isCritical = false;
    bool isAcknowledged = false;
    uint32_t lastNotificationMillis = 0;
} Alarm;

#endif
