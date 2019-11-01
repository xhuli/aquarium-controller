#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_H_
#pragma once

#include <stdint.h>
#include "Enums/AlarmSeverity.h"
#include "Enums/AlarmCode.h"

class Alarm {

private:

    AlarmCode code; // 1B
    AlarmSeverity severity; // 1B

    uint32_t lastNotificationMs = 0; // 4B
    bool acknowledged = false; // 1B

public:

    explicit Alarm(
            AlarmCode code,
            AlarmSeverity severity
    ) :
            code(code),
            severity(severity) {}

    AlarmCode getCode() const {
        return Alarm::code;
    }

    AlarmSeverity getSeverity() const {
        return Alarm::severity;
    }

    bool isAcknowledged() const {
        return Alarm::acknowledged;
    }

    void setAcknowledged(bool acknowledged) {
        Alarm::acknowledged = acknowledged;
    }

    void acknowledge() {
        Alarm::setAcknowledged(true);
    }

    uint32_t getLastNotificationMs() const {
        return Alarm::lastNotificationMs;
    }

    void setLastNotificationMs(uint32_t const &lastNotificationMillis) {
        Alarm::lastNotificationMs = lastNotificationMillis;
    }
};

#endif