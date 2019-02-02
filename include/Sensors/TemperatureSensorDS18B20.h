#ifndef __TEMPERATURE_SENSOR_DS18B20_H__
#define __TEMPERATURE_SENSOR_DS18B20_H__

#define __PROBE_RESOLUTION_BITS__ 9

#include "Abstract/AbstractTemperatureSensor.h"
#include "DallasTemperature.h"  // https://github.com/milesburton/Arduino-Temperature-Control-Library

class TemperatureSensorDS18B20 : public AbstractTemperatureSensor {
   private:
    DallasTemperature dsSensorsBus;
    uint8_t temperatureProbeIndex;

    DeviceAddress temperatureProbeAddress;
    uint64_t readOneWireMillis;
    uint32_t readOneWirePeriodMillis;

   public:
    TemperatureSensorDS18B20(
        OneWire &oneWireToAttach,
        uint8_t indexToAttach) : dsSensorsBus(&oneWireToAttach), temperatureProbeIndex(indexToAttach) {
        readOneWirePeriodMillis = 750 / (1 << (12 - __PROBE_RESOLUTION_BITS__));
    }

    void setup() {
        /* setup for the DS18B20 aquarium temperature probe */
        dsSensorsBus.begin();
        delay(1000);
        if (dsSensorsBus.getAddress(temperatureProbeAddress, temperatureProbeIndex)) {
            dsSensorsBus.setResolution(temperatureProbeAddress, __PROBE_RESOLUTION_BITS__);
            dsSensorsBus.setWaitForConversion(false);  // makes it async
            dsSensorsBus.requestTemperaturesByAddress(temperatureProbeAddress);
            readOneWireMillis = millis();
        }
    }

    void update(uint32_t currentMillis) {
        /* ASYNC read the DS18B20 aquarium temperature probe */
        if ((currentMillis - readOneWireMillis) > readOneWirePeriodMillis) {
            temperatureCelsius = dsSensorsBus.getTempC(temperatureProbeAddress);
            readOneWireMillis = currentMillis;
            dsSensorsBus.requestTemperaturesByAddress(temperatureProbeAddress);
        }
    }
};

#endif