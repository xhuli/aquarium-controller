#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_I_SLEEPABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_I_SLEEPABLE_H_
#pragma once

class ISleepable {

public:

    virtual void startSleeping(uint32_t const &sleepMillis) = 0;

    virtual void stopSleeping() = 0;

};

#endif