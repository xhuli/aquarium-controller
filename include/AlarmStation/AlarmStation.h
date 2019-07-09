#ifndef __AQUARIUM_CONTROLLER_ALARM_STATION_H__
#define __AQUARIUM_CONTROLLER_ALARM_STATION_H__

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

using namespace std;

#endif

#include <LinkedList.h>
#include <Abstract/AbstractConfigurationStorage.h>
#include <Abstract/AbstractAlarmListener.h>
#include "Alarm.h"
#include "AlarmSettings.h"

#ifndef MAX_NUMBER_OF_ALARMS
#define MAX_NUMBER_OF_ALARMS 8
#endif

class AlarmStation {
private:
    LinkedList<Alarm *> alarms = LinkedList<Alarm *>();
    LinkedList<AbstractAlarmListener *> listeners = LinkedList<AbstractAlarmListener *>();

    AlarmSettings settings = AlarmSettings();
    AbstractConfigurationStorage *storagePointer = nullptr;

    uint16_t periodBetweenNotificationsMillis = 5000;
    uint32_t lastAlarmNotificationPushMillis = 0;
    uint32_t criticalAlarmNotificationPeriodMillis = 15ul * 60ul * 1000ul; // <- 15 minutes
    uint32_t majorAlarmNotifyPeriodMillis = 60ul * 60ul * 1000ul; // <- 60 minutes

    bool hasConfigurationStorage = false;

    static int compareAlarmsByTimeStamp(Alarm *&alarmA, Alarm *&alarmB) {
        if (alarmA->timeStamp < alarmB->timeStamp) { return 1; }
        else if (alarmA->timeStamp > alarmB->timeStamp) { return -1; }
        else {
            if (alarmA->code < alarmB->code) { return -1; }
            else if (alarmA->code > alarmB->code) { return 1; }
            else { return 0; }
        }
    }

public:

    bool attachStorage(AbstractConfigurationStorage *configurationStorage) {
        //
        if (configurationStorage != nullptr) {
            //
            storagePointer = configurationStorage;
            hasConfigurationStorage = true;

            return true;
        }
        return false;
    }

    bool attachListener(AbstractAlarmListener *alarmListenerToAttachPointer) {
        //
        AbstractAlarmListener *tempAlarmListenerPointer = nullptr;

        for (uint8_t index = 0; index < listeners.size(); index++) {
            tempAlarmListenerPointer = listeners.get(index);
            if (tempAlarmListenerPointer == alarmListenerToAttachPointer) {
                return false;
            }
        }

        listeners.add(alarmListenerToAttachPointer);
        return true;
    }

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

    bool raiseAlarm(uint8_t alarmCode, bool isCritical) {
        //
        Alarm *alarmPointer = getAlarmByCode(alarmCode);

        if (alarmPointer != nullptr) {
            alarmPointer->timeStamp = now();
            alarms.sort(compareAlarmsByTimeStamp);
            return false;
        }

        if (alarms.size() == MAX_NUMBER_OF_ALARMS) {
            alarms.pop(); // removes the last element
        }

        alarmPointer = new Alarm();

        alarmPointer->code = alarmCode;
        alarmPointer->isAcknowledged = static_cast<uint8_t>(false);
        alarmPointer->isCritical = static_cast<uint8_t>(isCritical);
        alarmPointer->timeStamp = now();

        alarms.add(alarmPointer);

        alarms.sort(compareAlarmsByTimeStamp);
        pushAlarmNotification(millis(), alarmPointer); // immediately notify for new alarm
        alarmPointer->lastNotificationMillis = millis();

        return true;
    }

    bool acknowledgeAlarmByIndex(uint8_t index) {
        //
        Alarm *alarmPointer = alarms.get(index);

        if (alarmPointer != nullptr) {
            alarmPointer->isAcknowledged = static_cast<uint8_t>(true);
            return true;
        }
        return false;
    }

    bool acknowledgeAlarmByCode(uint8_t code) {
        //
        Alarm *alarmPointer = getAlarmByCode(code);

        if (alarmPointer != nullptr) {
            alarmPointer->isAcknowledged = static_cast<uint8_t>(true);
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
        Alarm *alarmPointer = nullptr;

        for (uint8_t index = 0; index < alarms.size(); index++) {
            //
            alarmPointer = alarms.get(index);

            if (alarmPointer != nullptr && alarmPointer->code == code) {
                alarmPointer = alarms.remove(index);
                delete (alarmPointer);
                return true;
            }
        }
        return false;
    }

    void deleteAlarmsInCodeRange(uint8_t startCode, uint8_t endCode) {
        //
        Alarm *alarmPointer = nullptr;

        for (uint8_t index = 0; index < alarms.size(); index++) {
            //
            alarmPointer = alarms.get(index);

            if (alarmPointer != nullptr && (
                    alarmPointer->code >= startCode &&
                    alarmPointer->code <= endCode
            )) {
                //
                alarmPointer = alarms.remove(index);
                delete (alarmPointer);
            }
        }
    }

    void pushAlarmNotification(uint32_t currentMillis, Alarm *alarmPointer) {
        //
        if (alarmPointer != nullptr) {

            alarmPointer->lastNotificationMillis = currentMillis;
            lastAlarmNotificationPushMillis = currentMillis;

            for (uint8_t index = 0; index < listeners.size(); index++) {
                listeners.get(index)->notifyOnAlarm(alarmPointer);
            }
        }
    }

    void setup() {
        //
        if (hasConfigurationStorage && storagePointer->isStoredDataValid()) {
            settings = storagePointer->readAlarmStationSettings(settings);

            criticalAlarmNotificationPeriodMillis = settings.criticalAlarmNotifyPeriodMinutes * 60ul * 1000ul;
            majorAlarmNotifyPeriodMillis = settings.majorAlarmNotifyPeriodMinutes * 60ul * 1000ul;
        }
    }

    void update(uint32_t currentMillis) {
        //
        for (uint8_t index = 0; index < alarms.size(); index++) {

            auto alarmPointer = alarms.get(index);

            if (!alarmPointer->isAcknowledged && ((currentMillis - lastAlarmNotificationPushMillis) > periodBetweenNotificationsMillis)) {
                //
                if (alarmPointer->isCritical && ((currentMillis - alarmPointer->lastNotificationMillis) > criticalAlarmNotificationPeriodMillis)) {
                    //
                    pushAlarmNotification(currentMillis, alarmPointer);
                    return;

                } else if ((currentMillis - alarmPointer->lastNotificationMillis) > majorAlarmNotifyPeriodMillis) {
                    //
                    pushAlarmNotification(currentMillis, alarmPointer);
                    return;
                }
            }
        }
    }
};

#endif
