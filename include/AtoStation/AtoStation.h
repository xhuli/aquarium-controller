#ifndef __ATO_STATION_H__
#define __ATO_STATION_H__

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

#endif

#include <stdint.h>  // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include <AlarmStation/AlarmCode.h>
#include <AlarmStation/AlarmStation.h>
#include <AlarmStation/AlarmSeverity.h>
#include "Abstract/AbstractDispenser.h"
#include "AtoSettings.h"
#include "Abstract/AbstractLiquidLevelSensor.h"
#include "Abstract/AbstractConfigurationStorage.h"

class AtoStation {
    AbstractDispenser *atoDispenserPointer = nullptr;

    AbstractConfigurationStorage *storagePointer = nullptr;

    AlarmStation *alarmStationPointer = nullptr;

    AbstractLiquidLevelSensor *mainLevelSensorPointer = nullptr;
    AbstractLiquidLevelSensor *backupHighLevelSensorPointer = nullptr;
    AbstractLiquidLevelSensor *backupLowLevelSensorPointer = nullptr;
    AbstractLiquidLevelSensor *reservoirLowLevelSensorPointer = nullptr;

    bool hasConfigurationStorage = false;
    bool hasAlarmStation = false;

    bool hasBackupHighLevelSensor = false;
    bool hasBackupLowLevelSensor = false;
    bool hasReservoirLowLevelSensor = false;

    uint32_t sleepStartMillis = 0;
    uint32_t sleepPeriodMillis = 0;
    uint32_t dispensingStartMillis = 0;
    uint32_t lastDispenseEndMillis = 0;

    AtoSettings atoSettings;

public:
    AtoStation(
            AbstractLiquidLevelSensor *mainLevelSensorToAttach,
            AbstractDispenser *atoDispenserPointer) :
            mainLevelSensorPointer(mainLevelSensorToAttach),
            atoDispenserPointer(atoDispenserPointer) {}

    enum State {
        INVALID,
        SLEEPING,
        SENSING,
        DISPENSING,
        BLOCKING,
    } state = State::INVALID;

    State getCurrentState() {
        return state;
    }

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

    bool attachAlarmStation(AlarmStation *alarmStationToAttach) {
        //
        if (alarmStationToAttach != nullptr) {
            //
            alarmStationPointer = alarmStationToAttach;
            hasAlarmStation = true;

            return true;
        }
        return false;
    }

    void attachBackupHighLevelSensor(AbstractLiquidLevelSensor *sensorPointer) {
        backupHighLevelSensorPointer = sensorPointer;
        hasBackupHighLevelSensor = true;
    }

    void attachBackupLowLevelSensor(AbstractLiquidLevelSensor *sensorPointer) {
        backupLowLevelSensorPointer = sensorPointer;
        hasBackupLowLevelSensor = true;
    }

    void attachReservoirLowLevelSensor(AbstractLiquidLevelSensor *sensorPointer) {
        reservoirLowLevelSensorPointer = sensorPointer;
        hasReservoirLowLevelSensor = true;
    }

    void sleep(uint16_t sleepMinutes) {
        //
        sleepPeriodMillis = sleepMinutes * 60ul * 1000ul;
//        sleepStartMillis = millis();
        sleepStartMillis = currentMillis;
        atoDispenserPointer->stopDispensing();
        state = State::SLEEPING;
    }

    void wake() {
        state = State::SENSING;
    }

    void setup() {
        //
        /* read configuration from storagePointer */
        if (hasConfigurationStorage && storagePointer->isStoredDataValid()) {
            atoSettings = storagePointer->readAtoSettings(atoSettings);
        }

        atoDispenserPointer->setup();

        /* must have at least main level sensor, other sensors are optional */
        mainLevelSensorPointer->setup();

        if (backupHighLevelSensorPointer != nullptr) {
            backupHighLevelSensorPointer->setup();
        }

        if (backupLowLevelSensorPointer != nullptr) {
            backupLowLevelSensorPointer->setup();
        }

        if (reservoirLowLevelSensorPointer != nullptr) {
            reservoirLowLevelSensorPointer->setup();
        }

        state = State::SENSING;
    }

