#ifndef _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_STATION_H_
#define _AQUARIUM_CONTROLLER_INCLUDE_DOSING_STATION_DOSING_STATION_H_
#pragma once

#include <Abstract/AbstractRunnable.h>
#include <Abstract/AbstractSleepable.h>

class Adafruit_MotorShield;

class DosingStation :
        public AbstractRunnable,
        public AbstractSleepable {

private:
    uint8_t numberOfDosingPorts;
    uint8_t currentMinute = 60;

public:
    LinkedList<Adafruit_MotorShield *> motorShieldsList{};
    LinkedList<DosingPort *> dosingPortsList{};

    static constexpr uint8_t AdafruitMotorShieldDcPortCount = 4;
    static constexpr uint16_t defaultMilliSecondsPerMilliLiter = 2047;

    explicit DosingStation(
            uint8_t numberOfDosingPorts
    ) :
            numberOfDosingPorts(numberOfDosingPorts) {};

    uint8_t getNumberOfDosingPorts() const {
        return numberOfDosingPorts;
    }

    void startSleeping(uint32_t const &sleepMs) override {
        AbstractSleepable::sleeping = true;
        AbstractSleepable::sleepMs = sleepMs;
        AbstractSleepable::sleepStartMs = millis();
        DosingStation::dosingPortsList.forEach([](DosingPort *dosingPort) { dosingPort->stopDispensing(); });
    }

    void stopSleeping() override {
        AbstractSleepable::sleeping = true;
    }

    /**
     * <br/>
     * Start port calibration for the port at the given index in the ports list. The list is zero based.
     *
     * @param portIndex – the zero based index of the port to calibrate
     */
    void startPortCalibration(uint8_t const portIndex) {
        dosingPortsList.get(portIndex)->startStopCalibrating();
    }

    /**
     * <br/>
     * Stop port calibration for the port at the given index in the ports list. The list is zero based.
     *
     * @param portIndex – the zero based index of the port to stop calibrating
     */
    void stopPortCalibration(uint8_t const portIndex) {
        dosingPortsList.get(portIndex)->startStopCalibrating();
    }

    void setup() override {
        uint8_t shieldNumber = 0;
        for (uint8_t dosingPortIndex = 0; dosingPortIndex < DosingStation::numberOfDosingPorts; ++dosingPortIndex) {

            uint8_t modulo = dosingPortIndex % DosingStation::AdafruitMotorShieldDcPortCount;
            uint8_t shieldDcMotorPort = (modulo != 0) ? modulo : DosingStation::AdafruitMotorShieldDcPortCount;

            /* assign DosingPort objects to shield ports */
            Adafruit_MotorShield *motorShield = motorShieldsList.get(shieldNumber);
            if (motorShield != nullptr) {
                motorShield->begin();

                auto *dosingPort = new DosingPort{motorShield->getMotor(shieldDcMotorPort), DosingStation::defaultMilliSecondsPerMilliLiter};

                dosingPortsList.add(dosingPort);
            }

            if (modulo == 0) { ++shieldNumber; }
        }
    }

    void loop() override {

        /* sleep */
        if (DosingStation::isSleeping()) {
            if (AbstractSleepable::shouldStopSleeping()) {
                DosingStation::stopSleeping();
                /*
                 * make `currentMinute` is different from actual `minute()`,
                 * so that the pumps will check their schedule right after sleep ends
                 */
                DosingStation::currentMinute = minute() - 1; // <- be careful what you do with `currentMinute` when `minute() = 0`
            } else {
                return;
            }
        }

        /* run scheduled port tasks only once per minute */
        if (DosingStation::currentMinute == minute()) {
            return;
        } else {
            DosingStation::currentMinute = minute();

            DosingStation::dosingPortsList.forEach([](DosingPort *pDosingPort) {
                pDosingPort->runScheduledTasks();
            });
        }
    }
};

#endif