#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_STATION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_STATION_H_
#pragma once

#include <Common/LinkedHashMap.h>
#include <Enums/State.h>
#include <Abstract/AbstractBuzzer.h>
#include <Abstract/AbstractSleepable.h>
#include "AlarmList.h"
#include "AlarmNotifyConfiguration.h"

class AlarmStation :
        public AbstractRunnable,
        public AbstractSleepable {

    AbstractBuzzer &buzzer;
    LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> &alarmNotifyConfigurations;

    State alarmStationState = State::Active;

    AlarmNotifyConfiguration defaultConfiguration{5, 5000};

public:

    AlarmList alarmList;

    explicit AlarmStation(
            AbstractBuzzer &buzzer,
            LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> &alarmNotifyConfigurations
    ) :
            buzzer(buzzer),
            alarmNotifyConfigurations(alarmNotifyConfigurations) {}

    /* § Section: State Methods */

    void setState(State const newStationState) {
        alarmStationState = newStationState;
    }

    State const &getState() const {
        return alarmStationState;
    }

    bool isInState(State const &compareState) const {
        return alarmStationState == compareState;
    }

    /* § Section: ISleepable Methods */

    void startSleeping(uint32_t const &sleepMs) override {
        AbstractSleepable::startSleeping(sleepMs);
        AlarmStation::setState(State::Sleeping);
        buzzer.stop();
    }

    void stopSleeping() override {
        AbstractSleepable::stopSleeping();
        AlarmStation::setState(State::Active);
    }

    /* § Section: Runnable Methods */

    void setup() override {
        // pass
    }

    void loop() override {
#ifdef __SERIAL_DEBUG__
        Serial << "------------------------------------------------\n";
        Serial << "AlarmStation::alarmList::size()\t\t" << static_cast<int>(alarmList.size()) << "\n";
        alarmList.forEach([](Alarm alarm) {
            Serial << "\t\t" << getAlarmCodeString(alarm) << "\tack:" << static_cast<int>(alarm.isAcknowledged()) << "\n";
        });
        Serial << "------------------------------------------------\n";
#endif
        if (AlarmStation::isInState(State::Sleeping)) {
            if (AbstractSleepable::shouldStopSleeping()) {
                AlarmStation::stopSleeping();
            } else {
                return;
            }
        }

        if (!buzzer.isBusy()) {
            for (Element<Alarm> *pElement = alarmList.getFirstElement(); pElement; pElement = pElement->getNext()) {
                if (!(pElement->value.isAcknowledged())) {
                    //
                    uint32_t soundPeriodMs = alarmNotifyConfigurations
                            .getOrDefault(pElement->value.getSeverity(), defaultConfiguration)
                            .getSoundPeriodMinutes();

                    soundPeriodMs = soundPeriodMs * 60 * 1000ul; /* expand minutes to milliseconds */

                    uint32_t lastNotificationMs = pElement->value.getLastNotificationMs();

                    if (lastNotificationMs == 0 || (millis() - lastNotificationMs > soundPeriodMs)) {
                        //
                        pElement->value.setLastNotificationMs(millis());

                        buzzer.buzz(alarmNotifyConfigurations
                                            .getOrDefault(pElement->value.getSeverity(), defaultConfiguration)
                                            .getSoundDurationMs()
                        );

                        break;
                    }
                }
            }
        }
    }

private:

#ifdef __SERIAL_DEBUG__

    static const char *getAlarmCodeString(const Alarm alarm) {
        switch (alarm.getCode()) {
            case AlarmCode::AmbientMaxTemperatureReached:
                return "AmbientMaxTemperatureReached";
            case AlarmCode::AmbientMaxHumidityReached:
                return "AmbientMaxHumidityReached";
            case AlarmCode::SystemMaxTemperatureReached:
                return "SystemMaxTemperatureReached";
            case AlarmCode::WaterMinTemperatureReached:
                return "WaterMinTemperatureReached";
            case AlarmCode::WaterMaxTemperatureReached:
                return "WaterMaxTemperatureReached";
            case AlarmCode::AtoTopOffFailed:
                return "AtoTopOffFailed";
            case AlarmCode::AtoReservoirLow:
                return "AtoReservoirLow";
            case AlarmCode::AtoHighLevel:
                return "AtoHighLevel";
            case AlarmCode::AtoLowLevel:
                return "AtoLowLevel";
            case AlarmCode::NoAlarm:
                return "NoAlarm";

            default:
                return " --.-- ";
        }
    }

#endif
};

#endif