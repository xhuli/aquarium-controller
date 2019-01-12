#ifndef __DOSING_SCHEDULE_H__
#define __DOSING_SCHEDULE_H__

#include <LinkedList.h>  // https://github.com/ivanseidel/LinkedList
#include "DosingTask.h"

#ifndef MAX_NUMBER_OF_TASKS
#define MAX_NUMBER_OF_TASKS 32
#endif

class DosingSchedule {
   private:
    LinkedList<DosingTask*> schedule;  // keep a list of dosing task pointers

    bool isValidDayHourMinute(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        // dayOfWeek: 0 - 7 {"All", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}
        // hour: 0 - 23
        // minute: 0 - 59
        if ((dayOfWeek < 8) && (hour < 24) && (minute < 60)) {
            return true;
        }
        return false;
    }

    bool isOverlappingTasks(uint8_t dayOfWeek, uint8_t hour, uint8_t minute) {
        DosingTask* dosingTaskPointer;

        for (int i = 0; i < schedule.size(); i++) {
            dosingTaskPointer = getTaskAtIndex(i);

            if ((dosingTaskPointer->startHour == hour) && (dosingTaskPointer->startMinute == minute)) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == dayOfWeek) {
                    return true;
                }
            }
        }

        return false;
    }

    static int compareTaskDayOfWeekHourMinute(DosingTask*& taskA, DosingTask*& taskB) {
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
    DosingSchedule() {
        schedule = LinkedList<DosingTask*>();
    }

    uint8_t size() {
        return schedule.size();
    }

    uint8_t getPendingDoseMiliLiters() {
        DosingTask* dosingTaskPointer;

        for (int i = 0; i < schedule.size(); i++) {
            dosingTaskPointer = getTaskAtIndex(i);

            if ((dosingTaskPointer->startHour == hour()) && (dosingTaskPointer->startMinute == minute())) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == weekday()) {
                    return dosingTaskPointer->doseMiliLiters;
                }
            }
        }

        return 0;
    }

    DosingTask* getTaskAtIndex(uint8_t index) {
        return schedule.get(index);
    }

    bool addTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        if (doseMiliLiters < 1) return false;

        if ((schedule.size() < MAX_NUMBER_OF_TASKS) && isValidDayHourMinute(dayOfWeek, startHour, startMinute) && (!isOverlappingTasks(dayOfWeek, startHour, startMinute))) {
            DosingTask* dosingTaskPointer = new DosingTask();
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            schedule.add(dosingTaskPointer);
            schedule.sort(compareTaskDayOfWeekHourMinute);
            return true;
        }

        return false;
    }

    bool updateTask(uint8_t index, uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        if (doseMiliLiters < 1) return false;

        DosingTask* dosingTaskPointer = getTaskAtIndex(index);

        if ((dosingTaskPointer != nullptr) && isValidDayHourMinute(dayOfWeek, startHour, startMinute) && (!isOverlappingTasks(dayOfWeek, startHour, startMinute))) {
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            schedule.sort(compareTaskDayOfWeekHourMinute);
            return true;
        }

        return false;
    }

    void removeTask(uint8_t index) {
        DosingTask* dosingTaskPointer = schedule.remove(index);
        delete dosingTaskPointer;
    }
};

#endif