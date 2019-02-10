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
    uint32_t lastCriticalAlarmNotificationMillis = 0;
    uint32_t lastMajorAlarmNotificationMillis = 0;
    uint32_t criticalAlarmNotificationPeriodMillis = 15ul * 60ul * 1000ul;
    uint32_t majorAlarmNotifyPeriodMillis = 60ul * 60ul * 1000ul;

    uint16_t alarmIndexToPush = 0;

    bool isNotifyingCriticalAlarms = true;
    bool hasPushedCriticalAlarmNotification = false;
    bool hasPushedMajorAlarmNotification = false;

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

    bool hasConfigurationStorage = false;

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
        pushAlarmNotification(alarmPointer); // immediately notify for new alarm

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

    void pushAlarmNotification(Alarm *alarmPointer) {
        //
        if (alarmPointer != nullptr) {
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
        cout << "\n\n";
        cout << "update" << "\n";
        cout << "alarms.size(): " << alarms.size() << "\n";
        cout << "currentMillis: " << currentMillis << "\n";
        cout << "lastAlarmNotificationPushMillis: " << lastAlarmNotificationPushMillis << "\n";
        cout << "periodBetweenNotificationsMillis: " << periodBetweenNotificationsMillis << "\n";

        if ((alarms.size() > 0) && ((currentMillis - lastAlarmNotificationPushMillis) > periodBetweenNotificationsMillis)) {
            //
            cout << "alarm size > 0 and periodBetweenNotifications" << "\n";
            cout << "alarmIndexToPush: " << alarmIndexToPush << "\n";

            auto alarmPointer = alarms.get(alarmIndexToPush);

            if (alarmPointer != nullptr && !alarmPointer->isAcknowledged) {

                cout << "alarm not ack" << "\n";

                if (isNotifyingCriticalAlarms) {

                    cout << "notifying critical alarms" << "\n";

                    if (alarmPointer->isCritical
                        && !(alarmPointer->hasSentNotification)
                        && ((currentMillis - lastCriticalAlarmNotificationMillis) >= criticalAlarmNotificationPeriodMillis)) {

                        cout << "push critical notification" << "\n";
                        cout << "lastCriticalAlarmNotificationMillis: " << lastCriticalAlarmNotificationMillis << "\n";

                        pushAlarmNotification(alarmPointer);
                        alarmIndexToPush += 1;
                        hasPushedCriticalAlarmNotification = true;
                        alarmPointer->hasSentNotification = true;

                        lastAlarmNotificationPushMillis = currentMillis;
                    }

                    if (alarmIndexToPush >= (alarms.size() - 1)) {

                        cout << "crit.notif. reset index" << "\n";

                        isNotifyingCriticalAlarms = false;

                        alarmIndexToPush = 0;

                        if (hasPushedCriticalAlarmNotification) {
                            lastCriticalAlarmNotificationMillis = currentMillis;
                        }

                        hasPushedCriticalAlarmNotification = false;

                        for (uint8_t index = 0; index < alarms.size(); index++) {
                            alarms.get(0)->hasSentNotification = false;
                        }
                    }

                } else {

                    cout << "notifying major alarms" << "\n";

                    if (!alarmPointer->isCritical
                        && !(alarmPointer->hasSentNotification)
                        && ((currentMillis - lastMajorAlarmNotificationMillis) >= majorAlarmNotifyPeriodMillis)) {

                        cout << "push major notification" << "\n";
                        cout << "lastMajorAlarmNotificationMillis: " << lastMajorAlarmNotificationMillis << "\n";

                        pushAlarmNotification(alarmPointer);
                        hasPushedMajorAlarmNotification = true;
                        alarmIndexToPush += 1;

                        lastAlarmNotificationPushMillis = currentMillis;
                    }

                    if (alarmIndexToPush >= (alarms.size() - 1)) {

                        cout << "major.notif. reset index" << "\n";

                        isNotifyingCriticalAlarms = true;

                        alarmIndexToPush = 0;

                        if (hasPushedMajorAlarmNotification) {
                            lastMajorAlarmNotificationMillis = currentMillis;
                        }

                        hasPushedMajorAlarmNotification = false;

                        for (uint8_t index = 0; index < alarms.size(); index++) {
                            alarms.get(0)->hasSentNotification = false;
                        }
                    }
                }

//                alarmIndexToPush += 1;
            }
        }
    }
};

#endif
