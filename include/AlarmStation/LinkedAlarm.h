#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_LINKED_ALARM_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_LINKED_ALARM_H_
#pragma once

#include <limits.h>
#include "Enums/AlarmSeverity.h"
#include "Enums/AlarmCode.h"

class LinkedAlarm {

public:

    LinkedAlarm *next; // 2B

private:

    AlarmCode code; // 1B
    AlarmSeverity severity; // 1B

    uint32_t lastNotificationMs = 0; // 4B
    bool acknowledged = false; // 1B

public:

    explicit LinkedAlarm(LinkedAlarm *head, AlarmCode code, AlarmSeverity severity) :
            next(head),
            code(code),
            severity(severity) {
    }

    LinkedAlarm *getNext() const {
        return next;
    }

    AlarmCode getCode() const {
        return code;
    }

    AlarmSeverity getSeverity() const {
        return severity;
    }

    bool isAcknowledged() const {
        return acknowledged;
    }

    void setAcknowledged(bool acknowledged) {
        LinkedAlarm::acknowledged = acknowledged;
    }

    void acknowledge() {
        LinkedAlarm::setAcknowledged(true);
    }

    uint32_t getLastNotificationMs() const {
        return lastNotificationMs;
    }

    void setLastNotificationMs(uint32_t const &lastNotificationMillis) {
        LinkedAlarm::lastNotificationMs = lastNotificationMillis;
    }
};

#endif