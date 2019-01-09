#ifndef __DOSING_SCHEDULE_H__
#define __DOSING_SCHEDULE_H__

#include <LinkedList.h>  // https://github.com/ivanseidel/LinkedList
#include "DosingTask.h"

class DosingSchedule {
   private:
    LinkedList<DosingTask*> dosingSchedule;  // keep a list of dosing task pointers

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

        for (int i = 0; i < dosingSchedule.size(); i++) {
            dosingTaskPointer = getTaskAtIndex(i);

            if ((dosingTaskPointer->startHour == hour()) && (dosingTaskPointer->startMinute == minute())) {
                // dayOfWeek == 0 task is valid for every week day
                if (dosingTaskPointer->dayOfWeek == 0 || dosingTaskPointer->dayOfWeek == weekday()) {
                    return true;
                }
            }
        }

        return false;
    }

    int8_t compareTasks(DosingTask* taskA, DosingTask* taksB) {
        if (taskA->dayOfWeek == taksB->dayOfWeek) {
            if (taskA->startHour == taskB->startHour) {
                if (taskA->startMinute == taksB->startMinute) {
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
        } else if (taskA->dayOfWeek < taksB->dayOfWeek) {
            return -1;
        } else {
            return 1;
        }
    }

   public:
    DosingSchedule() {
        dosingSchedule = LinkedList<DosingTask*>();
    }

    uint8_t size() {
        return dosingSchedule.size();
    }

    uint8_t getPendingDoseMiliLiters() {
        DosingTask* dosingTaskPointer;

        for (int i = 0; i < dosingSchedule.size(); i++) {
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
        return dosingSchedule.get(index);
    }

    bool addTask(uint8_t dayOfWeek, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        if (doseMiliLiters < 1) return false;

        if (isValidDayHourMinute(dayOfWeek, startHour, startMinute) && (!isOverlappingTasks(dayOfWeek, startHour, startMinute))) {
            DosingTask* dosingTaskPointer = new DosingTask();
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            dosingSchedule.add(dosingTaskPointer);
            dosingSchedule.sort(compareTasks);
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
            dosingSchedule.sort(compareTasks);
            return true;
        }

        return false;
    }

    void removeTask(uint8_t index) {
        DosingTask* dosingTaskPointer = dosingSchedule.remove(index);
        delete dosingTaskPointer;
    }
};

#endif