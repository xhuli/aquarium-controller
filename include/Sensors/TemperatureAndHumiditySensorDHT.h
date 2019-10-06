#ifndef __TEMPERATURE_AND_HUMIDITY_SENSOR_DHT_H__
#define __TEMPERATURE_AND_HUMIDITY_SENSOR_DHT_H__

#include "Abstract/AbstractHumiditySensor.h"
#include "Abstract/AbstractTemperatureSensor.h"
#include "Adafruit_Sensor.h"  // https://github.com/adafruit/Adafruit_Sensor
#include "DHT.h"              // https://github.com/adafruit/DHT-sensor-library
#include "DHT_U.h"            // https://github.com/adafruit/DHT-sensor-library

class TemperatureAndHumiditySensorDHT :
        public AbstractHumiditySensor,
        public AbstractTemperatureSensor {
private:
    DHT_Unified dht;
    sensors_event_t adaFruitSensorsEvent; // todo: this is bad, this class should not exist

public:
    TemperatureAndHumiditySensorDHT(uint8_t dhtType, uint8_t dhtAttachToPin) : dht(dhtAttachToPin, dhtType) {}

    void setup() override {
        dht.begin();
    }

    void update(uint32_t currentMillis) override {
        dht.temperature().getEvent(&adaFruitSensorsEvent);
        temperatureCelsius = adaFruitSensorsEvent.temperature;

        dht.humidity().getEvent(&adaFruitSensorsEvent);
        humidityPercent = adaFruitSensorsEvent.relative_humidity;
    }
};

#endif