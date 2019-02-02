#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <EEPROM.h>
#include "AtoStation/AtoSettings.h"
#include "DosingStation/DosingSchedule.h"
#include "DosingStation/DosingTask.h"
#include "TemperatureControlStation/TemperatureControlSettings.h"

/*
    ----------------------------------------------------
    EEPROM allocate to	    first byte	last byte	size
    ----------------------------------------------------
    reserved	            0	        31	        32
        crc                 0           3           4
        version             4           5           2
    temperature control	    32	        63	        32
    ato control	            64          95	        32
    future 1	            96	        127	        32
    future 2	            128	        159	        32
    future 3        	    160	        223	        32
    dosing calibrations     192     	223     	32
    dosing schedule 1	    224	        271	        48
    dosing schedule 2	    272	        319	        48
    dosing schedule 3	    320	        367	        48
    dosing schedule 4	    368	        415	        48
    dosing schedule 5	    416	        463	        48
    dosing schedule 6	    464	        511	        48
    dosing schedule 7	    512	        559	        48
    dosing schedule 8	    560	        607	        48
    dosing schedule 9	    608	        655	        48
    dosing schedule 10	    656	        703	        48
    dosing schedule 11	    704	        751	        48
    dosing schedule 12	    752	        799	        48
    dosing schedule 13	    800	        847	        48
    dosing schedule 14	    848	        895	        48
    dosing schedule 15	    896	        943	        48
    dosing schedule 16	    944	        991	        48
    free	                992	        1023	    32
*/

#define STORAGE_TEMPERATURE_CONTROL_START_ADDRESS 32
#define STORAGE_ATO_CONTROL_START_ADDRESS 64
#define STORAGE_FUTURE_1_START_ADDRESS 96
#define STORAGE_FUTURE_2_START_ADDRESS 128
#define STORAGE_FUTURE_3_START_ADDRESS 160
#define STORAGE_DOSING_CALIBRATIONS_START_ADDRESS 192
#define STORAGE_DOSING_SCHEDULES_START_ADDRESS 224

#ifndef MAX_NUMBER_OF_DOSING_TASKS
#define MAX_NUMBER_OF_DOSING_TASKS 16
#endif

#define STORAGE_VERSION_MAJOR 0
#define STORAGE_VERSION_MINOR 1

/*
    todo: 
    • check versions
    • wipe out the eeprom
*/

class Storage {
   private:
    uint8_t storageVersionMajor;
    uint8_t storageVersionMinor;
    uint32_t eepromAddress;
    uint32_t eepromCrc;
    bool isEeepromCrcValid = false;

   public:
    Storage() {
        if (readEeepromCrc(eepromCrc) != calculateEepromCrc()) {
#ifdef __GDZU_DEBUG__
            Serial.println("-------------------------");
            Serial.println(" EEPROM CRC not matching");
            Serial.println("-------------------------");
#endif
            // EEPROM.put(4, storageVersionMajor);
            // EEPROM.put(5, storageVersionMinor);
        } else {
            // read settings from eeprom
#ifdef __GDZU_DEBUG__
            Serial.println("---------------");
            Serial.println(" EEPROM CRC OK");
            Serial.println("---------------");
#endif
        }

        if (isStoredDataVersionCompatible()) {
#ifdef __GDZU_DEBUG__
            Serial.println("-----------------------------------------------------");
            Serial.println(" Major version is not matching!");
            Serial.println(" Stored data is not compatible with current software.");
            Serial.println("------------------------------------------------------");
#endif
        }
    }

    bool isStoredDataVersionCompatible() {
        storageVersionMajor = EEPROM.get(4, storageVersionMajor);
        return (storageVersionMajor == STORAGE_VERSION_MAJOR);
    }

    bool isStoredDataCrcValid() {
        return (readEeepromCrc(eepromCrc) == calculateEepromCrc());
    }

    bool isStoredDataValid() {
        return (isStoredDataCrcValid() && isStoredDataVersionCompatible());
    }

