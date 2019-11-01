#define __TEST_MODE__

#include <assert.h>
#include <iostream>
#include <chrono>

#include <Common/LinkedHashMap.h>
#include <Common/RunnableFunction.h>
#include <Common/FunctionList.h>
#include <Common/Sensor.h>
#include <Common/Switchable.h>
#include <Enums/State.h>
#include <AmbientStation/AmbientStation.h>
#include "../_Mocks/MockBuzzer.h"

static void setup() {
    AbstractRunnable::setupAll();
}

static void loop() {
    AbstractRunnable::loopAll();
}

static void loop(uint32_t forwardMs) {
    for (uint32_t ms = 0; ms < forwardMs; ++ms) {
        loop();
    }
}

static AmbientSettings ambientSettings{
        24.2,
        25.4,
        42,
        32,
        66,
        true,
        true
};

static LinkedHashMap<AlarmSeverity, AlarmNotifyConfiguration> alarmNotifyConfigurations{};

MockBuzzer buzzer{};
AlarmStation alarmStation{buzzer, alarmNotifyConfigurations};

AmbientStation ambientStation{};

Switchable ambientFan{};
Switchable systemFan{};
Switchable waterCooler{};
Switchable waterHeater{};

Sensor<float> ambientHumiditySensor{-100.0f};
Sensor<float> ambientTemperatureSensor{-100.0f};
Sensor<float> systemTemperatureSensor{-100.0f};
Sensor<float> waterTemperatureSensor{-100.0f};

//RunnableFunction ambientCoolingRules{[]() -> void {
//    /* what to do while seeping */
//    if (ambientStation.isInState(State::Sleeping)) {
//        ambientFan.setState(Switched::Off);
//        alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxHumidityReached);
//        alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxTemperatureReached);
//        return;
//    }
//
//    /* what to depending on sensors readings */
//    if (ambientHumiditySensor.getReading() >= ambientSettings.startAmbientFanAtHumidity ||
//        ambientTemperatureSensor.getReading() >= ambientSettings.startAmbientFanAtTemperature) {
//        //
//        ambientFan.setState(Switched::On);
//    } else {
//        ambientFan.setState(Switched::Off);
//    }
//
//    /* what to do with alarms */
//    if (ambientHumiditySensor.getReading() > ambientSettings.ambientMaxHumidityAlarmTrigger) {
//        alarmStation.alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Minor);
//    } else {
//        alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxHumidityReached);
//    }
//
//    if (ambientTemperatureSensor.getReading() > ambientSettings.ambientMaxTemperatureAlarmTrigger) {
//        alarmStation.alarmList.add(AlarmCode::AmbientMaxTemperatureReached, AlarmSeverity::Minor);
//    } else {
//        alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxTemperatureReached);
//    }
//}};
//
//RunnableFunction systemCoolingRules{[]() -> void {
//    /* what to do while seeping */
//    if (ambientStation.isInState(State::Sleeping)) {
//        systemFan.setState(Switched::Off);
//        alarmStation.alarmList.acknowledge(AlarmCode::SystemMaxTemperatureReached);
//        return;
//    }
//
//    /* what to depending on sensors readings */
//    if (systemTemperatureSensor.getReading() >= ambientSettings.startSystemFanAtTemperature) {
//        systemFan.setState(Switched::On);
//    } else {
//        systemFan.setState(Switched::Off);
//    }
//
//    /* what to do with alarms */
//    if (systemTemperatureSensor.getReading() > ambientSettings.systemMaxTemperatureAlarmTrigger) {
//        alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Minor);
//    } else {
//        alarmStation.alarmList.acknowledge(AlarmCode::SystemMaxTemperatureReached);
//    }
//}};
//
//RunnableFunction waterCoolingRules{[]() -> void {
//    /* what to do while seeping */
//    if (ambientStation.isInState(State::Sleeping)) {
//        waterCooler.setState(Switched::Off);
//        alarmStation.alarmList.acknowledge(AlarmCode::WaterMaxTemperatureReached);
//        return;
//    }
//
//    /* what to depending on sensors readings */
//    if (waterTemperatureSensor.getReading() >= ambientSettings.startWaterCoolingAtTemperature) {
//        waterCooler.setState(Switched::On);
//    } else {
//        waterCooler.setState(Switched::Off);
//    }
//
//    /* what to do with alarms */
//    if (waterTemperatureSensor.getReading() > ambientSettings.waterMaxTemperatureAlarmTrigger) {
//        alarmStation.alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Major);
//    } else {
//        alarmStation.alarmList.acknowledge(AlarmCode::WaterMaxTemperatureReached);
//    }
//}};
//
//RunnableFunction waterHeatingRules{[]() -> void {
//    /* what to do while seeping */
//    if (ambientStation.isInState(State::Sleeping)) {
//        waterHeater.setState(Switched::Off);
//        alarmStation.alarmList.acknowledge(AlarmCode::WaterMinTemperatureReached);
//        return;
//    }
//
//    /* what to depending on sensors readings */
//    if (waterTemperatureSensor.getReading() < ambientSettings.stopWaterHeatingAtTemperature) {
//        waterHeater.setState(Switched::On);
//    } else {
//        waterHeater.setState(Switched::Off);
//    }
//
//    /* what to do with alarms */
//    if (waterTemperatureSensor.getReading() < ambientSettings.waterMinTemperatureAlarmTrigger &&
//        waterTemperatureSensor.getReading() > -99.0f) {
//        //
//        alarmStation.alarmList.add(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Major);
//    } else {
//        alarmStation.alarmList.acknowledge(AlarmCode::WaterMinTemperatureReached);
//    }
//}};


