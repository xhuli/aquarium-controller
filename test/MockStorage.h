#ifndef __MOCK_STORAGE_H__
#define __MOCK_STORAGE_H__

#include <stdint.h>

#include "AtoStation/AtoSettings.h"
#include "DosingStation/DosingSchedule.h"
#include "Storage/AbstractConfigurationStorage.h"
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

    void saveDosingPumpSchedule(
            uint8_t dosingPumpNumber,
            DosingSchedule &dosingSchedule) override {
        // pass
    }

    DosingSchedule &readDosingPumpSchedule(
            uint8_t dosingPumpNumber,
            DosingSchedule &dosingSchedule) override {
        return dosingSchedule;
    }

    void saveDosingPumpCalibration(
            uint8_t dosingPumpNumber,
            uint16_t millisPerMilliLiter) override {
        // pass
    }

    uint32_t readDosingPumpCalibration(
            uint8_t dosingPumpNumber,
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
};

#endif