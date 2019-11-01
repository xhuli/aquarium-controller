#ifndef _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_STATION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_AMBIENT_STATION_AMBIENT_STATION_H_
#pragma once

#ifdef __TEST_MODE__

#include <stdint.h>
#include "../../test/_Mocks/MockCommon.h"

#endif

#include <Common/FunctionList.h>
#include <Enums/State.h>
#include <Abstract/AbstractRunnable.h>
#include <Abstract/AbstractSleepable.h>
#include <AlarmStation/AlarmStation.h>
#include "AbientSettings.h"

/**
 * <br/>
 * Business logic uses no measurement units.<br/>
 * Measurement units (°C or °F) depend on concrete <tt>Sensor</tt> implementation.
 */
class AmbientStation :
        public AbstractRunnable,
        public AbstractSleepable {

private:

    float ambientHumidity = -100.0f;
    float ambientTemperature = -100.0f;
    float systemTemperature = -100.0f;
    float waterTemperature = -100.0f;

    State ambientStationState = State::Active;

public:

    FunctionList rules;

    bool isInState(State const &compareState) const {
        return ambientStationState == compareState;
    }

    State getAmbientStationState() const {
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

    void startSleeping(uint32_t const &sleepMs) override {
        AbstractSleepable::startSleeping(sleepMs);
        // todo: decide what will happen with controlled devices !!!
        AmbientStation::setState(State::Sleeping);
    }

    void stopSleeping() override {
        AbstractSleepable::stopSleeping();
        AmbientStation::setState(State::Active);
    }

    /* § Section: Runnable Methods */

    void setup() override {}

    void loop() override {
        //
        if (AmbientStation::isInState(State::Sleeping)) {
            if (AbstractSleepable::shouldStopSleeping()) {
                AmbientStation::stopSleeping();
            }
        } else {
            rules.invokeAll();
        }
    }

private:

    /* § Section: State Methods */

    void setState(State const newStationState) {
        ambientStationState = newStationState;
    }
};

#endif