#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_ATO_SETTINGS_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ATO_STATION_ATO_SETTINGS_H_
#pragma once

/**
 * Two factors play role here:
 *  • max volume of daily evaporation (evaporationSpeed L/day)
 *  • min volume of water the dispenser can provide in amount of time (dispensingSpeed L/min)
 *
 *  Make sure min daily dispensed water volume is greater than max daily evaporated water volume
 */
struct AtoSettings {

    uint32_t minDispensingIntervalMs = 15ul * 60ul * 1000ul;    // 15 min * 60 sec * 1000 ms
    uint32_t maxDispensingDurationMs = 90ul * 1000ul;           // 90 sec * 1000 ms

    explicit AtoSettings() = default;

    AtoSettings(uint32_t minDispensingIntervalMs, uint32_t maxDispensingDurationMs) :
            minDispensingIntervalMs(minDispensingIntervalMs),
            maxDispensingDurationMs(maxDispensingDurationMs) {};
};

#endif