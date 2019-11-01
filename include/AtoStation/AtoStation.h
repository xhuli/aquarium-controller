#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_ATO_STATION_H__
#define _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_ATO_STATION_H__
#pragma once

#include <Enums/AlarmCode.h>
#include <Enums/AlarmSeverity.h>
#include <Enums/AtoStationState.h>
#include <Enums/Level.h>

#include <Common/Switchable.h>
#include <Abstract/AbstractRunnable.h>
#include <Abstract/AbstractSleepable.h>

#include <AlarmStation/AlarmStation.h>

#include "AtoSettings.h"

class AtoStation :
        public AbstractRunnable,
        public AbstractSleepable {

private:

    AtoSettings &atoSettings;
    Switchable &atoDispenser;

    AlarmStation *pAlarmStation = nullptr;

    Level normalLevelState = Level::Unknown;
    Level highLevelState = Level::Unknown;
    Level lowLevelState = Level::Unknown;
    Level reservoirLevelState = Level::Unknown;

    uint32_t currentMillis = 0;
    uint32_t dispensingStartMs = 0;

    AtoStationState atoStationState = AtoStationState::Invalid;
    bool hasTopOffFailed = false;

public:

    /* § Section: Public Methods */

    explicit AtoStation(AtoSettings &atoSettings, Switchable &atoDispenserToAttach) :
            atoSettings(atoSettings),
            atoDispenser(atoDispenserToAttach) {}

    ~AtoStation() = default;

    AtoStationState const &getState() const {
        return atoStationState;
    }

    bool isInState(AtoStationState const &compareState) const {
        return atoStationState == compareState;
    }

    void attachAlarmStation(AlarmStation *const pAlarmStation) {
        if (pAlarmStation != nullptr) {
            AtoStation::pAlarmStation = pAlarmStation;
        }
    }

    void setNormalLevelState(Level const level) {
        AtoStation::normalLevelState = level;
    }

    void setHighLevelState(Level const level) {
        AtoStation::highLevelState = level;
    }

    void setLowLevelState(Level const level) {
        AtoStation::lowLevelState = level;
    }

    void setReservoirLevelState(Level const level) {
        AtoStation::reservoirLevelState = level;
    }

    void startSleeping(uint32_t const &sleepMs) override {
        AbstractSleepable::startSleeping(sleepMs);

        AtoStation::stopDispensing();
        AtoStation::setState(AtoStationState::Sleeping);
        AtoStation::deleteAllStationAlarms();
    }

    void stopSleeping() override {
        AbstractSleepable::stopSleeping();
        AtoStation::setState(AtoStationState::Sensing);
    }

    void reset() {
        AtoStation::deleteAllStationAlarms();
        AtoStation::setState(AtoStationState::Sensing);
    }

    void setup() override {
        AtoStation::syncMillis();
        AtoStation::setState(AtoStationState::Sensing);
    }

    void loop() override {
        //
#ifdef __SERIAL_DEBUG__
        Serial << "------------------------------------------------\n";
        Serial << "\tatoStationState: \t" << getAtoStationStateString(atoStationState) << "\n";
        Serial << "------------------------------------------------\n";
        Serial << "\thighLevelState: \t" << getLiquidStateString(highLevelState) << "\n";
        Serial << "\tnormalLevelState: \t" << getLiquidStateString(normalLevelState) << "\n";
        Serial << "\tlowLevelState: \t\t" << getLiquidStateString(lowLevelState) << "\n";
        Serial << "\treservoirLevelState: \t" << getLiquidStateString(reservoirLevelState) << "\n";
        Serial << "\tAtoDispenser: \t" << getAtoDispenserStateString(atoDispenser.getState()) << "\n";
#endif

        AtoStation::syncMillis();
        bool hasAlarm = false;

        switch (atoStationState) {
            //
            case AtoStationState::Sleeping:
                //
                if (AbstractSleepable::shouldStopSleeping()) {
                    AtoStation::stopSleeping();
                }

                break;

            case AtoStationState::Sensing:
                //
                if (reservoirLevelState == Level::Low) {
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoReservoirLow, AlarmSeverity::Major);
                    hasAlarm = true;
                } else {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoReservoirLow);
                }

                if (highLevelState == Level::High) {
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoHighLevel, AlarmSeverity::Major);
                    hasAlarm = true;
                } else {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoHighLevel);
                }

                if (lowLevelState == Level::Low) {
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
                    hasAlarm = true;
                } else {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoLowLevel);
                }

                if (normalLevelState == Level::High) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoTopOffFailed);
                } else if (!hasAlarm && (normalLevelState == Level::Low) &&
                           (dispensingStartMs == 0 || ((currentMillis - dispensingStartMs) >= atoSettings.minDispensingIntervalMs))) {
                    //
                    AtoStation::setState(AtoStationState::Dispensing);
                    AtoStation::startDispensing();
                }

                break;

            case AtoStationState::Dispensing:
                //
                if (highLevelState == Level::High) {
                    AtoStation::stopDispensing();
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoHighLevel, AlarmSeverity::Major);
                    break;
                }

                if (lowLevelState == Level::Low) {
                    AtoStation::stopDispensing();
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
                    break;
                }

                if (normalLevelState == Level::High) {
                    AtoStation::stopDispensing();
                    AtoStation::setState(AtoStationState::Sensing);
                } else if (dispensingStartMs == 0 || ((currentMillis - dispensingStartMs) >= atoSettings.maxDispensingDurationMs)) {
                    //
                    AtoStation::stopDispensing();
                    hasTopOffFailed = true;
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoTopOffFailed, AlarmSeverity::Major);
                    break;
                }

                break;

            case AtoStationState::Alarming:
                //
                if (reservoirLevelState == Level::High) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoReservoirLow);
                } else if (reservoirLevelState == Level::Low) {
                    AtoStation::raiseAlarm(AlarmCode::AtoReservoirLow, AlarmSeverity::Major);
                }

                if (highLevelState == Level::Low) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoHighLevel);
                } else if (highLevelState == Level::High) {
                    AtoStation::raiseAlarm(AlarmCode::AtoHighLevel, AlarmSeverity::Major);
                }

                if (lowLevelState == Level::High) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoLowLevel);
                } else if (lowLevelState == Level::Low) {
                    AtoStation::raiseAlarm(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
                }

                if ((highLevelState == Level::Low || highLevelState == Level::Unknown) &&
                    (lowLevelState == Level::High || lowLevelState == Level::Unknown) &&
                    (reservoirLevelState == Level::High || reservoirLevelState == Level::Unknown) &&
                    (normalLevelState == Level::High || !hasTopOffFailed)) {
                    //
                    AtoStation::setState(AtoStationState::Sensing);
                }

                break;

            default:
                break;
        }
    }

