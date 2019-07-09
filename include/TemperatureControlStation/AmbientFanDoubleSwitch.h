#ifndef __AQUARIUM_CONTROLLER_AMBIENT_FAN_DOUBLE_SWITCH_H__
#define __AQUARIUM_CONTROLLER_AMBIENT_FAN_DOUBLE_SWITCH_H__

class AmbientFanDoubleSwitch {
private:
    bool isHumiditySwitchOn = false;
    bool isTemperatureSwitchOn = false;

public:

    void setHumiditySwitchOn() {
        isHumiditySwitchOn = true;
    }

    void setHumiditySwitchOff() {
        isHumiditySwitchOn = false;
    }

    void setTemperatureSwitchOn() {
        isTemperatureSwitchOn = true;
    }

    void setTemperatureSwitchOff() {
        isTemperatureSwitchOn = false;
    }

    bool isOn() {
        return (isHumiditySwitchOn || isTemperatureSwitchOn);
    }

    bool isOff() {
        return !isOn();
    }
};

#endif
