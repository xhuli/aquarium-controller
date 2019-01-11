#ifndef __DOSING_STATION_H__
#define __DOSING_STATION_H__

#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingPump.h"

#ifndef NUMBER_OF_DOSING_PUMPS
#define NUMBER_OF_DOSING_PUMPS 4
#endif

     typedef Adafruit_MotorShield MotorShield;

class DosingStation {
    unsigned long sleepStartMilis;
    unsigned long sleepPeriodMilis;

    enum ShieldPort {
        M1 = 1,
        M2 = 2,
        M3 = 3,
        M4 = 4
    };

    enum State {
        SLEEPING = false,
        ACTIVE = true,
    } state;

    MotorShield motorShiled01 = MotorShield();  // default address is 0x60
    /* 
        Check the link below to learn how te set the shield address
        https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino?view=all#addressing-the-shields-13-2
    */
    // MotorShield motorShiled02 = MotorShield(0x61);
    // MotorShield motorShiled03 = MotorShield(0x62);

    DosingPump dosingPump01 = DosingPump(motorShiled01, M1, 1);
    DosingPump dosingPump02 = DosingPump(motorShiled01, M2, 2);
    DosingPump dosingPump03 = DosingPump(motorShiled01, M3, 3);
    DosingPump dosingPump04 = DosingPump(motorShiled01, M4, 4);

    // DosingPump dosingPump05 = DosingPump(motorShiled02, M1, 5);
    // DosingPump dosingPump06 = DosingPump(motorShiled02, M2, 6);
    // DosingPump dosingPump07 = DosingPump(motorShiled02, M3, 7);
    // DosingPump dosingPump08 = DosingPump(motorShiled02, M4, 8);

    DosingPump* dosingPump[NUMBER_OF_DOSING_PUMPS] = {&dosingPump01, &dosingPump02, &dosingPump03, &dosingPump04};

   public:
    void sleep(uint32_t sleepMinutes) {
        state = SLEEPING;
        sleepPeriodMilis = sleepMinutes * 60ul * 1000ul;
        sleepStartMilis = millis();
    }

    void setup() {
        state = ACTIVE;

        motorShiled01.begin();
        // motorShiled02.begin();

        for (uint8_t i = 0; i < sizeof(dosingPump) / sizeof(*dosingPump); i++) {
            dosingPump[i]->setup();
        }
    }

    void update(bool minuteHeartbeat, uint64_t currentMillis) {
        switch (state) {
            case ACTIVE:
                for (uint8_t i = 0; i < sizeof(dosingPump) / sizeof(*dosingPump); i++) {
                    dosingPump[i]->update(minuteHeartbeat, currentMillis);
                }
                break;

            case SLEEPING:
                if ((unsigned long)(currentMillis - sleepStartMilis) > sleepPeriodMilis) {
                    state = ACTIVE;
                }
                break;
        }
    }
};

#endif