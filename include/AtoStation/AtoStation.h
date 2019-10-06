#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_ATO_STATION_H__
#define _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_ATO_STATION_H__
#pragma once

#include <Enums/AlarmCode.h>
#include <Enums/AlarmSeverity.h>
#include <Enums/AtoStationState.h>
#include <Enums/LiquidLevelState.h>

#include <Abstract/AbstractRunnable.h>
#include <Abstract/ISleepable.h>
#include <Abstract/AbstractSwitchable.h>

#include <AlarmStation/AlarmStation.h>

#include "AtoSettings.h"

class AtoStation :
        public AbstractRunnable,
        public ISleepable {

private:

    AtoSettings &atoSettings;
    AbstractSwitchable &atoDispenser;

    AlarmStation *pAlarmStation = nullptr;

    LiquidLevelState normalLevelState = LiquidLevelState::Unknown;
    LiquidLevelState highLevelState = LiquidLevelState::Unknown;
    LiquidLevelState lowLevelState = LiquidLevelState::Unknown;
    LiquidLevelState reservoirLevelState = LiquidLevelState::Unknown;

    uint32_t currentMillis = 0;

    uint32_t sleepStartMs = 0;
    uint32_t sleepPeriodMs = 0;
    uint32_t dispensingStartMs = 0;

    AtoStationState atoStationState = AtoStationState::Invalid;
    bool hasTopOffFailed = false;

public:

    /* § Section: Public Methods */

    explicit AtoStation(AtoSettings &atoSettings, AbstractSwitchable &atoDispenserToAttach) :
            atoSettings(atoSettings),
            atoDispenser(atoDispenserToAttach),
            currentMillis(millis()) {}

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

    void setNormalLevelState(LiquidLevelState const normalLevelState) {
        AtoStation::normalLevelState = normalLevelState;
    }

    void setHighLevelState(LiquidLevelState const highLevelState) {
        AtoStation::highLevelState = highLevelState;
    }

    void setLowLevelState(LiquidLevelState const lowLevelState) {
        AtoStation::lowLevelState = lowLevelState;
    }

    void setReservoirLevelState(LiquidLevelState const reservoirLevelState) {
        AtoStation::reservoirLevelState = reservoirLevelState;
    }

    void startSleeping(uint32_t const &sleepMillis) override {
        sleepPeriodMs = sleepMillis;
        sleepStartMs = AtoStation::currentMillis;
        AtoStation::stopDispensing();
        AtoStation::setState(AtoStationState::Sleeping);
        AtoStation::deleteAllStationAlarms();
    }

    void stopSleeping() override {
        AtoStation::setState(AtoStationState::Sensing);
    }

    void reset() {
        AtoStation::deleteAllStationAlarms();
        AtoStation::setState(AtoStationState::Sensing);
    }

    void setup() override {
        // Serial << "AtoStation::setup()\n";
        AtoStation::setState(AtoStationState::Sensing);
    }

    void loop() override {
        //
#ifdef __SERIAL_DEBUG__
        // Serial << "AtoStation::loop()\n";
        Serial << "------------------------------------------------\n";
        Serial << "\tatoStationState: \t" << getAtoStationStateString(atoStationState);
        Serial << "------------------------------------------------\n";
        Serial << "\thighLevelState: \t" << getLiquidStateString(highLevelState);
        Serial << "\tnormalLevelState: \t" << getLiquidStateString(normalLevelState);
        Serial << "\tlowLevelState: \t\t" << getLiquidStateString(lowLevelState);
        Serial << "\treservoirLevelState: \t" << getLiquidStateString(reservoirLevelState);
        Serial << "\tAtoDispenser: \t" << getAtoDispenserStateString(atoDispenser.getState());
#endif

        currentMillis = millis(); /* !!! IMPORTANT make sure you provide global `millis()` function !!! */
        bool hasAlarm = false;

        switch (atoStationState) {
            //
            case AtoStationState::Sleeping:
                //
                if ((currentMillis - sleepStartMs) >= sleepPeriodMs) {
                    AtoStation::setState(AtoStationState::Sensing);
                }

                break;

            case AtoStationState::Sensing:
                //
                if (reservoirLevelState == LiquidLevelState::Low) {
                    AtoStation::setState(AtoStationState::Alarming);
                    raiseAlarm(AlarmCode::AtoReservoirLow, AlarmSeverity::Major);
                    hasAlarm = true;
//                    break;
                } else {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoReservoirLow);
                }

                if (highLevelState == LiquidLevelState::High) {
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoHighLevel, AlarmSeverity::Major);
                    hasAlarm = true;
//                    break;
                } else {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoHighLevel);
                }

                if (lowLevelState == LiquidLevelState::Low) {
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
                    hasAlarm = true;
//                    break;
                } else {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoLowLevel);
                }

                if (normalLevelState == LiquidLevelState::High) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoTopOffFailed);
                } else if (!hasAlarm && (normalLevelState == LiquidLevelState::Low) &&
                           (dispensingStartMs == 0 || ((currentMillis - dispensingStartMs) >= atoSettings.minDispensingIntervalMs))) {
                    //
                    AtoStation::setState(AtoStationState::Dispensing);
                    AtoStation::startDispensing();
                }

                break;

            case AtoStationState::Dispensing:
                //
                /* do not stop dispensing if reservoir is low */
                /*
                if (reservoirLevelState == LiquidLevelState::Low) {
                    AtoStation::stopDispensing();
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoReservoirLow, AlarmSeverity::Major);
                    break;
                }
                 */

                if (highLevelState == LiquidLevelState::High) {
                    AtoStation::stopDispensing();
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoHighLevel, AlarmSeverity::Major);
                    break;
                }

                if (lowLevelState == LiquidLevelState::Low) {
                    AtoStation::stopDispensing();
                    AtoStation::setState(AtoStationState::Alarming);
                    AtoStation::raiseAlarm(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
                    break;
                }

                if (normalLevelState == LiquidLevelState::High) {
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
                if (reservoirLevelState == LiquidLevelState::High) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoReservoirLow);
                } else if (reservoirLevelState == LiquidLevelState::Low) {
                    AtoStation::raiseAlarm(AlarmCode::AtoReservoirLow, AlarmSeverity::Major);
                }

                if (highLevelState == LiquidLevelState::Low) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoHighLevel);
                } else if (highLevelState == LiquidLevelState::High) {
                    AtoStation::raiseAlarm(AlarmCode::AtoHighLevel, AlarmSeverity::Major);
                }

                if (lowLevelState == LiquidLevelState::High) {
                    AtoStation::acknowledgeAlarm(AlarmCode::AtoLowLevel);
                } else if (lowLevelState == LiquidLevelState::Low) {
                    AtoStation::raiseAlarm(AlarmCode::AtoLowLevel, AlarmSeverity::Major);
                }

                if ((highLevelState == LiquidLevelState::Low || highLevelState == LiquidLevelState::Unknown) &&
                    (lowLevelState == LiquidLevelState::High || lowLevelState == LiquidLevelState::Unknown) &&
                    (reservoirLevelState == LiquidLevelState::High || reservoirLevelState == LiquidLevelState::Unknown) &&
                    (normalLevelState == LiquidLevelState::High || !hasTopOffFailed)) {
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

#ifdef __ATO_MODE_TESTING_PLATFORMIO__

    /* § Section: Debug/Troubleshoot methods */

    const char *getAtoDispenserStateString(Switched switched) const {
        switch (switched) {
            case Switched::Off:
                return "OFF\n";
            case Switched::On:
                return "ON\n";
            default:
                return " -.- \n";
        }
    }

    const char *getAtoStationStateString(AtoStationState stationState) const {
        switch (stationState) {
            case AtoStationState::Sensing:
                return "Sensing\n";
            case AtoStationState::Dispensing:
                return "Dispensing\n";
            case AtoStationState::Sleeping:
                return "Sleeping\n";
            case AtoStationState::Alarming:
                return "Alarming\n";
            case AtoStationState::Invalid:
                return "Invalid\n";
            default:
                return " -.- \n";
        }
    }

    const char *getLiquidStateString(LiquidLevelState liquidLevelState) const {
        switch (liquidLevelState) {
            case LiquidLevelState::Low:
                return "Low\n";
            case LiquidLevelState::High:
                return "High\n";
            case LiquidLevelState::Unknown:
                return "Unknown\n";
            default:
                return " -.- \n";
        }
    }
#endif
};

#endif