#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_ARRAY_ELEMENT_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_ARRAY_ELEMENT_H_
#pragma once

#include <stdint.h>
#include "Enums/AlarmSeverity.h"
#include "Enums/AlarmCode.h"

class AlarmArrayElement {

private:

    AlarmCode code; // 1B
    AlarmSeverity severity; // 1B

    uint32_t lastNotificationMs = 0; // 4B
    bool acknowledged = false; // 1B
    bool active = false; // 1B

public:

    AlarmArrayElement() :
            code(AlarmCode::NoAlarm),
            severity(AlarmSeverity::NoSeverity) {}

    explicit AlarmArrayElement(AlarmCode code) :
            code(code),
            severity(AlarmSeverity::NoSeverity) {}

    AlarmArrayElement(AlarmCode code, AlarmSeverity severity) :
            code(code),
            severity(severity) {}

    AlarmCode getCode() const {
        return code;
    }

    AlarmSeverity getSeverity() const {
        return severity;
    }

    AlarmArrayElement setSeverity(AlarmSeverity const &severity) {
        AlarmArrayElement::severity = severity;
        return *this;
    }

    bool isActive() const {
        return active;
    }

    AlarmArrayElement setActive(bool active) {
        AlarmArrayElement::active = active;
        return *this;
    }

    bool isAcknowledged() const {
        return acknowledged;
    }

    AlarmArrayElement acknowledge() {
        AlarmArrayElement::acknowledged = true;
        return *this;
    }

    uint32_t getLastNotificationMs() const {
        return lastNotificationMs;
    }

    AlarmArrayElement setLastNotificationMs(uint32_t const &lastNotificationMillis) {
        AlarmArrayElement::lastNotificationMs = lastNotificationMillis;
        return *this;
    }
};

#endif