static void shouldSwitchOnWaterHeaterOnWaterTemperatureBelowHeaterTurnOffTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.stopWaterHeatingAtTemperature - 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(waterHeater.isInState(Switched::On));

    std::cout << "ok -> shouldSwitchOnWaterHeaterOnWaterTemperatureBelowHeaterTurnOffTemperature\n";
}

static void shouldSwitchOffWaterHeaterOnWaterTemperatureAboveHeaterTurnOffTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.stopWaterHeatingAtTemperature + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(waterHeater.isInState(Switched::Off));

    std::cout << "ok -> shouldSwitchOffWaterHeaterOnWaterTemperatureAboveHeaterTurnOffTemperature\n";
}

static void shouldSwitchOffWaterCoolerOnWaterTemperatureBelowCoolerTurnOnTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.startWaterCoolingAtTemperature - 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(waterCooler.isInState(Switched::Off));

    std::cout << "ok -> shouldSwitchOffWaterCoolerOnWaterTemperatureBelowCoolerTurnOnTemperature\n";
}

static void shouldSwitchOnWaterCoolerOnWaterTemperatureAboveCoolerTurnOnTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.startWaterCoolingAtTemperature + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(waterCooler.isInState(Switched::On));

    std::cout << "ok -> shouldSwitchOnWaterCoolerOnWaterTemperatureAboveCoolerTurnOnTemperature\n";
}

static void shouldSwitchOffSystemFanOnSystemTemperatureBelowSystemTurnOnTemperature() {
    /* when */
    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature - 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(systemFan.isInState(Switched::Off));

    std::cout << "ok -> shouldSwitchOffSystemFanOnSystemTemperatureBelowSystemTurnOnTemperature\n";
}

static void shouldSwitchOnSystemFanOnSystemTemperatureAboveSystemTurnOnTemperature() {
    /* when */
    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(systemFan.isInState(Switched::On));

    std::cout << "ok -> shouldSwitchOnSystemFanOnSystemTemperatureAboveSystemTurnOnTemperature\n";
}

