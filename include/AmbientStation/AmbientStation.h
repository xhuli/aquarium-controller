#ifndef _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_STATION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_STATION_H_
#pragma once

#ifdef __TEST_MODE__
#include <stdint.h>
#include "../../test/_Mocks/MockCommon.h"

#endif

#include <Common/FunctionList.h>
#include <Enums/StationState.h>
#include <Abstract/AbstractRunnable.h>
#include <Abstract/ISleepable.h>
#include <AlarmStation/AlarmStation.h>
#include "AbientSettings.h"

/**
 * <br/>
 * Business logic uses no measurement units.<br/>
 * Measurement units (°C or °F) depend on concrete <tt>Sensor</tt> implementation.
 */
class AmbientStation :
        public AbstractRunnable,
        public ISleepable {

private:

    float ambientHumidity = -100.0f;
    float ambientTemperature = -100.0f;
    float systemTemperature = -100.0f;
    float waterTemperature = -100.0f;

    uint32_t sleepStartMs = 0;
    uint32_t sleepPeriodMs = 0;

    StationState ambientStationState = StationState::Active;

public:

    FunctionList rules;

    bool isInState(StationState const &compareState) const {
        return ambientStationState == compareState;
    }

    StationState getAmbientStationState() const {
        return ambientStationState;
    }

    /* § Section: Setters, Sensor Readings */
    void setAmbientHumidity(float const ambientHumidity) {
        AmbientStation::ambientHumidity = ambientHumidity;
    }

    void setAmbientTemperature(float const ambientTemperature) {
        AmbientStation::ambientTemperature = ambientTemperature;
    }

    void setSystemTemperature(float const systemTemperature) {
        AmbientStation::systemTemperature = systemTemperature;
    }

    void setWaterTemperature(float const waterTemperature) {
        AmbientStation::waterTemperature = waterTemperature;
    }

    /* § Section: Getters, Sensor Readings */
    float getAmbientHumidity() const {
        return ambientHumidity;
    }

    float getAmbientTemperature() const {
        return ambientTemperature;
    }

    float getSystemTemperature() const {
        return systemTemperature;
    }

    float getWaterTemperature() const {
        return waterTemperature;
    }

    /* § Section: ISleepable Methods */

    void startSleeping(uint32_t const &sleepMillis) override {
        /* !!! warn: IMPORTANT global function `millis()` must be defined !!! */
        sleepPeriodMs = sleepMillis;
        sleepStartMs = millis();
        // todo: decide what will happend with controlled devices !!!
        setState(StationState::Sleeping);
    }

    void stopSleeping() override {
        setState(StationState::Active);
    }

    /* § Section: Runnable Methods */

    void setup() override {
        // pass
    }

    void loop() override {
        //
        if (AmbientStation::isInState(StationState::Sleeping)) {
            /* !!! warn: IMPORTANT global function `millis()` must be defined !!! */
            if ((millis() - sleepStartMs) >= sleepPeriodMs) {
                setState(StationState::Active);
            }
        } else {
            rules.invokeAll();
        }
    }

private:

    /* § Section: State Methods */

    void setState(StationState const newStationState) {
        ambientStationState = newStationState;
    }
};

#endif