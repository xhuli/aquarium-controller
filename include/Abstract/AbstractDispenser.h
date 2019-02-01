#ifndef __ABSTRACT_DISPENSER_H__
#define __ABSTRACT_DISPENSER_H__

class AbstractDispenser {
protected:
    bool isDispensing = false;

public:
    virtual ~AbstractDispenser() = default;

    bool getIsDispensing() {
        return isDispensing;
    }

    bool getIsNotDispensing() {
        return !isDispensing;
    }

    virtual void setIntensity(uint16_t intensity) = 0;

    virtual void startDispensing() = 0;

    virtual void stopDispensing() = 0;

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;
};

#endif