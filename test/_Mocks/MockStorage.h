#ifndef __MOCK_STORAGE_H__
#define __MOCK_STORAGE_H__

#include <stdint.h>

#include "AtoStation/AtoSettings.h"
#include "DosingStation/DosingSchedule.h"
#include "Abstract/AbstractConfigurationStorage.h"
#include "TemperatureControlStation/TemperatureControlSettings.h"

class MockStorage : public AbstractConfigurationStorage {
public:
    MockStorage() = default;

    ~MockStorage() override = default;

    bool isStoredDataVersionCompatible() override {
        return true;
    }

    bool isStoredDataCrcValid() override {
        return true;
    }

    bool isStoredDataValid() override {
        return true;
    }

    void saveDosingPortSchedule(
            uint8_t portNumber,
            DosingSchedule &dosingSchedule) override {
        // pass
    }

    DosingSchedule &readDosingPortSchedule(
            uint8_t portNumber,
            DosingSchedule &dosingSchedule) override {
        return dosingSchedule;
    }

    void saveDosingPortCalibration(
            uint8_t portNumber,
            uint16_t millisPerMilliLiter) override {
        // pass
    }

    uint16_t readDosingPortCalibration(
            uint8_t portNumber,
            uint16_t millisPerMilliLiter) override {
        return millisPerMilliLiter;
    }

    void saveTemperatureControlSettings(TemperatureControlSettings &settings) override {
        // pass
    }

    TemperatureControlSettings readTemperatureControlSettings(TemperatureControlSettings &settings) override {
        return settings;
    }

    void saveAtoSettings(AtoSettings &settings) override {
        // pass
    }

    AtoSettings &readAtoSettings(AtoSettings &settings) override {
        return settings;
    }

    void saveAlarmStationSettings(AlarmSettings &settings) override {
        // pass
    }

    virtual AlarmSettings readAlarmStationSettings(AlarmSettings &settings) override {
        return settings;
    }

};

#endif