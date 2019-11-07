#ifndef _AQUARIUM_CONTROLLER_ARDUINO_AMBIENT_STATION_ARDUINO_DS_TEMPERATURE_SENSORS_HUB_
#define _AQUARIUM_CONTROLLER_ARDUINO_AMBIENT_STATION_ARDUINO_DS_TEMPERATURE_SENSORS_HUB_
#pragma once

#include <Abstract/AbstractRunnable.h>
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
 * Wrapper class for the <tt>DallasTemperature</tt> library
 * <a href="https://github.com/milesburton/Arduino-Temperature-Control-Library">Arduino-Temperature-Control-Library</a><br/>
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
    const DsResolutionBits globalDsResolutionBits;
    LinkedMap<DeviceAddress *, DsResolutionBits> *addressToResolutionMap;
    LinkedMap<DeviceAddress *, Sensor<float> *> *addressToOutSensorMap;
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
     * @param globalDsResolutionBits – affects reading precision and speed for all sensors
     * @param addressToOutSensorMap – map of DS18B20 unique 64-bit serial code and <tt>Sensor</tt> instance
     * @param temperatureUnit – flag to call appropriate conversion methods
     */
    ArduinoDsTemperatureSensorsHub(
            OneWire *pOneWire,
            const DsResolutionBits globalDsResolutionBits,
            LinkedMap<DeviceAddress *, Sensor<float> *> *addressToOutSensorMap,
            TemperatureUnit temperatureUnit
    ) :
            sensors(pOneWire),
            globalDsResolutionBits(globalDsResolutionBits),
            addressToResolutionMap(nullptr),
            addressToOutSensorMap(addressToOutSensorMap),
            temperatureUnit(temperatureUnit),
            waitForConversionMs(sensors.millisToWaitForConversion(static_cast<uint8_t>(globalDsResolutionBits))) {};

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
            LinkedMap<DeviceAddress *, DsResolutionBits> *addressToResolutionMap,
            LinkedMap<DeviceAddress *, Sensor<float> *> *addressToOutSensorMap,
            TemperatureUnit temperatureUnit
    ) :
            sensors(pOneWire),
            globalDsResolutionBits(DsResolutionBits::__9),
            addressToResolutionMap(addressToResolutionMap),
            addressToOutSensorMap(addressToOutSensorMap),
            temperatureUnit(temperatureUnit) {};

    void setup() override {
        sensors.begin();
        delay(999);
#ifdef __SERIAL_DEBUG__
        Serial << "DS device count: " << static_cast<int>(sensors.getDeviceCount()) << "\n";
#endif
        /* Disable blocking wait for conversion, go to ASYNC mode */
        sensors.setWaitForConversion(false);

        if (addressToResolutionMap != nullptr) {
            /* Set individual resolution if specified */

            uint8_t greatestResolution = static_cast<uint8_t>(globalDsResolutionBits);

            MapIterator<DeviceAddress *, DsResolutionBits> const &mapIterator = addressToResolutionMap->iterator();

            while (mapIterator.hasNext()) {
                KeyValue<DeviceAddress *, DsResolutionBits> *pKeyValue = mapIterator.next();
                DeviceAddress *pDeviceAddress = pKeyValue->getKey();
                uint8_t mappedDeviceResolution = static_cast<uint8_t>(pKeyValue->getValue());

                sensors.setResolution(*pDeviceAddress, mappedDeviceResolution);

                /* figure out the greatest used resolution */
                if (mappedDeviceResolution > greatestResolution) {
                    greatestResolution = mappedDeviceResolution;
                }
            }

            waitForConversionMs = sensors.millisToWaitForConversion(greatestResolution);

        } else {
            /* Set global resolution for ALL sensors. */
            /* Note, this also affects sensors that are not specified in the 'addressToOutSensorMap' */
            sensors.setResolution(static_cast<uint8_t>(globalDsResolutionBits));
        }
    }

    void loop() override {
        if (waitForConversionStartMs == 0 || millis() - waitForConversionStartMs > waitForConversionMs) {

            MapIterator<DeviceAddress *, Sensor<float> *> const &mapIterator = addressToOutSensorMap->iterator();

            while (mapIterator.hasNext()) {
                KeyValue<DeviceAddress *, Sensor<float> *> *pKeyValue = mapIterator.next();
                DeviceAddress *pDeviceAddress = pKeyValue->getKey();
                Sensor<float> *pSensor = pKeyValue->getValue();

                /* Update sensor readings */
                if (temperatureUnit == TemperatureUnit::Celsius) {
                    pSensor->setReading(sensors.getTempC(*pDeviceAddress)); // <- getTempC(...)
                } else {
                    pSensor->setReading(sensors.getTempF(*pDeviceAddress)); // <- getTempF(...)
                }
            }

            sensors.requestTemperatures();
            waitForConversionStartMs = millis();
        }
    }
};

#endif