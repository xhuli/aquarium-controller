#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_LIST_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_LIST_H_
#pragma once

#include <Enums/AlarmCode.h>
#include <Enums/AlarmSeverity.h>
#include <Common/LinkedList.h>
#include "Alarm.h"

class AlarmList : public LinkedList<Alarm> {
public:

    Alarm *getFirst() {
        return &(head->value);
    }

    void add(AlarmCode const &alarmCode, AlarmSeverity const &alarmSeverity) {
        Alarm *alarm = get(alarmCode);
        if (alarm == nullptr) {
            LinkedList<Alarm>::add(Alarm{alarmCode, alarmSeverity});
        } else {
            alarm->setAcknowledged(false);
        }
    }

    void remove(AlarmCode const &alarmCode) {
        Element<Alarm> **tracer = &head;
        while (*tracer) {
            if ((*tracer)->value.getCode() == alarmCode) {
                Element<Alarm> *pAlarmToDelete = *tracer;
                *tracer = (*tracer)->next;
                --LinkedList<Alarm>::count;
                delete pAlarmToDelete;
                break;
            }
            tracer = &(*tracer)->next;
        }
    }

    void setAcknowledge(AlarmCode const &alarmCode, bool acknowledged) {
        Alarm *alarm = get(alarmCode);
        if (alarm != nullptr) {
            alarm->setAcknowledged(acknowledged);
        }
    }

    void acknowledge(AlarmCode const &alarmCode) {
        setAcknowledge(alarmCode, true);
    }

    bool isAcknowledged(AlarmCode const &alarmCode) {
        Alarm *alarm = get(alarmCode);
        if (alarm != nullptr) {
            return alarm->isAcknowledged();
        }
        return false;
    }

    bool contains(AlarmCode const &alarmCode) {
        return get(alarmCode) != nullptr;
    }

    Alarm *get(AlarmCode const &alarmCode) {
        Element<Alarm> **tracer = &head;
        while (*tracer) {
            if ((*tracer)->value.getCode() == alarmCode) {
                return &(*tracer)->value;
            }
            tracer = &(*tracer)->next;
        }
        return nullptr;
    }
};

#endif