#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ENUMS_STATE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ENUMS_STATE_H_
#pragma once

enum class State : uint8_t {
    Active,
    Sleeping,
    Calibrating,
};

#endif