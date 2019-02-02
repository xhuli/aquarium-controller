// Test_SaveAndReadToFromStorage.cpp

// #define __DS_DEBUG__

#include <Arduino.h>
#include <Streaming.h>  // http://arduiniana.org/libraries/streaming/
#include <Time.h>       // standard Arduino time library
#include <Wire.h>       // standard Arduino i2c library
#include <stdint.h>     // integer definitions: int8_t, int16_t, ..., uint8_t, ...
#include "DosingStation/DosingSchedule.h"
#include "DosingStation/DosingTask.h"
#include "Storage/Storage.h"
#include "TemperatureControlStation/TemperatureControlSettings.h"

const long SERIAL_SPEED = 9600;

TemperatureControlSettings settings;             // holds defult data
TemperatureControlSettings settingsFromStorage;  // holds defult data
Storage storage = Storage();
DosingSchedule dosingSchedule = DosingSchedule();
String userInput;
bool runTest = false;

uint32_t currentMillis;
uint32_t previousMillis = 0;
uint8_t loopCount = 0;

void printTemperatureControlSettings(TemperatureControlSettings tcs) {
    Serial << "------------------------------------------------" << endl;
    Serial << "stopHeatingTemperatureCelsius: " << tcs.stopHeatingTemperatureCelsius << endl;
    Serial << "startCoolingTemperatureCelsius: " << tcs.startCoolingTemperatureCelsius << endl;
    Serial << endl;
    Serial << "triggerAlarmMinAquariumTemperatureCelsius: " << tcs.minAquariumTemperatureCelsiusAlarmTrigger << endl;
    Serial << "triggerAlarmMaxAquariumTemperatureCelsius: " << tcs.maxAquariumTemperatureCelsiusAlarmTrigger << endl;
    Serial << "triggerAlarmMaxSystemTemperatureCelsius: " << tcs.maxSystemTemperatureCelsiusAlarmTrigger << endl;
    Serial << "triggerAlarmMaxSystemHumidityPercent: " << tcs.maxSystemHumidityPercentAlarmTrigger << endl;
    Serial << endl;
    Serial << "isHeatingControlEnabled: " << tcs.isHeatingControlEnabled << endl;
    Serial << "isCoolingControlEnabled: " << tcs.isCoolingControlEnabled << endl;
    Serial << endl;
    Serial << "isMinAquariumTemperatureAlarmEnabled: " << tcs.isMinAquariumTemperatureAlarmEnabled << endl;
    Serial << "isMaxAquariumTemperatureAlarmEnabled: " << tcs.isMaxAquariumTemperatureAlarmEnabled << endl;
    Serial << "isMaxSystemTemperatureAlarmEnabled: " << tcs.isMaxSystemTemperatureAlarmEnabled << endl;
    Serial << "isMaxSystemHumidityAlarmEnabled: " << tcs.isMaxSystemHumidityAlarmEnabled << endl;
    Serial << endl;
    Serial << "------------------------------------------------" << endl;
}

