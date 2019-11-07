#ifndef _AQUARIUM_CONTROLLER_ARDUINO_AMBIENT_STATION_ARDUINO_DHT_HUB_
#define _AQUARIUM_CONTROLLER_ARDUINO_AMBIENT_STATION_ARDUINO_DHT_HUB_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Common/Sensor.h>
#include <Enums/TemperatureUnit.h>

/**
 * DHT22 and AM2302 often have a pull-up already inside, but it doesn't hurt to add another one!
 */

enum class DhtModel : uint8_t {
    Dht11 = 11,
    Dht12 = 12,
    Dht21 = 21,
    Dht22 = 22,
    Am2301 = 21,
    Am2302 = 22,
};

class ArduinoDhtHub :
        public AbstractRunnable {

private:

    DHT dht;
    Sensor<float> &temperatureSensor;
    Sensor<float> &humiditySensor;
    const TemperatureUnit temperatureUnit;
    static constexpr uint32_t delayMs = 2200; // <- average response time is 2 seconds
    uint32_t delayStartMs = 0;

public:

    /**
     * <br/>
     * Hub handling the retrieval of temperature and humidity data from the DHT sensor.
     *
     * @param mcuPin – micro-controller pin where the sensor is attached
     * @param dhtModel – model of the DHT sensor
     * @param temperatureUnit – flag to call appropriate methods
     * @param outTemperatureSensor – sensor reference whose reading will be updated
     * @param outHumiditySensor  – sensor reference whose reading will be updated
     */
    ArduinoDhtHub(
            const uint8_t mcuPin,
            const DhtModel dhtModel,
            Sensor<float> &outTemperatureSensor,
            Sensor<float> &outHumiditySensor,
            TemperatureUnit temperatureUnit
    ) :
            dht(mcuPin, static_cast<uint8_t>(dhtModel)),
            temperatureSensor(outTemperatureSensor),
            humiditySensor(outHumiditySensor),
            temperatureUnit(temperatureUnit) {}

    void setup() override {
        dht.begin();
    }

    void loop() override {
        if (delayStartMs == 0 || millis() - delayStartMs > delayMs) {

            float f = NAN;

            f = dht.readHumidity();
            if (f != NAN) {
                humiditySensor.setReading(f);
            } else {
                humiditySensor.setReading(-54.0f);
            }

            f = NAN;

            if (temperatureUnit == TemperatureUnit::Celsius) {
                f = dht.readTemperature();
            } else {
                f = dht.readTemperature(true);
            }
            if (f != NAN) {
                temperatureSensor.setReading(f);
            } else {
                temperatureSensor.setReading(-54.0f);
            }

            delayStartMs = millis();
        }
    }
};

#endif