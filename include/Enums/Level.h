#ifndef __AQUARIUM_CONTROLLER_INCLUDE_ENUMS_LEVEL_H__
#define __AQUARIUM_CONTROLLER_INCLUDE_ENUMS_LEVEL_H__

#include <stdint.h>

enum class Level : uint8_t {
    Low,    // 0
    High,   // 1
    Unknown // 2
};

#endif