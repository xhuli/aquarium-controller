#ifndef __ABSTRACT_ATO_DISPENSER_H__
#define __ABSTRACT_ATO_DISPENSER_H__

class AbstractAtoDispenser {
protected:
    bool isDispensing = false;

public:
    virtual ~AbstractAtoDispenser() = default;

    bool getIsDispensing() {
        return isDispensing;
    }

    bool getIsNotDispensing() {
        return !isDispensing;
    }

    virtual void startDispensing() = 0;

    virtual void stopDispensing() = 0;

    virtual void setup() = 0;

    virtual void update(uint32_t currentMillis) = 0;
};

#endif