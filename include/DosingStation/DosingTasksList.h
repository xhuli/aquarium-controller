#ifndef _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_TASKS_LIST_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_TASKS_LIST_H_
#pragma once

#include <Common/LinkedList.h>
#include <Enums/DayOfWeek.h>
#include "DosingTask.h"

class DosingTasksList : public LinkedList<DosingTask *> {
public:
    DosingTask *get(DayOfWeek dayOfWeek, uint8_t hour, uint8_t minute) {
        Element<DosingTask *> **tracer = &head;
        while (*tracer) {
            DosingTask *pDosingTask = (*tracer)->value;
            if (pDosingTask->isScheduledAt(dayOfWeek, hour, minute)) {
                return pDosingTask;
            }
            tracer = &(*tracer)->next;
        }
        return nullptr;
    }

    bool add(
            DayOfWeek dayOfWeek,
            uint8_t hour,
            uint8_t minute,
            uint8_t doseMilliLiters,
            uint8_t doseMilliLiterQuarters
    ) {
        if (!DosingTask::isValidHour(hour) || !DosingTask::isValidMinute(minute)) { return false; }
        if (!DosingTask::isValidDose(doseMilliLiters, doseMilliLiterQuarters)) { return false; }

        DosingTask *pDosingTask = get(dayOfWeek, hour, minute);

        if (pDosingTask == nullptr) {
            LinkedList<DosingTask *>::add(new DosingTask(dayOfWeek, hour, minute, doseMilliLiters, doseMilliLiterQuarters));
        }

        return false;
    }
};

#endif