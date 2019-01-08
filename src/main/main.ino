#include <Adafruit_MotorShield.h>  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library
#include "utility/Adafruit_MS_PWMServoDriver.h"  // https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library

typedef Adafruit_MotorShield MotorShield;
typedef Adafruit_DCMotor DcMotor;

const long SERIAL_SPEED = 9600;

MotorShield motorShiled = Adafruit_MotorShield();
DcMotor *dosingPump01 = motorShiled.getMotor(1);
DcMotor *dosingPump02 = motorShiled.getMotor(2);
DcMotor *dosingPump03 = motorShiled.getMotor(3);
DcMotor *dosingPump04 = motorShiled.getMotor(4);

void setup() {
  Serial.begin(SERIAL_SPEED);
  while (!Serial) {
    // wait until Arduino Serial Monitor opens
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

class DosingPump {
    byte motorPort;
    MotorShield motorShiled;
    DcMotor *dosingPump;

    enum State {
      PUMPING = true,
      IDLE = false
    } state;

  public:
    DosingPump(Adafruit_MotorShield attachToShield, byte attachToMotorPort) : motorShiled(attachToShield),
      motorPort(attachToMotorPort) {
      DcMotor *dosingPump = motorShiled.getMotor(motorPort);
      state = IDLE;
    }

    void setSpeed(byte speed) {
      dosingPump->setSpeed(speed);
    }

    void start() {
      dosingPump->run(FORWARD);
      state = PUMPING;
    }

    void stop() {
      dosingPump->run(RELEASE);
      state = IDLE;
    }

    void setup() {}

    void loop() {
      switch (state) {
        case IDLE:
          Serial.println("Idle");
          break;

        case PUMPING:
          Serial.println("Pumping");
          break;
      }
    }
};
