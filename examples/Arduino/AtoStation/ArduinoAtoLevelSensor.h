#ifndef _AQUARIUM_CONTROLLER_ARDUINO_ATO_STATION_ARDUINO_ATO_LEVEL_SENSOR_H_
#define _AQUARIUM_CONTROLLER_ARDUINO_ATO_STATION_ARDUINO_ATO_LEVEL_SENSOR_H_

#ifndef HIGH
#define HIGH 0x1
#endif

#ifndef LOW
#define LOW  0x0
#endif

#ifndef INPUT
#define INPUT 0x0
#endif

#include <Enums/Level.h>
#include <Common/Sensor.h>
#include <Abstract/IForwarder.h>
#include <stdint-gcc.h>

/**
 * <br/>
 * ArduinoAtoLevelSensor.<br/>
 *
 * @param forwarder – forwards readings to consumer
 * @param initialValue – initial sensor logic value
 * @param mcuPin – micro-controller pin where the sensor is attached
 * @param notInvertedInput – <tt>true</tt> if the input signal is not inverted
 */
class ArduinoAtoLevelSensor :
        public Sensor<Level>,
        public AbstractRunnable {

private :

    const uint8_t mcuPin;
    const bool notInvertedInput;
    uint8_t pinReadValue = LOW;

public:
    /**
        * <br/>
        * ArduinoAtoLevelSensor constructor.
        *
        * @param forwarder – forwards readings to consumer
        * @param initialValue – sensor initial logic value
        * @param mcuPin – micro-controller pin where the sensor is attached
        * @param notInvertedInput – <tt>true</tt> if the input signal is not inverted
        */
    ArduinoAtoLevelSensor(
            IForwarder<Level> const &forwarder,
            Level initialValue,
            uint8_t const mcuPin,
            bool notInvertedInput
    ) :
            Sensor<Level>(&forwarder, initialValue),
            mcuPin(mcuPin),
            notInvertedInput(notInvertedInput) {}

    void setup() override {
        pinMode(mcuPin, INPUT); /* warn: Arduino specific */
    }

    void loop() override {

        pinReadValue = digitalRead(mcuPin); /* warn: Arduino specific */

        if (pinReadValue == HIGH) {
            ArduinoAtoLevelSensor::setReading((notInvertedInput) ? Level::High : Level::Low);
        } else {
            ArduinoAtoLevelSensor::setReading((notInvertedInput) ? Level::Low : Level::High);
        }
    }
};

#endif