    /* CRC */
    uint32_t calculateEepromCrc() {
        //
        const uint32_t crc_table[16] = {
            0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
            0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
            0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
            0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c};

        uint32_t crc = ~0L;

        /* the crc is stored in the first 4 eeprom bytes, therefore 'i' starts from 4 */
        for (uint16_t index = 4; index < EEPROM.length(); ++index) {
            crc = crc_table[(crc ^ EEPROM[index]) & 0x0f] ^ (crc >> 4);
            crc = crc_table[(crc ^ (EEPROM[index] >> 4)) & 0x0f] ^ (crc >> 4);
            crc = ~crc;
        }

        return crc;
    }

    uint32_t readEeepromCrc(uint32_t eepromCrc) {
        //
        return EEPROM.get(0, eepromCrc);
    }

    void saveEeepromCrc(uint32_t eepromCrc) {
        //
        EEPROM.put(0, eepromCrc);
    }

    /* dosing pumps schedule */
    void saveDosingPumpSchedule(uint8_t dosingPumpNumber, DosingSchedule &dosingSchedule) {
        //
        eepromAddress = STORAGE_DOSING_SCHEDULES_START_ADDRESS + (dosingPumpNumber - 1) * MAX_NUMBER_OF_DOSING_TASKS * sizeof(DosingTask);

        for (uint8_t taskIndex = 0; taskIndex < MAX_NUMBER_OF_DOSING_TASKS; taskIndex++) {
            if (taskIndex < dosingSchedule.size()) {
                EEPROM.put(eepromAddress, *(dosingSchedule.getTaskAtIndex(taskIndex)));

            } else {
                for (uint32_t j = eepromAddress; j < eepromAddress + sizeof(DosingTask); j++) {
                    EEPROM.update(j, 0);  // erase unused tasks space
                }
            }
            eepromAddress += sizeof(DosingTask);
        }

        saveEeepromCrc(calculateEepromCrc());
    }

    DosingSchedule &readDosingPumpSchedule(uint8_t dosingPumpNumber, DosingSchedule &dosingSchedule) {
        //
        eepromAddress = STORAGE_DOSING_SCHEDULES_START_ADDRESS + (dosingPumpNumber - 1) * MAX_NUMBER_OF_DOSING_TASKS * sizeof(DosingTask);

        DosingTask *dosingTaskPointer;

        for (uint8_t taskIndex = 0; taskIndex < MAX_NUMBER_OF_DOSING_TASKS; taskIndex++) {
            dosingTaskPointer = new DosingTask();
            EEPROM.get(eepromAddress, (*dosingTaskPointer));
            dosingSchedule.addTask(dosingTaskPointer);

            eepromAddress += sizeof(DosingTask);
        }

        return dosingSchedule;
    }

    /* dosing pumps calibration */
    void saveDosingPumpCalibration(uint8_t dosingPumpNumber, uint16_t millisPerMilliLiter) {
        EEPROM.put(STORAGE_DOSING_CALIBRATIONS_START_ADDRESS + ((dosingPumpNumber - 1) * sizeof(millisPerMilliLiter)), millisPerMilliLiter);

        saveEeepromCrc(calculateEepromCrc());
    }

    uint16_t readDosingPumpCalibration(uint8_t dosingPumpNumber, uint16_t millisPerMilliLiter) {
        return EEPROM.get(STORAGE_DOSING_CALIBRATIONS_START_ADDRESS + ((dosingPumpNumber - 1) * sizeof(millisPerMilliLiter)), millisPerMilliLiter);
    }

    /* temperature control settings */
    void saveTemperatureControlSettings(TemperatureControlSettings &settings) {
        EEPROM.put(STORAGE_TEMPERATURE_CONTROL_START_ADDRESS, settings);

        saveEeepromCrc(calculateEepromCrc());
    }

    TemperatureControlSettings readTemperatureControlSettings(TemperatureControlSettings &settings) {
        return EEPROM.get(STORAGE_TEMPERATURE_CONTROL_START_ADDRESS, settings);
    }

    /* ato control settings */
    void saveAtoSettings(AtoSettings &settings) {
        EEPROM.put(STORAGE_ATO_CONTROL_START_ADDRESS, settings);

        saveEeepromCrc(calculateEepromCrc());
    }

    AtoSettings readAtoSettings(AtoSettings &settings) {
        return EEPROM.get(STORAGE_ATO_CONTROL_START_ADDRESS, settings);
    }
};

#endif