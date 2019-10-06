#ifndef __AQUARIUM_CONTROLLER_INCLUDE_STATES_ATO_STATION_STATE_H__
#define __AQUARIUM_CONTROLLER_INCLUDE_STATES_ATO_STATION_STATE_H__
#pragma once

enum class AtoStationState : uint8_t {
    Sensing,            // 0
    Dispensing,         // 1
    Sleeping,           // 2
    Alarming,           // 3
    Invalid,            // 4
};

#endif