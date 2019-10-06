#ifndef __AQUARIUM_CONTROLLER_INCLUDE_STATES_LIQUID_LEVEL_STATE_H__
#define __AQUARIUM_CONTROLLER_INCLUDE_STATES_LIQUID_LEVEL_STATE_H__

enum class LiquidLevelState : uint8_t {
    Low,    // 0
    High,   // 1
    Unknown // 2
};

#endif