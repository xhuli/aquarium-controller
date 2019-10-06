#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_STATION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_STATION_H_
#pragma once

#include <Common/LinkedHashMap.h>
#include <Enums/StationState.h>
#include <Abstract/AbstractBuzzer.h>
#include <Abstract/ISleepable.h>
#include "AlarmList.h"
#include "AlarmNotifyConfiguration.h"

class AlarmStation :
        public AbstractRunnable,
        public ISleepable {

    AbstractBuzzer &buzzer;
    LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> &alarmNotifyConfigurations;

    uint32_t sleepStartMs = 0;
    uint32_t sleepPeriodMs = 0;
    StationState alarmStationState = StationState::Active;

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

    void setState(StationState const newStationState) {
        alarmStationState = newStationState;
    }

    StationState const &getState() const {
        return alarmStationState;
    }

    bool isInState(StationState const &compareState) const {
        return alarmStationState == compareState;
    }

    /* § Section: ISleepable Methods */

    void startSleeping(uint32_t const &sleepMillis) override {
        sleepPeriodMs = sleepMillis;
        sleepStartMs = millis();
        setState(StationState::Sleeping);
        buzzer.stop();
    }

    void stopSleeping() override {
        setState(StationState::Active);
    }

    /* § Section: Runnable Methods */

    void setup() override {
        // pass
    }

    void loop() override {
#ifdef __SERIAL_DEBUG__
        Serial << "------------------------------------------------\n";
        Serial << "AlarmStation::alarmList::size()\t\t" << static_cast<int>(alarmList.size()) << "\n";
        for (LinkedAlarm *pAlarm = alarmList.getFirst(); pAlarm; pAlarm = pAlarm->getNext()) {
            Serial << "\t\t" << getAlarmStationStateString(pAlarm) << "\tack:" << static_cast<int>(pAlarm->isAcknowledged()) << "\n";
        }
        Serial << "------------------------------------------------\n";
#endif
        if (alarmStationState == StationState::Sleeping) {
            if (millis() - sleepStartMs >= sleepPeriodMs) {
                stopSleeping();
            } else {
                return;
            }
        }

        if (!buzzer.isBusy()) {
            for (LinkedAlarm *pAlarm = alarmList.getFirst(); pAlarm; pAlarm = pAlarm->getNext()) {
                if (!(pAlarm->isAcknowledged())) {
                    //
                    uint32_t soundPeriodMs = alarmNotifyConfigurations
                            .getOrDefault(pAlarm->getSeverity(), defaultConfiguration)
                            .getSoundPeriodMinutes();

                    soundPeriodMs = soundPeriodMs * 60 * 1000ul; /* expand minutes to milliseconds */

                    uint32_t lastNotificationMs = pAlarm->getLastNotificationMs();

                    if (lastNotificationMs == 0 || (millis() - lastNotificationMs > soundPeriodMs)) {
                        //
                        pAlarm->setLastNotificationMs(millis());

                        buzzer.buzz(alarmNotifyConfigurations
                                            .getOrDefault(pAlarm->getSeverity(), defaultConfiguration)
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
    const char * getAlarmStationStateString(const LinkedAlarm *pAlarm) const {
        switch(pAlarm->getCode()) {
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
                return "";
        }
    }
#endif
};

#endif