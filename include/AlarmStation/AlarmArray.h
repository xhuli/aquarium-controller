#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_ARRAY_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_ARRAY_H_
#pragma once

#include "Enums/AlarmCode.h"
#include "Enums/AlarmSeverity.h"
#include "AlarmArrayElement.h"

template<int N>
class AlarmArray {

private:

    AlarmArrayElement alarmArray[N];

public:

    AlarmArray() {
        for (uint8_t index = 0; index < static_cast<uint8_t>(N); ++index) {
            alarmArray[index] = AlarmArrayElement(static_cast<AlarmCode>(index));
        }
    }

    ~AlarmArray() {
        AlarmArray::removeAll();
    }

    uint8_t size() {
        uint8_t active = 0;
        for (uint8_t index = 0; index < static_cast<uint8_t>(N); ++index) {
            if (alarmArray[index].isActive()) ++active;
        }
        return active;
    }

    bool isEmpty() {
        return size() == 0;
    }

    void add(AlarmCode const &alarmCode, AlarmSeverity const &alarmSeverity) {
        alarmArray[static_cast<uint8_t>(alarmCode)]
                .setActive(true)
                .setSeverity(alarmSeverity);
    }

    void remove(AlarmCode const &alarmCode) {
        alarmArray[static_cast<uint8_t>(alarmCode)]
                .setActive(false);
    }

    void removeAll() {
        for (uint8_t index = 0; index < static_cast<uint8_t>(N); ++index) {
            alarmArray[index].setActive(false);
        }
    }

    void acknowledge(AlarmCode const &alarmCode) {
        if (alarmArray[static_cast<uint8_t>(alarmCode)].isActive()) {
            alarmArray[static_cast<uint8_t>(alarmCode)].acknowledge();
        }
    }

    bool isAcknowledged(AlarmCode const &alarmCode) const {
        return (
                alarmArray[static_cast<uint8_t>(alarmCode)].isActive()
                && alarmArray[static_cast<uint8_t>(alarmCode)].isAcknowledged()
        );
    }

    bool contains(AlarmCode const &alarmCode) {
        return alarmArray[static_cast<uint8_t>(alarmCode)].isActive();
    }

    AlarmArrayElement get(AlarmCode const &alarmCode) {
        return alarmArray[static_cast<uint8_t>(alarmCode)];
    }
};

#endif