void setup() {
    Serial.begin(SERIAL_SPEED);
    while (!Serial) {
        // wait until Arduino Serial Monitor opens
    }
    delay(100);

    Serial << endl;
    Serial << endl;
    Serial << "============" << endl;
    Serial << " Init Done!" << endl;
    Serial << "============" << endl;
    Serial << endl;
    Serial << endl;

    DosingTask* storeTaskPointer;
    // DosingTask* readTaskPointer;

    Serial << "===========================" << endl;
    Serial << " Create Tasks for Schedule" << endl;
    Serial << "===========================" << endl;
    for (uint8_t i = 1; i <= 3; i++) {
        storeTaskPointer = new DosingTask();
        storeTaskPointer->dayOfWeek = i % 8;
        storeTaskPointer->startHour = i;
        storeTaskPointer->startMinute = 30 + i;
        storeTaskPointer->doseMilliLiters = i;
        storeTaskPointer->doseMilliLitersFraction = i % 4;

        dosingSchedule.addTask(storeTaskPointer);

        // Serial << i << ": storeTaskPointer.dayOfWeek: " << storeTaskPointer->dayOfWeek << endl;
        // Serial << i << ": storeTaskPointer.startHour: " << storeTaskPointer->startHour << endl;
        // Serial << i << ": storeTaskPointer.startMinute: " << storeTaskPointer->startMinute << endl;
        // Serial << i << ": storeTaskPointer.doseMilliLiters: " << storeTaskPointer->doseMilliLiters << endl;
        // Serial << i << ": storeTaskPointer.doseMilliLitersFraction: " << storeTaskPointer->doseMilliLitersFraction << endl;
        // Serial << endl;
    }
    Serial << "=======" << endl;
    Serial << " DONE!" << endl;
    Serial << "=======" << endl;
    Serial << endl;

    // Serial << "===========================" << endl;
    // Serial << " Print Tasks from Schedule" << endl;
    // Serial << "===========================" << endl;
    Serial << "Schedule size: " << dosingSchedule.size() << endl;
    // Serial << endl;

    // for (uint8_t i = 0; i < dosingSchedule.size(); i++) {
    //     readTaskPointer = dosingSchedule.getTaskAtIndex(i);
    //     // Serial << i << ": readTaskPointer address: " << readTaskPointer << endl;

    //     Serial << i << ": readTaskPointer.dayOfWeek: " << readTaskPointer->dayOfWeek << endl;
    //     Serial << i << ": readTaskPointer.startHour: " << readTaskPointer->startHour << endl;
    //     Serial << i << ": readTaskPointer.startMinute: " << readTaskPointer->startMinute << endl;
    //     Serial << i << ": readTaskPointer.doseMilliLiters: " << readTaskPointer->doseMilliLiters << endl;
    //     Serial << i << ": readTaskPointer.doseMilliLitersFraction: " << readTaskPointer->doseMilliLitersFraction << endl;
    //     Serial << endl;
    // }
    // Serial << "=======" << endl;
    // Serial << " DONE!" << endl;
    // Serial << "=======" << endl;

    // Serial << endl;
    // Serial << endl;

    runTest = true;

    // Serial << "==============================" << endl;
    // Serial << " Read First Task from Storage" << endl;
    // Serial << "==============================" << endl;

    // DosingTask readFirstDosingTask = DosingTask();
    // EEPROM.get(224, readFirstDosingTask);
    // Serial << "readFirstDosingTask.dayOfWeek: " << readFirstDosingTask.dayOfWeek << endl;
    // Serial << "readFirstDosingTask.startHour: " << readFirstDosingTask.startHour << endl;
    // Serial << "readFirstDosingTask.startMinute: " << readFirstDosingTask.startMinute << endl;
    // Serial << "readFirstDosingTask.doseMilliLiters: " << readFirstDosingTask.doseMilliLiters << endl;
    // Serial << "readFirstDosingTask.doseMilliLitersFraction: " << readFirstDosingTask.doseMilliLitersFraction << endl;
    // Serial << endl;
    // Serial << "=======" << endl;
    // Serial << " DONE!" << endl;
    // Serial << "=======" << endl;

    // Serial << "===============================" << endl;
    // Serial << " Read Second Task from Storage" << endl;
    // Serial << "===============================" << endl;

    // EEPROM.get(227, readFirstDosingTask);
    // Serial << "readFirstDosingTask.dayOfWeek: " << readFirstDosingTask.dayOfWeek << endl;
    // Serial << "readFirstDosingTask.startHour: " << readFirstDosingTask.startHour << endl;
    // Serial << "readFirstDosingTask.startMinute: " << readFirstDosingTask.startMinute << endl;
    // Serial << "readFirstDosingTask.doseMilliLiters: " << readFirstDosingTask.doseMilliLiters << endl;
    // Serial << "readFirstDosingTask.doseMilliLitersFraction: " << readFirstDosingTask.doseMilliLitersFraction << endl;
    // Serial << endl;
    // Serial << "=======" << endl;
    // Serial << " DONE!" << endl;
    // Serial << "=======" << endl;

    // Serial << "==============================" << endl;
    // Serial << " Read 16-th Task from Storage" << endl;
    // Serial << "==============================" << endl;

    // EEPROM.get(269, readFirstDosingTask);
    // Serial << "readFirstDosingTask.dayOfWeek: " << readFirstDosingTask.dayOfWeek << endl;
    // Serial << "readFirstDosingTask.startHour: " << readFirstDosingTask.startHour << endl;
    // Serial << "readFirstDosingTask.startMinute: " << readFirstDosingTask.startMinute << endl;
    // Serial << "readFirstDosingTask.doseMilliLiters: " << readFirstDosingTask.doseMilliLiters << endl;
    // Serial << "readFirstDosingTask.doseMilliLitersFraction: " << readFirstDosingTask.doseMilliLitersFraction << endl;
    // Serial << endl;
    // Serial << "=======" << endl;
    // Serial << " DONE!" << endl;
    // Serial << "=======" << endl;
}

