#ifndef __ABSTRACT_CONFIGURATION_STORAGE_H__
#define __ABSTRACT_CONFIGURATION_STORAGE_H__

#include "AlarmStation/AlarmSettings.h"
#include "AtoStation/AtoSettings.h"
#include "DosingStation/DosingSchedule.h"
#include "TemperatureControlStation/TemperatureControlSettings.h"

class AbstractConfigurationStorage {
   public:
    virtual ~AbstractConfigurationStorage() = default;

    virtual bool isStoredDataVersionCompatible() = 0;
    virtual bool isStoredDataCrcValid() = 0;
    virtual bool isStoredDataValid() = 0;

    virtual void saveDosingPortSchedule(uint8_t portNumber, DosingSchedule &dosingSchedule) = 0;
    virtual DosingSchedule& readDosingPortSchedule(uint8_t portNumber, DosingSchedule &dosingSchedule) = 0;

    virtual void saveDosingPortCalibration(uint8_t dosingPumpNumber, uint16_t millisPerMilliLiter) = 0;
    virtual uint16_t readDosingPortCalibration(uint8_t dosingPumpNumber, uint16_t millisPerMilliLiter) = 0;

    virtual void saveTemperatureControlSettings(TemperatureControlSettings& settings) = 0;
    virtual TemperatureControlSettings readTemperatureControlSettings(TemperatureControlSettings& settings) = 0;

    virtual void saveAtoSettings(AtoSettings& settings) = 0;
    virtual AtoSettings& readAtoSettings(AtoSettings& settings) = 0;

    virtual void saveAlarmStationSettings(AlarmSettings& settings) = 0;
    virtual AlarmSettings readAlarmStationSettings(AlarmSettings& settings) = 0;
};

#endif