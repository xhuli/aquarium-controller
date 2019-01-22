#ifndef __TEMPERATURE_AND_HUMIDITY_SENSOR_H__
#define __TEMPERATURE_AND_HUMIDITY_SENSOR_H__

#include "Adafruit_Sensor.h"  // https://github.com/adafruit/Adafruit_Sensor
#include "DHT.h"              // https://github.com/adafruit/DHT-sensor-library
#include "DHT_U.h"            // https://github.com/adafruit/DHT-sensor-library

class TemperatureAndHumiditySensor {
   private:
    DHT_Unified dht;
    sensors_event_t event;  // adafruit sensor event

    float temperatureCelsius = -127;
    float humidityPercent = -1.0;

   public:
    TemperatureAndHumiditySensor(uint8_t dhtType,
                                 uint8_t dhtAttachToPin) : dht(dhtAttachToPin, dhtType) {}

    float getTemperatureCelsius() {
        return temperatureCelsius;
    }

    float getHumidityPercent() {
        return humidityPercent;
    }

    void setup() {
        dht.begin();
    }

    void update() {
        dht.temperature().getEvent(&event);
        temperatureCelsius = event.temperature;

        dht.humidity().getEvent(&event);
        humidityPercent = event.relative_humidity;
    }
};

#endif