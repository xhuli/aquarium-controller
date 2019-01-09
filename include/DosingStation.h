#ifndef __DOSING_STATION_H__
#define __DOSING_STATION_H__

#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "DosingPump.h"

typedef Adafruit_MotorShield MotorShield;

class DosingStation {
    unsigned long startSleepMilis;
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

    DosingPump dosingPump01 = DosingPump(motorShiled01, M1);
    DosingPump dosingPump02 = DosingPump(motorShiled01, M2);
    DosingPump dosingPump03 = DosingPump(motorShiled01, M3);
    DosingPump dosingPump04 = DosingPump(motorShiled01, M4);

    // DosingPump dosingPump05 = DosingPump(motorShiled02, M1);
    // DosingPump dosingPump06 = DosingPump(motorShiled02, M2);
    // DosingPump dosingPump07 = DosingPump(motorShiled02, M3);
    // DosingPump dosingPump08 = DosingPump(motorShiled02, M4);

   public:
    void sleep(uint8_t sleepMinutes) {
        state = SLEEPING;
        sleepPeriodMilis = ((unsigned long)sleepMinutes) * 60UL * 1000UL;
        startSleepMilis = millis();
    }

    void setup() {
        state = ACTIVE;

        motorShiled01.begin();
        // motorShiled02.begin();

        dosingPump01.setup();
        dosingPump02.setup();
        dosingPump03.setup();
        dosingPump04.setup();

        // dosingPump05.setup();
        // dosingPump06.setup();
        // dosingPump07.setup();
        // dosingPump08.setup();
    }

    void loop(bool minuteHeartbeat) {
        switch (state) {
            case ACTIVE:
                dosingPump01.loop(minuteHeartbeat);
                dosingPump02.loop(minuteHeartbeat);
                dosingPump03.loop(minuteHeartbeat);
                dosingPump04.loop(minuteHeartbeat);

                // dosingPump05.loop(minuteHeartbeat);
                // dosingPump06.loop(minuteHeartbeat);
                // dosingPump07.loop(minuteHeartbeat);
                // dosingPump08.loop(minuteHeartbeat);
                break;

            case SLEEPING:
                if ((unsigned long)(millis() - startSleepMilis) > sleepPeriodMilis) {
                    state = ACTIVE;
                }
                break;
        }
    }
};

#endif