static void shouldSwitchOffAmbientFanOnAmbientTemperatureBelowAmbientTurnOnTemperature() {
    /* when */
    ambientTemperatureSensor.setReading(ambientSettings.startAmbientFanAtTemperature - 0.001f);
    ambientHumiditySensor.setReading(ambientSettings.startAmbientFanAtHumidity - 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(ambientFan.isInState(Switched::Off));

    std::cout << "ok -> shouldSwitchOffAmbientFanOnAmbientTemperatureBelowAmbientTurnOnTemperature\n";
}

static void shouldSwitchOnAmbientFanOnAmbientTemperatureAboveAmbientTurnOnTemperature() {
    /* when */
    ambientTemperatureSensor.setReading(ambientSettings.startAmbientFanAtTemperature + 0.001f);
    ambientHumiditySensor.setReading(ambientSettings.startAmbientFanAtHumidity);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(ambientFan.isInState(Switched::On));

    std::cout << "ok -> shouldSwitchOnAmbientFanOnAmbientTemperatureAboveAmbientTurnOnTemperature\n";
}

static void shouldSwitchOnAmbientFanOnAmbientHumidityAboveAmbientTurnOnHumidity() {
    /* when */
    ambientTemperatureSensor.setReading(ambientSettings.startAmbientFanAtTemperature);
    ambientHumiditySensor.setReading(ambientSettings.startAmbientFanAtHumidity + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(ambientFan.isInState(Switched::On));

    std::cout << "ok -> shouldSwitchOnAmbientFanOnAmbientHumidityAboveAmbientTurnOnHumidity\n";
}

static void shouldRaiseAlarmOnWaterTemperatureBelowWaterMinAlarmTriggerTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.waterMinTemperatureAlarmTrigger - 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::WaterMinTemperatureReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::WaterMinTemperatureReached));

    std::cout << "ok -> shouldRaiseAlarmOnWaterTemperatureBelowWaterMinAlarmTriggerTemperature\n";
}

static void shouldRaiseAlarmOnWaterTemperatureAboveWaterMaxAlarmTriggerTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.waterMaxTemperatureAlarmTrigger + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));

    std::cout << "ok -> shouldRaiseAlarmOnWaterTemperatureAboveWaterMaxAlarmTriggerTemperature\n";
}

static void shouldRaiseAlarmOnSystemTemperatureAboveSystemMaxAlarmTriggerTemperature() {
    /* when */
    systemTemperatureSensor.setReading(ambientSettings.systemMaxTemperatureAlarmTrigger + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::SystemMaxTemperatureReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::SystemMaxTemperatureReached));

    std::cout << "ok -> shouldRaiseAlarmOnSystemTemperatureAboveSystemMaxAlarmTriggerTemperature\n";
}

static void shouldRaiseAlarmOnAmbientTemperatureAboveAmbientMaxAlarmTriggerTemperature() {
    /* when */
    ambientTemperatureSensor.setReading(ambientSettings.ambientMaxTemperatureAlarmTrigger + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::AmbientMaxTemperatureReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AmbientMaxTemperatureReached));

    std::cout << "ok -> shouldRaiseAlarmOnAmbientTemperatureAboveAmbientMaxAlarmTriggerTemperature\n";
}

static void shouldRaiseAlarmOnAmbientHumidityAboveAmbientMaxAlarmTriggerHumidity() {
    /* when */
    ambientHumiditySensor.setReading(ambientSettings.ambientMaxHumidityAlarmTrigger + 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    std::cout << "ok -> shouldRaiseAlarmOnAmbientHumidityAboveAmbientMaxAlarmTriggerHumidity\n";
}

static void shouldAcknowledgeAlarmOnWaterTemperatureAboveOrEqualToWaterMinAlarmTriggerTemperature() {
    /* when */
    waterTemperatureSensor.setReading(ambientSettings.waterMinTemperatureAlarmTrigger - 0.001f);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::WaterMinTemperatureReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::WaterMinTemperatureReached));

    std::cout << "ok -> shouldAcknowledgeAlarmOnWaterTemperatureAboveOrEqualToWaterMinAlarmTriggerTemperature\n";
}

static void shouldAcknowledgeAlarmOnWaterTemperatureBelowOrEqualToWaterMaxAlarmTriggerTemperature() {
    /* given */
    waterTemperatureSensor.setReading(ambientSettings.waterMaxTemperatureAlarmTrigger + 0.001f);
    loop();
    assert(alarmStation.alarmList.contains(AlarmCode::WaterMaxTemperatureReached));

    /* when */
    waterTemperatureSensor.setReading(ambientSettings.waterMaxTemperatureAlarmTrigger);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::WaterMaxTemperatureReached));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::WaterMaxTemperatureReached));

    std::cout << "ok -> shouldAcknowledgeAlarmOnWaterTemperatureBelowOrEqualToWaterMaxAlarmTriggerTemperature\n";
}

