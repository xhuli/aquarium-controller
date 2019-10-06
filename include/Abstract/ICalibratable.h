#ifndef _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_I_CALIBRATABLE_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_ABSTRACT_I_CALIBRATABLE_H_
#pragma once

class ICalibratable {
public:
    virtual void startCalibration() = 0;

    virtual void stopCalibration() = 0;
};

#endif