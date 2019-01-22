#ifndef __DOSING_STATION_H__
#define __DOSING_STATION_H__

#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingPump.h"
#include "Storage/Storage.h"

#ifndef NUMBER_OF_DOSING_PUMPS
#define NUMBER_OF_DOSING_PUMPS 4
#endif

typedef Adafruit_MotorShield MotorShield;

class DosingStation {
   private:
    Storage storage;
    uint32_t sleepStartMilis;
    uint32_t sleepPeriodMilis;

    bool hasMinutePassed = false;
    uint8_t currentMinute = 0;

    enum ShieldPort {
        M1 = 1,
        M2 = 2,
        M3 = 3,
        M4 = 4
    };

    enum class DosingStationState {
        SLEEPING = false,
        ACTIVE = true,
    } state;

    /* 
        How to set shield address
        https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino?view=all#addressing-the-shields-13-2
    */
    MotorShield motorShiled01 = MotorShield();  // default address is 0x60

    DosingPump dosingPump01 = DosingPump(motorShiled01, ShieldPort::M1, 1);
    DosingPump dosingPump02 = DosingPump(motorShiled01, ShieldPort::M2, 2);
    DosingPump dosingPump03 = DosingPump(motorShiled01, ShieldPort::M3, 3);
    DosingPump dosingPump04 = DosingPump(motorShiled01, ShieldPort::M4, 4);

#if NUMBER_OF_DOSING_PUMPS < 5
    DosingPump* dosingPumpsPointer[NUMBER_OF_DOSING_PUMPS] = {&dosingPump01, &dosingPump02, &dosingPump03, &dosingPump04};

#elif NUMBER_OF_DOSING_PUMPS < 9
    MotorShield motorShiled02 = MotorShield(0x61);

    DosingPump dosingPump05 = DosingPump(motorShiled02, ShieldPort::M1, 5);
    DosingPump dosingPump06 = DosingPump(motorShiled02, ShieldPort::M2, 6);
    DosingPump dosingPump07 = DosingPump(motorShiled02, ShieldPort::M3, 7);
    DosingPump dosingPump08 = DosingPump(motorShiled02, ShieldPort::M4, 8);

    DosingPump* dosingPumpsPointer[NUMBER_OF_DOSING_PUMPS] = {
        &dosingPump01, &dosingPump02, &dosingPump03, &dosingPump04,
        &dosingPump05, &dosingPump06, &dosingPump07, &dosingPump08};

#elif NUMBER_OF_DOSING_PUMPS < 13
    MotorShield motorShiled02 = MotorShield(0x61);
    MotorShield motorShiled03 = MotorShield(0x62);

    DosingPump dosingPump05 = DosingPump(motorShiled02, ShieldPort::M1, 5);
    DosingPump dosingPump06 = DosingPump(motorShiled02, ShieldPort::M2, 6);
    DosingPump dosingPump07 = DosingPump(motorShiled02, ShieldPort::M3, 7);
    DosingPump dosingPump08 = DosingPump(motorShiled02, ShieldPort::M4, 8);

    DosingPump dosingPump09 = DosingPump(motorShiled03, ShieldPort::M1, 9);
    DosingPump dosingPump10 = DosingPump(motorShiled03, ShieldPort::M2, 10);
    DosingPump dosingPump11 = DosingPump(motorShiled03, ShieldPort::M3, 11);
    DosingPump dosingPump12 = DosingPump(motorShiled03, ShieldPort::M4, 12);

    DosingPump* dosingPumpsPointer[NUMBER_OF_DOSING_PUMPS] = {
        &dosingPump01, &dosingPump02, &dosingPump03, &dosingPump04,
        &dosingPump05, &dosingPump06, &dosingPump07, &dosingPump08,
        &dosingPump09, &dosingPump10, &dosingPump11, &dosingPump12};

#elif NUMBER_OF_DOSING_PUMPS < 17
    MotorShield motorShiled02 = MotorShield(0x61);
    MotorShield motorShiled03 = MotorShield(0x62);
    MotorShield motorShiled04 = MotorShield(0x63);

    DosingPump dosingPump05 = DosingPump(motorShiled02, ShieldPort::M1, 5);
    DosingPump dosingPump06 = DosingPump(motorShiled02, ShieldPort::M2, 6);
    DosingPump dosingPump07 = DosingPump(motorShiled02, ShieldPort::M3, 7);
    DosingPump dosingPump08 = DosingPump(motorShiled02, ShieldPort::M4, 8);

    DosingPump dosingPump09 = DosingPump(motorShiled03, ShieldPort::M1, 9);
    DosingPump dosingPump10 = DosingPump(motorShiled03, ShieldPort::M2, 10);
    DosingPump dosingPump11 = DosingPump(motorShiled03, ShieldPort::M3, 11);
    DosingPump dosingPump12 = DosingPump(motorShiled03, ShieldPort::M4, 12);

    DosingPump dosingPump13 = DosingPump(motorShiled04, ShieldPort::M1, 13);
    DosingPump dosingPump14 = DosingPump(motorShiled04, ShieldPort::M2, 14);
    DosingPump dosingPump15 = DosingPump(motorShiled04, ShieldPort::M3, 15);
    DosingPump dosingPump16 = DosingPump(motorShiled04, ShieldPort::M4, 16);

    DosingPump* dosingPumpsPointer[NUMBER_OF_DOSING_PUMPS] = {
        &dosingPump01, &dosingPump02, &dosingPump03, &dosingPump04,
        &dosingPump05, &dosingPump06, &dosingPump07, &dosingPump08,
        &dosingPump09, &dosingPump10, &dosingPump11, &dosingPump12,
        &dosingPump13, &dosingPump14, &dosingPump15, &dosingPump16};
#endif

   public:
    void sleep(uint32_t sleepMinutes) {
        //
        state = DosingStationState::SLEEPING;
        sleepPeriodMilis = sleepMinutes * 60ul * 1000ul;
        sleepStartMilis = millis();
    }

    void setup() {
        //
#if NUMBER_OF_DOSING_PUMPS < 5
        motorShiled01.begin();
#elif NUMBER_OF_DOSING_PUMPS < 9
        motorShiled01.begin();
        motorShiled02.begin();
#elif NUMBER_OF_DOSING_PUMPS < 13
        motorShiled01.begin();
        motorShiled02.begin();
        motorShiled03.begin();
#elif NUMBER_OF_DOSING_PUMPS < 17
        motorShiled01.begin();
        motorShiled02.begin();
        motorShiled03.begin();
        motorShiled04.begin();
#endif

        for (uint8_t i = 0; i < sizeof(dosingPumpsPointer) / sizeof(*dosingPumpsPointer); i++) {
            dosingPumpsPointer[i]->setup();
        }

        currentMinute = minute();

        state = DosingStationState::ACTIVE;
    }

    void update(bool minuteHeartbeat, uint32_t currentMillis) {
        //
        switch (state) {
            case DosingStationState::ACTIVE:
                for (uint8_t i = 0; i < sizeof(dosingPumpsPointer) / sizeof(*dosingPumpsPointer); i++) {
                    dosingPumpsPointer[i]->update(minuteHeartbeat, currentMillis);
                }
                break;

            case DosingStationState::SLEEPING:
                if ((currentMillis - sleepStartMilis) > sleepPeriodMilis) {
                    state = DosingStationState::ACTIVE;
                }
                break;
        }
    }
};

#endif