static void shouldAcknowledgeAlarmOnSystemTemperatureBelowOrEqualToSystemMaxAlarmTriggerTemperature() {
    /* given */
    systemTemperatureSensor.setReading(ambientSettings.systemMaxTemperatureAlarmTrigger + 0.001f);
    loop();
    assert(alarmStation.alarmList.contains(AlarmCode::SystemMaxTemperatureReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::SystemMaxTemperatureReached));

    /* when */
    systemTemperatureSensor.setReading(ambientSettings.systemMaxTemperatureAlarmTrigger);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::SystemMaxTemperatureReached));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::SystemMaxTemperatureReached));

    std::cout << "ok -> shouldAcknowledgeAlarmOnSystemTemperatureBelowOrEqualToSystemMaxAlarmTriggerTemperature\n";
}

static void shouldAcknowledgeAlarmOnAmbientTemperatureBelowOrEqualToAmbientMaxAlarmTriggerTemperature() {
    /* given */
    ambientTemperatureSensor.setReading(ambientSettings.ambientMaxTemperatureAlarmTrigger + 0.001f);
    loop();
    assert(alarmStation.alarmList.contains(AlarmCode::AmbientMaxTemperatureReached));

    /* when */
    ambientTemperatureSensor.setReading(ambientSettings.ambientMaxTemperatureAlarmTrigger);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::AmbientMaxTemperatureReached));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::AmbientMaxTemperatureReached));

    std::cout << "ok -> shouldAcknowledgeAlarmOnAmbientTemperatureBelowOrEqualToAmbientMaxAlarmTriggerTemperature\n";
}

static void shouldAcknowledgeAlarmOnAmbientHumidityBelowOrEqualToAmbientMaxAlarmTriggerHumidity() {
    /* given */
    ambientHumiditySensor.setReading(ambientSettings.ambientMaxHumidityAlarmTrigger + 0.001f);
    loop();
    assert(alarmStation.alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(!alarmStation.alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    /* when */
    ambientHumiditySensor.setReading(ambientSettings.ambientMaxHumidityAlarmTrigger);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));
    assert(alarmStation.alarmList.contains(AlarmCode::AmbientMaxHumidityReached));
    assert(alarmStation.alarmList.isAcknowledged(AlarmCode::AmbientMaxHumidityReached));

    std::cout << "ok -> shouldAcknowledgeAlarmOnAmbientHumidityBelowOrEqualToAmbientMaxAlarmTriggerHumidity\n";
}

static void shouldGoToStateSleepingOnStartSleeping() {
    /* when */
    ambientStation.startSleeping(3000);
    loop();

    /* then */
    assert(ambientStation.isInState(State::Sleeping));

    std::cout << "ok -> shouldGoToStateSleepingOnStartSleeping\n";
}

static void shouldGoToStateActiveAfterSleepPeriod() {
    /* given */
    uint32_t sleepMs = 3;
    ambientStation.startSleeping(sleepMs);

    /* when */
    loop(sleepMs);
    assert(ambientStation.isInState(State::Sleeping));
    loop();

    /* then */
    assert(ambientStation.isInState(State::Active));

    std::cout << "ok -> shouldGoToStateSleepingOnStartSleeping\n";
}

static void shouldExecuteSwitchRuleToSwitchDeviceOn() {
    /* when */
    systemTemperatureSensor.setReading(-100.0f);
    loop();
    assert(systemFan.isInState(Switched::Off));

    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature + 0.001f);
    loop();

    /* then */
    assert(systemFan.isInState(Switched::On));

    std::cout << "ok -> shouldExecuteSwitchRuleToSwitchDeviceOn\n";
}

static void shouldExecuteSwitchRuleToSwitchDeviceOff() {
    /* when */
    systemTemperatureSensor.setReading(-100.0f);
    loop();
    assert(systemFan.isInState(Switched::Off));

    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature + 0.001f);
    loop();
    assert(systemFan.isInState(Switched::On));

    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature - 0.001f);
    loop();

    /* then */
    assert(systemFan.isInState(Switched::Off));

    std::cout << "ok -> shouldExecuteSwitchRuleToSwitchDeviceOff\n";
}

