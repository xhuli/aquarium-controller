#ifndef __ATO_STATION_H__
#define __ATO_STATION_H__

#ifdef __TEST_MODE__

#include "../../test/_Mocks/MockCommon.h"

#endif

#include <stdint.h>  // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include "Abstract/AbstractDispenser.h"
#include "AtoSettings.h"
#include "Abstract/AbstractLiquidLevelSensor.h"
#include "Abstract/AbstractConfigurationStorage.h"

class AtoStation {
#ifdef __TEST_MODE__
public:
#else
    private:
#endif
    AbstractConfigurationStorage *storagePointer;
    AbstractDispenser *atoDispenserPointer;

    AbstractLiquidLevelSensor *mainLevelSensorPointer = nullptr;
    AbstractLiquidLevelSensor *backupHighLevelSensorPointer = nullptr;
    AbstractLiquidLevelSensor *backupLowLevelSensorPointer = nullptr;
    AbstractLiquidLevelSensor *reservoirLowLevelSensorPointer = nullptr;

    bool hasMainLevelSensor = false;
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
            AbstractConfigurationStorage *configurationStoragePointer,
            AbstractDispenser *atoDispenserPointer) : storagePointer(configurationStoragePointer),
                                                         atoDispenserPointer(atoDispenserPointer) {
    }

    enum class State {
        INVALID = 0,
        SLEEPING = 1,
        SENSING = 2,
        DISPENSING = 3,
        RESERVOIR_LOW = 4,
    } state = State::INVALID;

    State getCurrentState() {
        return state;
    }

    void attachMainLevelSensor(AbstractLiquidLevelSensor *sensorPointer) {
        mainLevelSensorPointer = sensorPointer;
        hasMainLevelSensor = true;
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

    bool raiseAlarmBackupHighSensorIsSensingLiquid() {
        //
        if (hasBackupHighLevelSensor && backupHighLevelSensorPointer->isSensingLiquid()) {
            //
            // todo: raise alarm
            // ATO ALARM
            // BACKUP HIGH SENSOR ON

            std::cout << "Alarm: " << "BACKUP HIGH SENSOR ON" << std::endl;

            return true;
        }
        return false;
    }

    bool raiseAlarmBackupLowSensorIsNotSensingLiquid() {
        //
        if (hasBackupLowLevelSensor && backupLowLevelSensorPointer->isNotSensingLiquid()) {
            //
            // todo: raise alarm
            // ATO ALARM
            // BACKUP LOW SENSOR OFF

            std::cout << "Alarm: " << "BACKUP LOW SENSOR OFF" << std::endl;

            return true;
        }
        return false;
    }

    bool raiseAlarmReservoirLowSensorIsNotSensingLiquid() {
        //
        if (hasReservoirLowLevelSensor && reservoirLowLevelSensorPointer->isNotSensingLiquid()) {
            //
            // todo: raise alarm
            // ATO ALARM
            // RESERVOIR LOW LEVEL

            std::cout << "Alarm: " << "RESERVOIR LOW LEVEL" << std::endl;

            return true;
        }
        return false;
    }

    bool raiseAlarmTopOffFailedAfterMaxDispenseDuration() {
        //
        // todo: raise alarm
        // ATO ALARM
        // TOP OFF FAILED

        std::cout << "Alarm: " << "TOP OFF FAILED" << std::endl;
        
        return true;
    }

    void sleep(uint16_t sleepMinutes) {
        //
        sleepPeriodMillis = sleepMinutes * 60ul * 1000ul;
        sleepStartMillis = millis();
        atoDispenserPointer->stopDispensing();
        state = State::SLEEPING;
    }

    void wake() {
        state = State::SENSING;
    }

    void setup() {
        //
        /* read configuration from storage */
        if (storagePointer->isStoredDataValid()) {
            atoSettings = storagePointer->readAtoSettings(atoSettings);
        }

        atoDispenserPointer->setup();

        /* must have at least main level sensor, other sensors are optional */
        if (hasMainLevelSensor) {
            state = State::SENSING;
        } else {
            // todo: raise alarm

            state = State::INVALID;
        }

        if (backupHighLevelSensorPointer != nullptr) {
            backupHighLevelSensorPointer->setup();
        }

        if (backupLowLevelSensorPointer != nullptr) {
            backupLowLevelSensorPointer->setup();
        }

        if (reservoirLowLevelSensorPointer != nullptr) {
            reservoirLowLevelSensorPointer->setup();
        }
    }

    void reset() {
        lastDispenseEndMillis = millis() - atoSettings.minDispensingIntervalMillis - 1;
        setup();
    }

    void update(uint32_t currentMillis) {
        //
        if (hasMainLevelSensor) {
            mainLevelSensorPointer->update(currentMillis);
        }

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
                    raiseAlarmReservoirLowSensorIsNotSensingLiquid();
                    state = State::RESERVOIR_LOW;
                    break;
                }

                if (hasBackupHighLevelSensor && backupHighLevelSensorPointer->isSensingLiquid()) {
                    //
                    raiseAlarmBackupHighSensorIsSensingLiquid();
                    state = State::INVALID;
                    break;
                }

                if (hasBackupLowLevelSensor && backupLowLevelSensorPointer->isNotSensingLiquid()) {
                    //
                    raiseAlarmBackupLowSensorIsNotSensingLiquid();
                    state = State::INVALID;
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
                    raiseAlarmReservoirLowSensorIsNotSensingLiquid();

                    state = State::RESERVOIR_LOW;
                    break;
                }

                if (hasBackupHighLevelSensor && backupHighLevelSensorPointer->isSensingLiquid()) {
                    //
                    atoDispenserPointer->stopDispensing();
                    lastDispenseEndMillis = currentMillis;
                    raiseAlarmBackupHighSensorIsSensingLiquid();

                    state = State::INVALID;
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

                        // todo: raise alarm
                        // ATO ALARM
                        // TOP OFF FAILED

                        raiseAlarmTopOffFailedAfterMaxDispenseDuration();
                        state = State::RESERVOIR_LOW;
                        break;
                    }
                }

                break;

            case State::RESERVOIR_LOW:
                //
                if (hasReservoirLowLevelSensor && reservoirLowLevelSensorPointer->isSensingLiquid()) {
                    state = State::SENSING;
                }

                break;

            case State::INVALID:
            default:
                break;
        }
    }
};

#endif