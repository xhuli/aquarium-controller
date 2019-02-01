#ifndef __DOSING_SCHEDULE_H__
#define __DOSING_SCHEDULE_H__

#include "LinkedList.h"  // https://github.com/ivanseidel/LinkedList
#include "DosingTask.h"

#ifndef MAX_NUMBER_OF_TASKS
#define MAX_NUMBER_OF_TASKS 16
#endif

#ifdef __TEST_MODE__
#include <time.h>
time_t theTime = time(NULL);
struct tm* aTime = localtime(&theTime);

uint8_t minute() {
    return (uint8_t) aTime->tm_min;
}

uint8_t hour() {
    return (uint8_t) aTime->tm_hour;
}

uint8_t weekday() {
    return (uint8_t) aTime->tm_wday;
}
#endif

class DosingSchedule {
   private:
    LinkedList<DosingTask*> schedule = LinkedList<DosingTask*>();  // keep a list of dosing task pointers

    bool isValidDayHourMinute(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        //
        // dayOfWeek: 0 - 7 {"All", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}
        // hour: 0 - 23
        // minute: 0 - 59
        //
        return (dayOfWeek < 8) && (hour < 24) && (minute < 60);
    }

    bool isNotOverlappingTasks(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        //
        DosingTask* dosingTaskPointer;

        for (uint8_t i = 0; i < schedule.size(); i++) {
            dosingTaskPointer = getTaskAtIndex(i);

            if ((dosingTaskPointer->startHour == hour) && (dosingTaskPointer->startMinute == minute)) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == dayOfWeek) {
                    return false;
                }
            }
        }

        return true;
    }

    bool isValidData(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        //
        return (isValidDayHourMinute(dayOfWeek, hour, minute) && isNotOverlappingTasks(dayOfWeek, hour, minute));
    }

    static int compareTaskDayOfWeekHourMinute(DosingTask*& taskA, DosingTask*& taskB) {
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
        DosingTask* dosingTaskPointer;

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

    DosingTask* getTaskAtIndex(uint8_t index) {
        //
        return schedule.get(index);
    }

    bool addTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMilliLiters, uint8_t doseMilliLitersFraction) {
        //
        if ((doseMilliLiters < 1) && (doseMilliLitersFraction == 0)) return false;

        if ((schedule.size() < MAX_NUMBER_OF_TASKS) && isValidData(dayOfWeek, startHour, startMinute)) {
            //
            auto * dosingTaskPointer = new DosingTask();
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMilliLitersFraction = doseMilliLitersFraction;
            dosingTaskPointer->doseMilliLiters = doseMilliLiters;

            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);

            return true;
        }

        return false;
    }

    bool addTask(DosingTask* dosingTaskPointer) {
        //
        if ((dosingTaskPointer->doseMilliLiters < 1) && (dosingTaskPointer->doseMilliLitersFraction == 0)) return false;

        if ((schedule.size() < MAX_NUMBER_OF_TASKS) && isValidData(dosingTaskPointer->dayOfWeek, dosingTaskPointer->startHour, dosingTaskPointer->startMinute)) {
            //
            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);
            
            return true;
        }

        return false;
    }

    bool updateTask(uint8_t index, uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMilliLiters, uint8_t doseMilliLitersFraction) {
        //
        if ((doseMilliLiters < 1) && (doseMilliLitersFraction == 0)) return false;

        DosingTask* dosingTaskPointer = getTaskAtIndex(index);

        if ((dosingTaskPointer != nullptr) && isValidData(dayOfWeek, startHour, startMinute)) {
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMilliLitersFraction = doseMilliLitersFraction;
            dosingTaskPointer->doseMilliLiters = doseMilliLiters;
            schedule.sort(compareTaskDayOfWeekHourMinute);
            return true;
        }

        return false;
    }

    void removeTask(uint8_t index) {
        //
        DosingTask* dosingTaskPointer = schedule.remove(index);
        delete dosingTaskPointer;
    }
};

#endif