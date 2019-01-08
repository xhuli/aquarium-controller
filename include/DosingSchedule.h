#ifndef __DOSING_SCHEDULE_H__
#define __DOSING_SCHEDULE_H__

#include <LinkedList.h>  // https://github.com/ivanseidel/LinkedList
#include "DosingTask.h"

class DosingSchedule {
   private:
    LinkedList<DosingTask*> dosingSchedule;  // keep a list of dosing task pointers

    bool isValidHourMinute(uint8_t hour, uint8_t minute) {
        if ((hour < 24) && (minute < 60)) {
            return true;
        }
        return false;
    }

   public:
    DosingSchedule() {
        dosingSchedule = LinkedList<DosingTask*>();
    }

    uint8_t size() {
        return dosingSchedule.size();
    }

    DosingTask* getTaskAtIndex(uint8_t index) {
        return dosingSchedule.get(index);
    }

    DosingTask* getTaskAtHourMinute(uint8_t hour, uint8_t minute) {
        // returns the first match, will never return any duplicates if they exist

        DosingTask* dosingTaskPointer;

        for (int i = 0; i < dosingSchedule.size(); i++) {
            if ((dosingTaskPointer->startHour == hour) && (dosingTaskPointer->startMinute == minute)) {
                return dosingTaskPointer;
            }
        }
        return nullptr;
    }

    bool addTask(uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        // add new task at the end of the list

        if (doseMiliLiters < 1) return false;

        // check startHour and startMinute are valid
        // check there is no other dosing task at the same time
        if (isValidHourMinute(startHour, startMinute) && (getTaskAtHourMinute(startHour, startMinute) == nullptr)) {
            DosingTask* dosingTaskPointer = new DosingTask();
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            dosingSchedule.add(dosingTaskPointer);
            return true;
        }
        return false;
    }

    bool updateTask(uint8_t index, uint8_t startHour, uint8_t startMinute, uint8_t doseMiliLiters) {
        // update existing task at index

        if (doseMiliLiters < 1) return false;

        DosingTask* dosingTaskPointer = getTaskAtIndex(index);

        // check the index is valid, not NULL
        // check startHour and startMinute are valid
        // check there is no other dosing task at the same time
        if ((dosingTaskPointer != nullptr) && isValidHourMinute(startHour, startMinute) && (getTaskAtHourMinute(startHour, startMinute) == nullptr)) {
            dosingTaskPointer->startHour = startHour;
            dosingTaskPointer->startMinute = startMinute;
            dosingTaskPointer->doseMiliLiters = doseMiliLiters;
            return true;
        }
        return false;
    }

    void removeTask(uint8_t index) {
        // remove task from schedule

        DosingTask* dosingTaskPointer = dosingSchedule.remove(index);
        delete dosingTaskPointer;
    }
};

#endif