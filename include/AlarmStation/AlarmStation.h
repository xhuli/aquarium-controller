#ifndef __AQUARIUM_CONTROLLER_ALARM_STATION_H__
#define __AQUARIUM_CONTROLLER_ALARM_STATION_H__

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

#endif

#include <LinkedList.h>
#include "Alarm.h"

#ifndef MAX_NUMBER_OF_ALARMS
#define MAX_NUMBER_OF_ALARMS 8
#endif

class AlarmStation {
private:

    LinkedList<Alarm *> alarms = LinkedList<Alarm *>();  // keep a list of raised alarm pointers

public:

    uint8_t getNumberOfAlarms() {
        return static_cast<uint8_t>(alarms.size());
    }

    Alarm *getAlarmByIndex(uint8_t index) {
        return alarms.get(index);
    }

    Alarm *getAlarmByCode(uint8_t code) {
        //
        Alarm *alarmPointer;

        for (uint8_t index = 0; index < alarms.size(); index++) {
            alarmPointer = alarms.get(index);
            if (alarmPointer->code == code) {
                return alarmPointer;
            }
        }
        return nullptr;
    }

    bool raiseAlarm(uint8_t alarmCode, bool critical) {
        //
        Alarm *alarmPointer;
        bool isCodeInList = false;

        for (uint8_t index = 0; index < alarms.size(); index++) {
            alarmPointer = alarms.get(index);

            if (alarmPointer->code == alarmCode) {
                return false;
            }
        }

        if (alarms.size() == MAX_NUMBER_OF_ALARMS) {
            alarms.pop(); // removes the last element
        }

        alarmPointer = new Alarm();

        alarmPointer->code = alarmCode;
        alarmPointer->acknowledged = static_cast<uint8_t>(false);
        alarmPointer->critical = static_cast<uint8_t>(critical);
        alarmPointer->timeStamp = now();

        alarms.add(alarmPointer);

        return true;
    }

    bool acknowledgeAlarm(uint8_t index) {
        //
        Alarm *alarmPointer = alarms.get(index);

        if (alarmPointer != nullptr) {
            //
            alarmPointer->acknowledged = static_cast<uint8_t>(true);
            return true;
        }

        return false;
    }

    bool deleteAlarmByIndex(uint8_t index) {
        //
        Alarm *alarmPointer = alarms.remove(index);

        if (alarmPointer != nullptr) {
            delete (alarmPointer);
            return true;
        }

        return false;
    }

    bool deleteAlarmByCode(uint8_t code) {
        //
        Alarm *alarmPointer;

        for (uint8_t index = 0; index < alarms.size(); index++) {
            alarmPointer = alarms.get(index);
            if (alarmPointer->code == code) {
                alarmPointer = alarms.remove(index);
                delete (alarmPointer);
                return true;
            }
        }
        return false;
    }
};

#endif