private:

/* § Section: Private Methods */

    inline void syncMillis() {
        AtoStation::currentMillis = millis();
    }

    void raiseAlarm(AlarmCode const &alarmCode, AlarmSeverity const &alarmSeverity) const {
        if (pAlarmStation != nullptr) {
            pAlarmStation->alarmList.add(alarmCode, alarmSeverity);
        }
    }

    void acknowledgeAlarm(AlarmCode const alarmCode) const {
        if (pAlarmStation != nullptr) {
            pAlarmStation->alarmList.acknowledge(alarmCode);
        }
    }

    void deleteAlarm(AlarmCode const alarmCode) const {
        if (pAlarmStation != nullptr) {
            pAlarmStation->alarmList.remove(alarmCode);
        }
    }

    void deleteAllStationAlarms() const {
        AtoStation::deleteAlarm(AlarmCode::AtoTopOffFailed);
        AtoStation::deleteAlarm(AlarmCode::AtoHighLevel);
        AtoStation::deleteAlarm(AlarmCode::AtoLowLevel);
    }

    void startDispensing() {
#ifdef __SERIAL_DEBUG__
        // Serial << "\t\tAtoStation::startDispensing()\n";
#endif
        dispensingStartMs = currentMillis;
        atoDispenser.setState(Switched::On);
    }

    void stopDispensing() const {
#ifdef __SERIAL_DEBUG__
        // Serial << "\t\tAtoStation::stopDispensing()\n";
#endif
        atoDispenser.setState(Switched::Off);
    }

    void setState(AtoStationState const newAtoStationState) {
        atoStationState = newAtoStationState;
    }

#ifdef __SERIAL_DEBUG__

    /* § Section: Debug/Troubleshoot methods */

    const char *getAtoDispenserStateString(Switched switched) const {
        switch (switched) {
            case Switched::Off:
                return "OFF";
            case Switched::On:
                return "ON";
            default:
                return " -.- ";
        }
    }

    const char *getAtoStationStateString(AtoStationState stationState) const {
        switch (stationState) {
            case AtoStationState::Sensing:
                return "Sensing";
            case AtoStationState::Dispensing:
                return "Dispensing";
            case AtoStationState::Sleeping:
                return "Sleeping";
            case AtoStationState::Alarming:
                return "Alarming";
            case AtoStationState::Invalid:
                return "Invalid";
            default:
                return " -.- ";
        }
    }

    const char *getLiquidStateString(Level liquidLevelState) const {
        switch (liquidLevelState) {
            case Level::Low:
                return "Low";
            case Level::High:
                return "High";
            case Level::Unknown:
                return "Unknown";
            default:
                return " -.- ";
        }
    }
#endif
};

#endif