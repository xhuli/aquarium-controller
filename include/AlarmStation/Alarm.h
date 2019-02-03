#ifndef AQUARIUMCONTROLLER_ALARM_H
#define AQUARIUMCONTROLLER_ALARM_H

#include <cstdint>

typedef struct {

    uint8_t code: 6, critical: 1, acknowledged: 1; // 1 byte
    time_t timeStamp; // 4 bytes
} Alarm;

#endif
