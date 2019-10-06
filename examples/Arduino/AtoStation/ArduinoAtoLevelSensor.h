#ifndef _AQUARIUM_CONTROLLER__ARDUINO_ATO_STATION_ARDUINO_ATO_LEVEL_SENSOR_H_
#define _AQUARIUM_CONTROLLER__ARDUINO_ATO_STATION_ARDUINO_ATO_LEVEL_SENSOR_H_

#include <Enums/LiquidLevelState.h>
#include <Common/Sensor.h>
#include <Abstract/IForwarder.h>
#include <AtoStation/AtoStation.h>
#include <stdint-gcc.h>

/**
 * <br/>
 * ArduinoAtoLevelSensor.<br/>
 *
 * @param forwarder – forwards readings to consumer
 * @param initialValue – initial sensor logic value
 * @param mcuPin – micro-controller pin where the sensor is attached
 * @param logicalHighLiquidStateInPinVolts – volts equaling logical <tt>HIGH</tt>
 */
class ArduinoAtoLevelSensor :
        public Sensor<LiquidLevelState>,
        public AbstractRunnable {

private :

    const uint8_t mcuPin;
    const uint8_t logicalHighLiquidStateInPinVolts;
    uint8_t pinReadValue = LOW;

public:
    /**
        * <br/>
        * ArduinoAtoLevelSensor constructor.
        *
        * @param forwarder – forwards readings to consumer
        * @param initialValue – sensor initial logic value
        * @param mcuPin – micro-controller pin where the sensor is attached
        * @param logicalHighLiquidStateInPinVolts – volts equaling logical <tt>HIGH</tt>
        */
    ArduinoAtoLevelSensor(
            IForwarder<LiquidLevelState> &forwarder,
            LiquidLevelState initialValue,
            uint8_t const mcuPin,
            uint8_t logicalHighLiquidStateInPinVolts
    ) :
            Sensor<LiquidLevelState>(&forwarder, initialValue),
            mcuPin(mcuPin),
            logicalHighLiquidStateInPinVolts(logicalHighLiquidStateInPinVolts) {}

    void setup() override {
        pinMode(mcuPin, INPUT); /* warn: Arduino specific */
    }

    void loop() override {

        pinReadValue = digitalRead(mcuPin);

        if (pinReadValue == HIGH) {
            if (logicalHighLiquidStateInPinVolts == HIGH) {
                ArduinoAtoLevelSensor::setReading(LiquidLevelState::High);
            } else {
                ArduinoAtoLevelSensor::setReading(LiquidLevelState::Low);
            }
        } else {
            if (logicalHighLiquidStateInPinVolts == HIGH) {
                ArduinoAtoLevelSensor::setReading(LiquidLevelState::Low);
            } else {
                ArduinoAtoLevelSensor::setReading(LiquidLevelState::High);
            }
        }
    }
};

#endif