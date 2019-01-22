// #define DS_DEBUG

#include <Arduino.h>
#include <OneWire.h>    // https://github.com/PaulStoffregen/OneWire
#include <Streaming.h>  // http://arduiniana.org/libraries/streaming/
#include <stdint.h>     // integer definitions: int8_t, int16_t, ..., uint8_t, ...

#ifdef DS_DEBUG
#include "DallasTemperature.h"
#else
#include "Sensors/TemperatureSensorDS18B20.h"
#endif

enum ArduinoPin {
    /* edit pin numbers according to your hardware setup */
    // HeatingRelay = 1,
    // CoolingRelay = 2,
    // DHT22Sesnsor = 3,  // ambient temperature and humidity
    OneWireBus = 2,
    // Buzzer = 5,
};

const long SERIAL_SPEED = 9600;

OneWire oneWire(2);
#ifdef DS_DEBUG
DallasTemperature sensors(&oneWire);
#else
TemperatureSensorDS18B20 sumpTemperatureSensor = TemperatureSensorDS18B20(oneWire, 0);
#endif

uint32_t currentMillis = 0;
uint32_t previousMilis = 0;
uint32_t refreshPeriod = 2000;

void setup() {
    Serial.begin(SERIAL_SPEED);
    while (!Serial) {
        // wait until Arduino Serial Monitor opens
    }
    delay(100);

    currentMillis = millis();

#ifdef DS_DEBUG
    sensors.begin();
#else
    sumpTemperatureSensor.setup();
    sumpTemperatureSensor.update(currentMillis);
// sumpTemperatureSensor.calibrate(36.22);
#endif
}

void loop() {
    currentMillis = millis();

    if ((currentMillis - previousMilis) > refreshPeriod) {
        previousMilis = currentMillis;

#ifdef DS_DEBUG
        sensors.requestTemperatures();
        Serial << "0: " << sensors.getTempCByIndex(0) << " °C" << endl;
        Serial << "1: " << sensors.getTempCByIndex(1) << " °C" << endl;
#else
        sumpTemperatureSensor.update(currentMillis);

        Serial << "Temperatrure: " << sumpTemperatureSensor.getTemperatureCelsius() << " °C" << endl;
        Serial << endl;
#endif
    }
}