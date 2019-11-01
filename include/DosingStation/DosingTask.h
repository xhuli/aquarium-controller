#ifndef _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_TASK_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_TASK_H_
#pragma once

struct DosingTask {

    static constexpr uint8_t EveryMinute = 60;
    static constexpr uint8_t EveryHour = 24;

    DayOfWeek weekDay;
    uint8_t hour;
    uint8_t minute;
    uint8_t doseMilliLiters;
    uint8_t doseMilliLiterQuarters;

    DosingTask(
            DayOfWeek weekDay,              // weekday (1-7,..?), Sunday is day 1
            uint8_t hour,                   // hour (0-23), 24 is every hour
            uint8_t minute,                 // minute (0-59), 60 is every minute
            uint8_t doseMilliLiters,        // mL (0-255)
            uint8_t doseMilliLiterQuarters  // mL/4 (0-3)
    ) :
            weekDay(weekDay),
            hour(hour),
            minute(minute),
            doseMilliLiters(doseMilliLiters),
            doseMilliLiterQuarters(doseMilliLiterQuarters) {}

    static bool isValidMinute(uint8_t const minute) {
        return minute <= DosingTask::EveryMinute;
    }

    static bool isValidHour(uint8_t const hour) {
        return hour <= DosingTask::EveryHour;
    }

    static bool isValidDose(uint8_t doseMilliLiters, uint8_t doseMilliLiterQuarters) {
        return (doseMilliLiters != 0 && doseMilliLiterQuarters != 0 && doseMilliLiterQuarters <= 3);
    }

    bool isScheduledAt(DayOfWeek dayOfWeek, uint8_t hour, uint8_t minute) const {
        return DosingTask::matchesMinute(minute) &&
               DosingTask::matchesHour(hour) &&
               DosingTask::matchesDay(dayOfWeek);
    }

    bool matchesMinute(uint8_t const minute) const {
        return DosingTask::minute == minute || DosingTask::minute == DosingTask::EveryMinute;
    }

    bool matchesHour(uint8_t const hour) const {
        return DosingTask::hour == hour || DosingTask::hour == DosingTask::EveryHour;
    }

    bool matchesDay(DayOfWeek weekDay) const {
        if (DosingTask::weekDay == DayOfWeek::EveryDay) { return true; }
        if (DosingTask::weekDay == weekDay) { return true; }

        switch (DosingTask::weekDay) {
            case DayOfWeek::MoTuWeThFr:
            case DayOfWeek::MondayToFriday:
                switch (weekDay) {
                    case DayOfWeek::Monday:
                    case DayOfWeek::Tuesday:
                    case DayOfWeek::Wednesday:
                    case DayOfWeek::Thursday:
                    case DayOfWeek::Friday:
                        return true;
                    default:
                        return false;
                }

            case DayOfWeek::SaSu:
            case DayOfWeek::SaturdayAndSunday:
                switch (weekDay) {
                    case DayOfWeek::Saturday:
                    case DayOfWeek::Sunday:
                        return true;
                    default:
                        return false;
                }

            case DayOfWeek::MoWeFrSu:
                switch (weekDay) {
                    case DayOfWeek::Monday:
                    case DayOfWeek::Wednesday:
                    case DayOfWeek::Friday:
                    case DayOfWeek::Sunday:
                        return true;
                    default:
                        return false;
                }

            case DayOfWeek::TuThSa:
                switch (weekDay) {
                    case DayOfWeek::Tuesday:
                    case DayOfWeek::Thursday:
                    case DayOfWeek::Saturday:
                        return true;
                    default:
                        return false;
                }

            default:
                return false;
        }
    }
};

#endif