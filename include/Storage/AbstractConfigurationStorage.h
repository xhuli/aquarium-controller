#ifndef __ABSTRACT_CONFIGURATION_STORAGE_H__
#define __ABSTRACT_CONFIGURATION_STORAGE_H__

#include "AtoStation/AtoSettings.h"
#include "DosingStation/DosingSchedule.h"
#include "TemperatureControlStation/TemperatureControlSettings.h"

class AbstractConfigurationStorage {
   public:
    virtual ~AbstractConfigurationStorage() = default;

    virtual bool isStoredDataVersionCompatible() = 0;
    virtual bool isStoredDataCrcValid() = 0;
    virtual bool isStoredDataValid() = 0;

    virtual void saveDosingPumpSchedule(uint8_t dosingPumpNumber, DosingSchedule& dosingSchedule) = 0;
    virtual DosingSchedule& readDosingPumpSchedule(uint8_t dosingPumpNumber, DosingSchedule& dosingSchedule) = 0;

    virtual void saveDosingPumpCalibration(uint8_t dosingPumpNumber, uint16_t millisPerMilliLiter) = 0;
    virtual uint32_t readDosingPumpCalibration(uint8_t dosingPumpNumber, uint16_t millisPerMilliLiter) = 0;

    virtual void saveTemperatureControlSettings(TemperatureControlSettings& settings) = 0;
    virtual TemperatureControlSettings readTemperatureControlSettings(TemperatureControlSettings& settings) = 0;

    virtual void saveAtoSettings(AtoSettings& settings) = 0;
    virtual AtoSettings& readAtoSettings(AtoSettings& settings) = 0;
};

#endif