#ifndef __TEMPERATURE_SENSOR_DS18B20_H__
#define __TEMPERATURE_SENSOR_DS18B20_H__

#define __PROBE_RESOLUTION_BITS__ 9

/* DS18B20 */
#include "DallasTemperature.h"  // https://github.com/milesburton/Arduino-Temperature-Control-Library

class TemperatureSensorDS18B20 {
   private:
    DallasTemperature dsSensors;
    uint8_t temperatureProbeIndex;

    DeviceAddress temparatureProbeAddress;
    uint64_t readOneWireMilis;
    uint32_t readOneWirePeriodMilis;

    float adjustTemperatureCelsius = 0;
    float temperatureCelsius = -127;

   public:
    TemperatureSensorDS18B20(
        OneWire &oneWireToAttach,
        uint8_t indexToAttach) : dsSensors(&oneWireToAttach), temperatureProbeIndex(indexToAttach) {
        readOneWirePeriodMilis = 750 / (1 << (12 - __PROBE_RESOLUTION_BITS__));
    }

    void calibrate(float adjustToTemperatureCelsius) {
        /* assuming linear deviation around 25 °C */
        if (temperatureCelsius != -127) {
            adjustTemperatureCelsius = adjustToTemperatureCelsius - temperatureCelsius;
        }
    }

    float getTemperatureCelsius() {
        return temperatureCelsius;
    }

    void setup() {
        /* setup for the DS18B20 aquarium tempearature probe */
        dsSensors.begin();
        delay(1000);
        if (dsSensors.getAddress(temparatureProbeAddress, temperatureProbeIndex)) {
            dsSensors.setResolution(temparatureProbeAddress, __PROBE_RESOLUTION_BITS__);
            dsSensors.setWaitForConversion(false);  // makes it async
            dsSensors.requestTemperaturesByAddress(temparatureProbeAddress);
            readOneWireMilis = millis();
        }
    }

    void update(uint32_t currentMillis) {
        /* ASYNC read the DS18B20 aquarium tempearature probe */
        if ((currentMillis - readOneWireMilis) > readOneWirePeriodMilis) {
            temperatureCelsius = dsSensors.getTempC(temparatureProbeAddress) + adjustTemperatureCelsius;
            readOneWireMilis = currentMillis;
            dsSensors.requestTemperaturesByAddress(temparatureProbeAddress);
        }
    }
};

#endif