#ifndef __AQUARIUM_CONTROLLER_ABSTRACT_ALARM_LISTENER_H__
#define __AQUARIUM_CONTROLLER_ABSTRACT_ALARM_LISTENER_H__

#include "AlarmStation/Alarm.h"

class AbstractAlarmListener {
public:
    virtual ~AbstractAlarmListener() = default;

    virtual void notifyOnAlarm(Alarm *alarm) = 0;
};

#endif
