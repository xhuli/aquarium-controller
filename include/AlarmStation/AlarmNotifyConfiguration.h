#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_NOTIFY_CONFIGURATION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ALARM_STATION_ALARM_NOTIFY_CONFIGURATION_H_
#pragma once

/**
 * Object representing alarm notify configuration.
 *
 * @param soundPeriodMinutes – (0 – 255) alarm sound period in minutes
 * @param soundDurationMs – (0 – 65,535) alarm sound duration in milliseconds
 */
class AlarmNotifyConfiguration {

private:

    uint8_t soundPeriodMinutes;
    uint16_t soundDurationMs;

public:

    /**
     * Object representing alarm notify configuration.
     *
     * @param soundPeriodMinutes – (0 – 255) alarm sound period in minutes
     * @param soundDurationMs – (0 – 65,535) alarm sound duration in milliseconds
     */
    explicit AlarmNotifyConfiguration(
            uint8_t const soundPeriodMinutes,
            uint16_t const soundDurationMs
    ) :
            soundPeriodMinutes(soundPeriodMinutes),
            soundDurationMs(soundDurationMs) {}

    uint8_t getSoundPeriodMinutes() const {
        return soundPeriodMinutes;
    }

    uint16_t getSoundDurationMs() const {
        return soundDurationMs;
    }
};

#endif