void loop() {
    //
    // Serial << endl;
    // Serial << "Run test? [y|n]: ";
    // userInput = Serial.readStringUntil('\n');
    // if (userInput.equals("y")) {
    //     runTest = true;
    // }

    if (runTest && (loopCount < 1)) {
        loopCount += 1;  // protection not to burn the eeprom by accident

        // /*
        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should print default temperature control settings ::" << endl;
        Serial << "Default temperature control settings" << endl;
        printTemperatureControlSettings(settings);
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;

        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should update temperature control settings in memory ::" << endl;
        Serial << "Update temperature controll settings object" << endl;
        settings.stopHeatingTemperatureCelsius = 80;
        settings.startCoolingTemperatureCelsius = 90;
        settings.isHeatingControlEnabled = true;
        settings.isCoolingControlEnabled = true;
        settings.triggerAlarmMinAquariumTemperatureCelsius = 13;
        settings.triggerAlarmMinAquariumTemperatureEnabled = false;
        settings.triggerAlarmMaxAquariumTemperatureCelsius = 100;
        settings.triggerAlarmMaxAquariumTemperatureEnabled = false;
        settings.triggerAlarmMaxSystemTemperatureCelsius = 10;
        settings.triggerAlarmMaxSystemTemperatureEnabled = false;
        settings.triggerAlarmMaxSystemHumidityPercent = 60;
        settings.triggerAlarmMaxSystemHumidityEnabled = false;
        printTemperatureControlSettings(settings);
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;

        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should chek id stored data in Storage is valid ::" << endl;
        if (storage.isStoredDataValid()) {
            Serial << "Stored Data is not Valid!" << endl;
        } else {
            Serial << "Valid Stored Data!" << endl;
        }
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;

        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should save and read temperature control settings to Storage ::" << endl;
        // Serial << "Write TemperatureControllSettings to Storage? [y|n]: " << endl;
        // userInput = Serial.readStringUntil("\n");
        // if (userInput.equals("y")) {
        Serial << "Started saving temperature controll settings to Storage" << endl;
        storage.saveTemperatureControlSettings(settings);
        Serial << "Done saving temperature controll settings to Storage!" << endl;
        Serial << "Started reading temperature controll settings from Storage" << endl;
        settingsFromStorage = storage.readTemperatureControlSettings(settingsFromStorage);
        Serial << "Settings read from Storage: " << endl;
        printTemperatureControlSettings(settingsFromStorage);

        Serial << (settings.stopHeatingTemperatureCelsius == settingsFromStorage.stopHeatingTemperatureCelsius) << endl;
        Serial << (settings.startCoolingTemperatureCelsius == settingsFromStorage.startCoolingTemperatureCelsius) << endl;
        Serial << (settings.minAquariumTemperatureCelsiusAlarmTrigger == settingsFromStorage.minAquariumTemperatureCelsiusAlarmTrigger) << endl;
        Serial << (settings.maxAquariumTemperatureCelsiusAlarmTrigger == settingsFromStorage.maxAquariumTemperatureCelsiusAlarmTrigger) << endl;
        Serial << (settings.maxSystemTemperatureCelsiusAlarmTrigger == settingsFromStorage.maxSystemTemperatureCelsiusAlarmTrigger) << endl;
        Serial << (settings.maxSystemHumidityPercentAlarmTrigger == settingsFromStorage.maxSystemHumidityPercentAlarmTrigger) << endl;
        Serial << (settings.isHeatingControlEnabled == settingsFromStorage.isHeatingControlEnabled) << endl;
        Serial << (settings.isCoolingControlEnabled == settingsFromStorage.isCoolingControlEnabled) << endl;
        Serial << (settings.isMinAquariumTemperatureAlarmEnabled == settingsFromStorage.isMinAquariumTemperatureAlarmEnabled) << endl;
        Serial << (settings.isMaxAquariumTemperatureAlarmEnabled == settingsFromStorage.isMaxAquariumTemperatureAlarmEnabled) << endl;
        Serial << (settings.isMaxSystemTemperatureAlarmEnabled == settingsFromStorage.isMaxSystemTemperatureAlarmEnabled) << endl;
        Serial << (settings.isMaxSystemHumidityAlarmEnabled == settingsFromStorage.isMaxSystemHumidityAlarmEnabled) << endl;
        Serial << "Done reading temperature controll settings from Storage" << endl;
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;
// */
        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should read valid crc from Storage ::" << endl;
        Serial << storage.isStoredDataCrcValid() << endl;
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;

        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should have valid version in Storage ::" << endl;
        Serial << storage.isStoredDataVersionCompatible() << endl;
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;
// */

        // /*
        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should store and read dosing calibration ::" << endl;
        Serial << "Started saving calirations to Storage" << endl;
        uint16_t millisPerMilliLiter = 1000;
        for (uint8_t dosingPumpNumber = 1; dosingPumpNumber <= 16; dosingPumpNumber++) {
            millisPerMilliLiter += dosingPumpNumber;
            storage.saveDosingPumpCalibration(dosingPumpNumber, millisPerMilliLiter);
        }
        Serial << "Done saving calibrations to Storage" << endl;
        uint16_t expectmillisPerMilliLiter = 1000;
        Serial << "Started reading calirations from Storage" << endl;
        for (uint8_t dosingPumpNumber = 1; dosingPumpNumber <= 16; dosingPumpNumber++) {
            expectMillisPerMilliLiter += dosingPumpNumber;
            Serial << (storage.readDosingPumpCalibration(dosingPumpNumber, millisPerMilliLiter) == expectMillisPerMilliLiter) << endl;
        }
        Serial << "Done reading calibrations from Storage" << endl;
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;
// */
        // /*
        // TEST <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        Serial << ":: TEST: should store and read dosing schedule ::" << endl;
        Serial << "Started saving schedules to Storage" << endl;
        Serial << "Schedule size: " << dosingSchedule.size() << endl;
        uint8_t numberOfPumpsToTests = 2;
        for (uint8_t dosingPumpNumber = 1; dosingPumpNumber <= numberOfPumpsToTests; dosingPumpNumber++) {
            storage.saveDosingPumpSchedule(dosingPumpNumber, dosingSchedule);
        }
        Serial << "Done saving schedules to Storage" << endl;
        
        Serial << "Started reading schedules from Storage" << endl;
        DosingSchedule readDosingSchedule;
        DosingTask* savedTaskPointer;
        DosingTask* readTaskPointer;

        for (uint8_t dosingPumpNumber = 1; dosingPumpNumber <= numberOfPumpsToTests; dosingPumpNumber++) {
            readDosingSchedule = storage.readDosingPumpSchedule(dosingPumpNumber, readDosingSchedule);
            Serial << "Read Schedule size: " << readDosingSchedule.size() << endl;
            for (uint8_t dosingTaskIndex = 0; dosingTaskIndex < readDosingSchedule.size(); dosingTaskIndex++) {
                savedTaskPointer = dosingSchedule.getTaskAtIndex(dosingTaskIndex);
                readTaskPointer = readDosingSchedule.getTaskAtIndex(dosingTaskIndex);

                Serial << "pumpNumber: " << dosingPumpNumber << "; taskIndex: " << dosingTaskIndex << "    dayOfWeek: " << readTaskPointer->dayOfWeek << endl;
                Serial << "pumpNumber: " << dosingPumpNumber << "; taskIndex: " << dosingTaskIndex << "    doseMilliLiters: " << readTaskPointer->doseMilliLiters << endl;
                Serial << "pumpNumber: " << dosingPumpNumber << "; taskIndex: " << dosingTaskIndex << "    doseMilliLitersFraction: " << readTaskPointer->doseMilliLitersFraction << endl;
                Serial << "pumpNumber: " << dosingPumpNumber << "; taskIndex: " << dosingTaskIndex << "    startHour: " << readTaskPointer->startHour << endl;
                Serial << "pumpNumber: " << dosingPumpNumber << "; taskIndex: " << dosingTaskIndex << "    startMinute: " << readTaskPointer->startMinute << endl;

                Serial << readTaskPointer->dayOfWeek << " toEqual " << savedTaskPointer->dayOfWeek << " -> " << (savedTaskPointer->dayOfWeek == readTaskPointer->dayOfWeek) << endl;
                Serial << readTaskPointer->startHour << " toEqual " << savedTaskPointer->startHour << " -> " << (savedTaskPointer->startHour == readTaskPointer->startHour) << endl;
                Serial << readTaskPointer->startMinute << " toEqual " << savedTaskPointer->startMinute << " -> " << (savedTaskPointer->startMinute == readTaskPointer->startMinute) << endl;
                Serial << readTaskPointer->doseMilliLiters << " toEqual " << savedTaskPointer->doseMilliLiters << " -> " << (savedTaskPointer->doseMilliLiters == readTaskPointer->doseMilliLiters) << endl;
                Serial << readTaskPointer->doseMilliLitersFraction << " toEqual " << savedTaskPointer->doseMilliLitersFraction << " -> " << (savedTaskPointer->doseMilliLitersFraction == readTaskPointer->doseMilliLitersFraction) << endl;
            }
            Serial << endl;
        }
        Serial << "Done reading schedules from Storage" << endl;
        Serial << ":: TEST End ~ ===============================================" << endl;
        Serial << endl;
// */

        runTest = false;
    }
}
