#ifndef _AQUARIUM_CONTROLLER_ARDUINO_AMBIENT_STATION_ARDUINO_DS_TEMPERATURE_SENSORS_HUB_LED_
#define _AQUARIUM_CONTROLLER_ARDUINO_AMBIENT_STATION_ARDUINO_DS_TEMPERATURE_SENSORS_HUB_LED_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Common/CountDown.h>
#include <Enums/TemperatureUnit.h>

enum class DsResolutionBits : uint8_t {
    __9 = 9,
    _10 = 10,
    _11 = 11,
    _12 = 12,
};

/**
 * <br/>
 * <a href="https://datasheets.maximintegrated.com/en/ds/DS18B20.pdf">DS18B20 - Programmable Resolution 1-Wire Digital Thermometer</a><br/>
 * Wrapper class for the <tt>DalasTemperature</tt> library <a href="https://github.com/milesburton/Arduino-Temperature-Control-Library">Arduino-Temperature-Control-Library</a><br/>
 * Response times regarding bit resolution:<br/>
 * <tt>• 09-bit resolution => 93.75 ms</tt><br/>
 * <tt>• 10-bit resolution => 187.5 ms</tt><br/>
 * <tt>• 11-bit resolution => 375 ms</tt><br/>
 * <tt>• 12-bit resolution => 750 ms</tt><br/>
 */
class ArduinoDsTemperatureSensorsHub :
        public AbstractRunnable {

private:

    DallasTemperature sensors;
    const DsResolutionBits dsGlobalResolutionBits;
    LinkedHashMap<DeviceAddress *, DsResolutionBits> *addressToResolutionMap;
    LinkedHashMap<DeviceAddress *, Sensor<float> *> *addressToOutSensorMap;
    TemperatureUnit temperatureUnit = TemperatureUnit::Celsius;
    uint16_t waitForConversionMs = 750;
    uint32_t waitForConversionStartMs = 0;

public:

    /**
     * <br/>
     * All mapped sensors readings will be updated with precision and speed depending on specified resolution bits.<br/>
     * All sensor readings depend on specified temperature measurement unit.<br/>
     *
     * @param pOneWire – pointer/address to instance of <tt>OneWire</tt>
     * @param dsGlobalResolutionBits – affects reading precision and speed for all sensors
     * @param addressToOutSensorMap – map of DS18B20 unique 64-bit serial code and <tt>Sensor</tt> instance
     * @param temperatureUnit – flag to call appropriate conversion methods
     */
    ArduinoDsTemperatureSensorsHub(
            OneWire *pOneWire,
            const DsResolutionBits dsGlobalResolutionBits,
            LinkedHashMap<DeviceAddress *, Sensor<float> *> *addressToOutSensorMap,
            TemperatureUnit temperatureUnit
    ) :
            sensors(pOneWire),
            dsGlobalResolutionBits(dsGlobalResolutionBits),
            addressToResolutionMap(nullptr),
            addressToOutSensorMap(addressToOutSensorMap),
            temperatureUnit(temperatureUnit),
            waitForConversionMs(sensors.millisToWaitForConversion(static_cast<uint8_t>(dsGlobalResolutionBits))) {};

    /**
     * <br/>
     * Mapped sensor reading will be updated with precision depending on mapped resolution bits for each address.<br/>
     * All sensor reading speed will be the same, it's the speed of the slowest sensor with greatest precision/resolution.<br/>
     * All sensor readings depend on specified temperature measurement unit.
     *
     * @param pOneWire – pointer/address to instance of <tt>OneWire</tt>
     * @param addressToResolutionMap – map of DS18B20 unique 64-bit serial code and resolution bits
     * @param addressToOutSensorMap – map of DS18B20 unique 64-bit serial code and <tt>Sensor</tt> instance
     * @param temperatureUnit – flag to call appropriate conversion methods
     */
    ArduinoDsTemperatureSensorsHub(
            OneWire *pOneWire,
            LinkedHashMap<DeviceAddress *, DsResolutionBits> *addressToResolutionMap,
            LinkedHashMap<DeviceAddress *, Sensor<float> *> *addressToOutSensorMap,
            TemperatureUnit temperatureUnit
    ) :
            sensors(pOneWire),
            dsGlobalResolutionBits(DsResolutionBits::__9),
            addressToResolutionMap(addressToResolutionMap),
            addressToOutSensorMap(addressToOutSensorMap),
            temperatureUnit(temperatureUnit) {};

    void setup() override {
        sensors.begin();
        delay(999);
#ifdef __SERIAL_DEBUG__
        Serial << "DS device count: " << static_cast<int>(sensors.getDeviceCount()) << "\n";
#endif
        // Disable blocking wait for conversion, go to ASYNC mode
        sensors.setWaitForConversion(false);

        if (addressToResolutionMap != nullptr) {
            // Set individual resolution if specified

            uint8_t greatestResolution = static_cast<uint8_t>(dsGlobalResolutionBits);

            for (KeyValuePair<DeviceAddress *, DsResolutionBits> *pKeyValuePair = addressToResolutionMap->getFirstPair();
                 pKeyValuePair;
                 pKeyValuePair = pKeyValuePair->next) {
                //
                uint8_t mappedDeviceResolution = static_cast<uint8_t>(pKeyValuePair->value);
                sensors.setResolution(*pKeyValuePair->key, mappedDeviceResolution);

                // figure out the greatest used resolution
                if (mappedDeviceResolution > greatestResolution) {
                    greatestResolution = mappedDeviceResolution;
                }
            }

            waitForConversionMs = sensors.millisToWaitForConversion(greatestResolution);

        } else {
            // Set global resolution for ALL sensors.
            // Note, this also affects sensors that are not specified in the 'addressToOutSensorMap'
            sensors.setResolution(static_cast<uint8_t>(dsGlobalResolutionBits));
        }
    }

    void loop() override {
        if (waitForConversionStartMs == 0 || millis() - waitForConversionStartMs > waitForConversionMs) {

            // Update sensor readings
            if (temperatureUnit == TemperatureUnit::Celsius) {
                for (KeyValuePair<DeviceAddress *, Sensor<float> *> *pKeyValuePair = addressToOutSensorMap->getFirstPair();
                     pKeyValuePair;
                     pKeyValuePair = pKeyValuePair->next) {
                    //
                    pKeyValuePair->value->setReading(sensors.getTempC(*pKeyValuePair->key)); // <- getTempC(...)
                }
            } else {
                for (KeyValuePair<DeviceAddress *, Sensor<float> *> *pKeyValuePair = addressToOutSensorMap->getFirstPair();
                     pKeyValuePair;
                     pKeyValuePair = pKeyValuePair->next) {
                    //
                    pKeyValuePair->value->setReading(sensors.getTempF(*pKeyValuePair->key)); // <- getTempF(...)
                }
            }

            sensors.requestTemperatures();
            waitForConversionStartMs = millis();
        }
    }
};

#endif