    void reset() {
        lastDispenseEndMillis = millis() - atoSettings.minDispensingIntervalMillis - 1;

        if (hasAlarmStation) {
            //
            alarmStationPointer->deleteAlarmsInCodeRange(
                    AlarmCode::TopOffFailed,
                    AlarmCode::BackupLowSensorOff
            );
        }

        setup();
    }

    void update(uint32_t currentMillis) {
        //
        mainLevelSensorPointer->update(currentMillis);

        if (hasBackupHighLevelSensor) {
            backupHighLevelSensorPointer->update(currentMillis);
        }

        if (hasBackupLowLevelSensor) {
            backupLowLevelSensorPointer->update(currentMillis);
        }

        if (hasReservoirLowLevelSensor) {
            reservoirLowLevelSensorPointer->update(currentMillis);
        }

        switch (state) {
            //
            case State::SLEEPING:
                //
                if ((currentMillis - sleepStartMillis) > sleepPeriodMillis) {
                    state = State::SENSING;
                }

                break;

            case State::SENSING:
                //
                if (hasReservoirLowLevelSensor && reservoirLowLevelSensorPointer->isNotSensingLiquid()) {
                    //
                    state = State::BLOCKING;
                    if (hasAlarmStation) { alarmStationPointer->raiseAlarm(AlarmCode::ReservoirLow, AlarmSeverity::Major); }
                    break;
                }

                if (hasBackupHighLevelSensor && backupHighLevelSensorPointer->isSensingLiquid()) {
                    //
                    state = State::INVALID;
                    if (hasAlarmStation) { alarmStationPointer->raiseAlarm(AlarmCode::BackupHighSensorOn, AlarmSeverity::Critical); }
                    break;
                }

                if (hasBackupLowLevelSensor && backupLowLevelSensorPointer->isNotSensingLiquid()) {
                    //
                    state = State::INVALID;
                    if (hasAlarmStation) { alarmStationPointer->raiseAlarm(AlarmCode::BackupLowSensorOff, AlarmSeverity::Major); }
                    break;
                }

                if (mainLevelSensorPointer->isNotSensingLiquid() && ((currentMillis - lastDispenseEndMillis) > atoSettings.minDispensingIntervalMillis)) {
                    //
                    dispensingStartMillis = currentMillis;
                    atoDispenserPointer->startDispensing();

                    state = State::DISPENSING;
                }

                break;

            case State::DISPENSING:
                //
                if (hasReservoirLowLevelSensor && reservoirLowLevelSensorPointer->isNotSensingLiquid()) {
                    //
                    atoDispenserPointer->stopDispensing();
                    lastDispenseEndMillis = currentMillis;
                    state = State::BLOCKING;

                    if (hasAlarmStation) { alarmStationPointer->raiseAlarm(AlarmCode::ReservoirLow, AlarmSeverity::Major); }
                    break;
                }

                if (hasBackupHighLevelSensor && backupHighLevelSensorPointer->isSensingLiquid()) {
                    //
                    atoDispenserPointer->stopDispensing();
                    lastDispenseEndMillis = currentMillis;
                    state = State::INVALID;

                    if (hasAlarmStation) { alarmStationPointer->raiseAlarm(AlarmCode::BackupHighSensorOn, AlarmSeverity::Critical); }
                    break;
                }

                if (mainLevelSensorPointer->isSensingLiquid()) {
                    //
                    atoDispenserPointer->stopDispensing();
                    lastDispenseEndMillis = currentMillis;

                    state = State::SENSING;

                } else {
                    //
                    if ((currentMillis - dispensingStartMillis) > atoSettings.maxDispensingDurationMillis) {
                        //
                        atoDispenserPointer->stopDispensing();
                        lastDispenseEndMillis = currentMillis;
                        state = State::BLOCKING;

                        if (hasAlarmStation) { alarmStationPointer->raiseAlarm(AlarmCode::TopOffFailed, AlarmSeverity::Major); }
                        break;
                    }
                }

                break;

            case State::BLOCKING:
                //
                if (hasReservoirLowLevelSensor && reservoirLowLevelSensorPointer->isSensingLiquid()) {

                    state = State::SENSING;

                    if (hasAlarmStation) { alarmStationPointer->deleteAlarmByCode(AlarmCode::ReservoirLow); }
                }

                break;

            case State::INVALID:
            default:
                break;
        }
    }
};

#endif