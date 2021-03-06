#ifndef __TEMPERATURE_SENSOR_DS18B20_H__
#define __TEMPERATURE_SENSOR_DS18B20_H__

#include "Abstract/AbstractTemperatureSensor.h"
#include "DallasTemperature.h"  // https://github.com/milesburton/Arduino-Temperature-Control-Library

constexpr uint8_t ProbeResolutionBits = 9;

class TemperatureSensorDS18B20 : public AbstractTemperatureSensor {
private:
    DallasTemperature dsSensorsBus;
    uint8_t temperatureProbeIndex;

    DeviceAddress temperatureProbeAddress;
    uint32_t readOneWireMillis = 0;
    uint32_t readOneWirePeriodMillis = 0;

public:
    TemperatureSensorDS18B20(
            OneWire &oneWireToAttach,
            uint8_t indexToAttach
    ) :
            dsSensorsBus(&oneWireToAttach),
            temperatureProbeIndex(indexToAttach) {
        readOneWirePeriodMillis = 750 / (1 << (12 - ProbeResolutionBits));
    }

    void setup() {
        /* setup for the DS18B20 aquarium temperature probe */
        dsSensorsBus.begin();
        delay(1000);
        if (dsSensorsBus.getAddress(temperatureProbeAddress, temperatureProbeIndex)) {
            dsSensorsBus.setResolution(temperatureProbeAddress, ProbeResolutionBits);
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