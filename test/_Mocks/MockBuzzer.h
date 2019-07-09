#ifndef __AQUARIUM_CONTROLLER_MOCK_BUZZER_H__
#define __AQUARIUM_CONTROLLER_MOCK_BUZZER_H__

#include <Abstract/AbstractAlarmListener.h>

class MockBuzzer : public AbstractAlarmListener {
private:
    bool isBuzzing = false;
public:
    ~MockBuzzer() override = default;

    bool getIsBuzzing() {
        return isBuzzing;
    }

    bool getIsNotBuzzing() {
        return !isBuzzing;
    }

    void mute() {
        isBuzzing = false;
    }

    void notifyOnAlarm(Alarm *alarm) override {
        std::cout << "NOTIFICATION RECEIVED: "
                  << static_cast<uint16_t >(alarm->code)
                  << " (" << ((alarm->isCritical) ? "Critical" : "Major") << ")" << "\n";
        isBuzzing = true;
    }
};

#endif
