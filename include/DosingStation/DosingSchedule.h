#ifndef __DOSING_SCHEDULE_H__
#define __DOSING_SCHEDULE_H__

#include <Time.h>       // standard Arduino time library
#include <Wire.h>       // standard Arduino i2c library
#include <avr/wdt.h>    // Arduino watchdog library
#include <stdint.h>     // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include "DS3232RTC.h"  // https://github.com/JChristensen/DS3232RTC

#include "LinkedList.h"  // https://github.com/ivanseidel/LinkedList
#include "DosingTask.h"

#ifndef MAX_NUMBER_OF_TASKS
#define MAX_NUMBER_OF_TASKS 16
#endif

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

#endif

class DosingSchedule {
private:
    LinkedList<DosingTask *> schedule = LinkedList<DosingTask *>();  // keep a list of dosing task pointers

    static bool isValidDayHourMinute(int16_t dayOfWeek, int16_t hour, int16_t minute) {
        //
        // dayOfWeek: 0 - 7 {"All", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}
        // hour: 0 - 23
        // minute: 0 - 59
        //
        return (dayOfWeek < 8)
               && (dayOfWeek >= 0)
               && (hour < 24)
               && (hour >= 0)
               && (minute < 60)
               && (minute >= 0);
    }

    bool isNotOverlappingTasks(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        //
        DosingTask *dosingTaskPointer;

        for (uint8_t index = 0; index < schedule.size(); index++) {
            dosingTaskPointer = schedule.get(index);

            if ((dosingTaskPointer->startHour == hour) && (dosingTaskPointer->startMinute == minute)) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == dayOfWeek) {
                    return false;
                }
            }
        }

        return true;
    }

    bool isValidTime(int16_t dayOfWeek, int16_t hour, int16_t minute) {
        //
        return (isValidDayHourMinute(dayOfWeek, hour, minute) && isNotOverlappingTasks((uint8_t) dayOfWeek, (uint8_t) hour, (uint8_t) minute));
    }

    static bool isValidDose(int16_t doseMilliLiters, int16_t doseMilliLitersFraction) {
        return !(doseMilliLiters == 0 && doseMilliLitersFraction == 0)
               && !(doseMilliLiters < 0 || doseMilliLitersFraction < 0)
               && !(doseMilliLiters > 255 || doseMilliLitersFraction > 3);
    }

    static int compareTaskDayOfWeekHourMinute(DosingTask *&taskA, DosingTask *&taskB) {
        //
        if (taskA->dayOfWeek == taskB->dayOfWeek) {
            if (taskA->startHour == taskB->startHour) {
                if (taskA->startMinute == taskB->startMinute) {
                    return 0;
                } else if (taskA->startMinute < taskB->startMinute) {
                    return -1;
                } else {
                    return 1;
                }
            } else if (taskA->startHour < taskB->startHour) {
                return -1;
            } else {
                return 1;
            }
        } else if (taskA->dayOfWeek < taskB->dayOfWeek) {
            return -1;
        } else {
            return 1;
        }
    }

public:
    uint8_t size() {
        //
        return static_cast<uint8_t>(schedule.size());
    }

    float getPendingDoseMilliLiters() {
        DosingTask *dosingTaskPointer;

        for (uint8_t i = 0; i < schedule.size(); i++) {
            dosingTaskPointer = getTaskAtIndex(i);

            if ((dosingTaskPointer->startHour == hour()) && (dosingTaskPointer->startMinute == minute())) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == weekday()) {
                    return (dosingTaskPointer->doseMilliLiters + 0.25f * dosingTaskPointer->doseMilliLitersFraction);
                }
            }
        }

        return 0;
    }

    DosingTask *getTaskAtIndex(uint8_t index) {
        //
        return schedule.get(index);
    }

    bool addTask(int16_t dayOfWeek, int16_t startHour, int16_t startMinute, int16_t doseMilliLiters, int16_t doseMilliLitersFraction) {
        //
        if ((schedule.size() < MAX_NUMBER_OF_TASKS)
            && isValidTime(dayOfWeek, startHour, startMinute)
            && isValidDose(doseMilliLiters, doseMilliLitersFraction)) {
            //
            auto *dosingTaskPointer = new DosingTask();
            dosingTaskPointer->dayOfWeek = (uint8_t) dayOfWeek;
            dosingTaskPointer->startHour = (uint8_t) startHour;
            dosingTaskPointer->startMinute = (uint8_t) startMinute;
            dosingTaskPointer->doseMilliLitersFraction = (uint8_t) doseMilliLitersFraction;
            dosingTaskPointer->doseMilliLiters = (uint8_t) doseMilliLiters;

            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);

            return true;
        }

        return false;
    }

    bool addTask(DosingTask *dosingTaskPointer) {
        //
        if ((dosingTaskPointer->dayOfWeek < 0) ||
            (dosingTaskPointer->startHour < 0) ||
            (dosingTaskPointer->startMinute < 0) ||
            (dosingTaskPointer->doseMilliLiters < 0) ||
            (dosingTaskPointer->doseMilliLitersFraction < 0)) {
            return false;
        }
        if ((dosingTaskPointer->doseMilliLiters < 1) && (dosingTaskPointer->doseMilliLitersFraction == 0)) { return false; }

        if ((schedule.size() < MAX_NUMBER_OF_TASKS) &&
            isValidTime(dosingTaskPointer->dayOfWeek, dosingTaskPointer->startHour, dosingTaskPointer->startMinute)) {
            //
            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);

            return true;
        }

        return false;
    }

    bool updateTask(uint8_t index, int16_t dayOfWeek, int16_t startHour, int16_t startMinute, int16_t doseMilliLiters, int16_t doseMilliLitersFraction) {
        //
        if ((dayOfWeek < 0) || (startHour < 0) || (startMinute < 0) || (doseMilliLiters < 0) || (doseMilliLitersFraction < 0)) { return false; }

        if ((doseMilliLiters < 1) && (doseMilliLitersFraction == 0)) { return false; }

        DosingTask *dosingTaskPointer = getTaskAtIndex(index);

        if ((dosingTaskPointer != nullptr) && isValidTime(dayOfWeek, startHour, startMinute)) {
            dosingTaskPointer->dayOfWeek = (uint8_t) dayOfWeek;
            dosingTaskPointer->startHour = (uint8_t) startHour;
            dosingTaskPointer->startMinute = (uint8_t) startMinute;
            dosingTaskPointer->doseMilliLiters = (uint8_t) doseMilliLiters;
            dosingTaskPointer->doseMilliLitersFraction = (uint8_t) doseMilliLitersFraction;
            schedule.sort(compareTaskDayOfWeekHourMinute);
            return true;
        }

        return false;
    }

    void removeTask(uint8_t index) {
        //
        DosingTask *dosingTaskPointer = schedule.remove(index);
        delete dosingTaskPointer;
    }
};

#endif