static void shouldExecuteSwitchRuleWithUpdatedSettingsToSwitchDeviceOn() {
    /* when */
    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature - 0.001f);
    loop();
    assert(systemFan.isInState(Switched::Off));
    ambientSettings.startSystemFanAtTemperature -= 1;
    loop();

    /* then */
    assert(systemFan.isInState(Switched::On));

    std::cout << "ok -> shouldExecuteSwitchRuleWithUpdatedSettingsToSwitchDeviceOn\n";
}

static void shouldExecuteSwitchRuleWithUpdatedSettingsToSwitchDeviceOff() {
    /* when */
    systemTemperatureSensor.setReading(ambientSettings.startSystemFanAtTemperature + 0.001f);
    loop();
    assert(systemFan.isInState(Switched::On));
    ambientSettings.startSystemFanAtTemperature += 1;
    loop();

    /* then */
    assert(systemFan.isInState(Switched::Off));

    std::cout << "ok -> shouldExecuteSwitchRuleWithUpdatedSettingsToSwitchDeviceOff\n";
}


int main() {

    ambientStation.rules.add([]() -> void {
        if (waterTemperatureSensor.getReading() < ambientSettings.stopWaterHeatingAtTemperature) {
            waterHeater.setState(Switched::On);
        } else {
            waterHeater.setState(Switched::Off);
        }
    });

    ambientStation.rules.add([]() -> void {
        if ((waterTemperatureSensor.getReading() < ambientSettings.waterMinTemperatureAlarmTrigger) && waterTemperatureSensor.getReading() > -99.0f) {
            alarmStation.alarmList.add(AlarmCode::WaterMinTemperatureReached, AlarmSeverity::Major);
        } else {
            alarmStation.alarmList.acknowledge(AlarmCode::WaterMinTemperatureReached);
        }
    });

    ambientStation.rules.add([]() -> void {
        if (waterTemperatureSensor.getReading() >= ambientSettings.startWaterCoolingAtTemperature) {
            waterCooler.setState(Switched::On);
        } else {
            waterCooler.setState(Switched::Off);
        }
    });

    ambientStation.rules.add([]() -> void {
        if (waterTemperatureSensor.getReading() > ambientSettings.waterMaxTemperatureAlarmTrigger) {
            alarmStation.alarmList.add(AlarmCode::WaterMaxTemperatureReached, AlarmSeverity::Major);
        } else {
            alarmStation.alarmList.acknowledge(AlarmCode::WaterMaxTemperatureReached);
        }
    });

    ambientStation.rules.add([]() -> void {
        if (systemTemperatureSensor.getReading() >= ambientSettings.startSystemFanAtTemperature) {
            systemFan.setState(Switched::On);
        } else {
            systemFan.setState(Switched::Off);
        }
    });

    ambientStation.rules.add([]() -> void {
        if (systemTemperatureSensor.getReading() > ambientSettings.systemMaxTemperatureAlarmTrigger) {
            alarmStation.alarmList.add(AlarmCode::SystemMaxTemperatureReached, AlarmSeverity::Minor);
        } else {
            alarmStation.alarmList.acknowledge(AlarmCode::SystemMaxTemperatureReached);
        }
    });

    ambientStation.rules.add([]() -> void {
        if (ambientHumiditySensor.getReading() >= ambientSettings.startAmbientFanAtHumidity ||
            ambientTemperatureSensor.getReading() >= ambientSettings.startAmbientFanAtTemperature) {
            //
            ambientFan.setState(Switched::On);
        } else {
            ambientFan.setState(Switched::Off);
        }
    });

    ambientStation.rules.add([]() -> void {
        if (ambientHumiditySensor.getReading() > ambientSettings.ambientMaxHumidityAlarmTrigger) {
            alarmStation.alarmList.add(AlarmCode::AmbientMaxHumidityReached, AlarmSeverity::Minor);
        } else {
            alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxHumidityReached);
        }

        if (ambientTemperatureSensor.getReading() > ambientSettings.ambientMaxTemperatureAlarmTrigger) {
            alarmStation.alarmList.add(AlarmCode::AmbientMaxTemperatureReached, AlarmSeverity::Minor);
        } else {
            alarmStation.alarmList.acknowledge(AlarmCode::AmbientMaxTemperatureReached);
        }
    });

    alarmNotifyConfigurations.put(AlarmSeverity::Critical, AlarmNotifyConfiguration(1, 7000));
    alarmNotifyConfigurations.put(AlarmSeverity::Major, AlarmNotifyConfiguration(5, 5000));
    alarmNotifyConfigurations.put(AlarmSeverity::Minor, AlarmNotifyConfiguration(15, 3000));
    alarmNotifyConfigurations.put(AlarmSeverity::NoSeverity, AlarmNotifyConfiguration(60, 1000));

    std::cout << "\n"
              << "------------------------------------------------------------" << "\n"
              << " >> TEST START" << "\n"
              << "------------------------------------------------------------" << "\n";

    auto start = std::chrono::high_resolution_clock::now();

    const int repeat = 1;

    for (int i = 0; i < repeat; ++i) {

        shouldSwitchOnWaterHeaterOnWaterTemperatureBelowHeaterTurnOffTemperature();
        shouldSwitchOffWaterHeaterOnWaterTemperatureAboveHeaterTurnOffTemperature();

        shouldSwitchOffWaterCoolerOnWaterTemperatureBelowCoolerTurnOnTemperature();
        shouldSwitchOnWaterCoolerOnWaterTemperatureAboveCoolerTurnOnTemperature();

        shouldSwitchOffSystemFanOnSystemTemperatureBelowSystemTurnOnTemperature();
        shouldSwitchOnSystemFanOnSystemTemperatureAboveSystemTurnOnTemperature();

        shouldSwitchOffAmbientFanOnAmbientTemperatureBelowAmbientTurnOnTemperature();
        shouldSwitchOnAmbientFanOnAmbientTemperatureAboveAmbientTurnOnTemperature();
        shouldSwitchOnAmbientFanOnAmbientHumidityAboveAmbientTurnOnHumidity();

        shouldRaiseAlarmOnWaterTemperatureBelowWaterMinAlarmTriggerTemperature();
        shouldRaiseAlarmOnWaterTemperatureAboveWaterMaxAlarmTriggerTemperature();
        shouldRaiseAlarmOnSystemTemperatureAboveSystemMaxAlarmTriggerTemperature();
        shouldRaiseAlarmOnAmbientTemperatureAboveAmbientMaxAlarmTriggerTemperature();
        shouldRaiseAlarmOnAmbientHumidityAboveAmbientMaxAlarmTriggerHumidity();

        shouldAcknowledgeAlarmOnWaterTemperatureAboveOrEqualToWaterMinAlarmTriggerTemperature();
        shouldAcknowledgeAlarmOnWaterTemperatureBelowOrEqualToWaterMaxAlarmTriggerTemperature();
        shouldAcknowledgeAlarmOnSystemTemperatureBelowOrEqualToSystemMaxAlarmTriggerTemperature();
        shouldAcknowledgeAlarmOnAmbientTemperatureBelowOrEqualToAmbientMaxAlarmTriggerTemperature();
        shouldAcknowledgeAlarmOnAmbientHumidityBelowOrEqualToAmbientMaxAlarmTriggerHumidity();

        shouldGoToStateSleepingOnStartSleeping();
        shouldGoToStateActiveAfterSleepPeriod();

        shouldExecuteSwitchRuleToSwitchDeviceOn();
        shouldExecuteSwitchRuleToSwitchDeviceOff();
        shouldExecuteSwitchRuleWithUpdatedSettingsToSwitchDeviceOn();
        shouldExecuteSwitchRuleWithUpdatedSettingsToSwitchDeviceOff();

        if (repeat > 1) {
            std::cout << "------------------------------------------------------------\n";
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    std::cout << "\n"
                 "------------------------------------------------------------" << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    std::cout << "------------------------------------------------------------" << "\n"
              << " >> TEST END" << "\n"
              << "------------------------------------------------------------" << "\n"
              << "\n";

    return 0;
}