#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_LIST_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_LIST_H_
#pragma once

#include "Enums/AlarmCode.h"
#include "Enums/AlarmSeverity.h"
#include "LinkedAlarm.h"

class AlarmList {

private:

    LinkedAlarm *head = nullptr;
    uint8_t count = 0;

public:

    ~AlarmList() {
        removeAll();
    }

    uint8_t size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    LinkedAlarm *getFirst() const {
        return head;
    }

    void add(AlarmCode const &alarmCode, AlarmSeverity const &alarmSeverity) {
        LinkedAlarm *alarm = get(alarmCode);
        if (alarm == nullptr) {
            head = new LinkedAlarm(head, alarmCode, alarmSeverity);
            ++count;
        } else {
            alarm->setAcknowledged(false);
        }
    }

    void remove(AlarmCode const &alarmCode) {
        LinkedAlarm **tracer = &head;
        while (*tracer) {
            if ((*tracer)->getCode() == alarmCode) {
                LinkedAlarm *pAlarmToDelete = *tracer;
                *tracer = (*tracer)->next;
                count--;
                delete pAlarmToDelete;
                break;
            }
            tracer = &(*tracer)->next;
        }
    }

    void removeAll() {
        LinkedAlarm **tracer = &head;
        while (*tracer) {
            LinkedAlarm *pAlarmToDelete = *tracer;
            *tracer = (*tracer)->next;
            count--;
            delete pAlarmToDelete;
        }
    }

    void setAcknowledge(AlarmCode const &alarmCode, bool acknowledged) {
        LinkedAlarm *alarm2 = get(alarmCode);
        if (alarm2 != nullptr) {
            alarm2->setAcknowledged(acknowledged);
        }
    }

    void acknowledge(AlarmCode const &alarmCode) {
        setAcknowledge(alarmCode, true);
    }

    bool isAcknowledged(AlarmCode const &alarmCode) {
        LinkedAlarm *alarm2 = get(alarmCode);
        if (alarm2 != nullptr) {
            return alarm2->isAcknowledged();
        }
        return false;
    }

    bool contains(AlarmCode const &alarmCode) {
        return get(alarmCode) != nullptr;
    }

    LinkedAlarm *get(AlarmCode const &alarmCode) {
        LinkedAlarm **tracer = &head;
        while (*tracer) {
            if ((*tracer)->getCode() == alarmCode) {
                return (*tracer);
            }
            tracer = &(*tracer)->next;
        }
        return nullptr;
    }
};

#endif