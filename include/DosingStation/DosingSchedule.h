#ifndef __DOSING_SCHEDULE_H__
#define __DOSING_SCHEDULE_H__

#include <LinkedList.h>  // https://github.com/ivanseidel/LinkedList
#include "DosingTask.h"

#ifndef MAX_NUMBER_OF_TASKS
#define MAX_NUMBER_OF_TASKS 16
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
        if ((dayOfWeek < 8) && (hour < 24) && (minute < 60)) {
            return true;
        }
        return false;
    }

    bool isNotOverlappingTasks(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        //
        DosingTask* dosingTaskPointer;

        for (int i = 0; i < schedule.size(); i++) {
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
        return schedule.size();
    }

    void sort() {
        //
        schedule.sort(compareTaskDayOfWeekHourMinute);
    }

    float getPendingDoseMiliLiters() {
        DosingTask* dosingTaskPointer;

        for (int i = 0; i < schedule.size(); i++) {
            dosingTaskPointer = getTaskAtIndex(i);

            if ((dosingTaskPointer->startHour == hour()) && (dosingTaskPointer->startMinute == minute())) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == weekday()) {
                    return (dosingTaskPointer->doseMiliLiters + 0.25 * dosingTaskPointer->doseMiliLitersFraction);
                }
            }
        }

        return 0;
    }

    DosingTask* getTaskAtIndex(uint8_t index) {
        //
        return schedule.get(index);
    }

    bool addTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters, uint8_t doseMiliLitersFraction) {
        //
        if ((doseMiliLiters < 1) && (doseMiliLitersFraction == 0)) return false;

        if ((schedule.size() < MAX_NUMBER_OF_TASKS) && isValidData(dayOfWeek, startHour, startMinute)) {
            //
            DosingTask* dosingTaskPointer = new DosingTask();
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLitersFraction = doseMiliLitersFraction;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;

            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);

            return true;
        }

        return false;
    }

    bool addTask(DosingTask* dosingTaskPointer) {
        //
        if ((dosingTaskPointer->doseMiliLiters < 1) && (dosingTaskPointer->doseMiliLitersFraction == 0)) return false;

        if ((schedule.size() < MAX_NUMBER_OF_TASKS) && isValidData(dosingTaskPointer->dayOfWeek, dosingTaskPointer->startHour, dosingTaskPointer->startMinute)) {
            //
            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);
            
            return true;
        }

        return false;
    }

    bool updateTask(uint8_t index, uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters, uint8_t doseMiliLitersFraction) {
        //
        if ((doseMiliLiters < 1) && (doseMiliLitersFraction == 0)) return false;

        DosingTask* dosingTaskPointer = getTaskAtIndex(index);

        if ((dosingTaskPointer != nullptr) && isValidData(dayOfWeek, startHour, startMinute)) {
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLitersFraction = doseMiliLitersFraction;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
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