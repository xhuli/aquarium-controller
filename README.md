# Warning
This is work in progress. The project has never been build and tested.

# Abstract
This project is for the joy of building an aquarium dosing pump. 
From a financial perspective, probably it is more feasible to buy a fabricated dosing pump.

# Requirements / Specifications
    1. dosing station can simultaneously operate 1 - 4 dosing pumps
    2. dosing station can be extended to simultaneously operate 1 - 8 dosing pumps
    3. minimum dose is 1 mL
    4. maximum dose is 255 mL
    5. dose increment is 1 mL
    
## Mandatory Features
    1. all actions are displayed on LCD screen
    2. when idle display the system date and time
    3. each dosing pump can be calibrated
    4. each dosing pump can have it's own dosing schedule
    5. each dosing schedule can have up to 24 tasks
    6. each dosing task is executed at given day of the week, hour and minute, dispensing set amount of liquid - dose
    7. no two tasks can exist at same day of the week, hour and minute in one dosing schedule (for given dosing pump)
    8. no task queuing (if previous task is not finished, pending task will not execute, will be skipped)
    9. user can set system time using an IR remote
    10. user can set system date using an IR remote
    11. user can calibrate each dosing pump using an IR remote, one at a time
    12. user can manually start/stop each dosing pump, one at a time
    13. user can create dosing task (set dayOfWeek, hour, minute, and dose) for each dosing pump using an IR remote
    14. user can update (edit) existing tasks, one at a time
    15. user can remove (delete) existing tasks, one at a time
    16. user can manually set the dosing station in sleep mode for some amount of time using an IR remote
    17. persist dosing pump calibration to survive power off
    18. persist dosing schedules to survive power off
    19. auto-reboot in case of system halt

## Optional Features
    1. display environment temperature
    2. display environment humidity
    3. display aquarium temperature

## Late Features
    1. Temperature Control
        1. user can set heater power-off temperature
        2. user can set cooler power-on temperature
    2. ATO - automatic top off
        1. user can set the ATO in sleep mode for some amount of time using an IR remote
        2. ATO will dispense water into the system when the LOW_LEVEL sensor returns NOT_SENSING
        3. ATO will dispense water into the system until the HI_LEVEL_ sensor returns SENSING, or
        4. if HI_LEVEL sensor is not met in pre-defined time the ATO will stop dispensing
        5. ATO will not dispense water if RESERVE_LOW sensor is NOT_SENSING

# Required Hardware
    1. Arduino Uno, quantity: 1
    2. I2C TB6612 Stepper Motor PCA9685 Servo Driver Shield V2, quantity: 1 or 2
    3. DS3231 RTC module, quantity: 1
    4. DC12V/24V Peristaltic Dosing pump, quantity: 1 to 8
    5. Infra-red IR Wireless Remote Control Module Kits DIY Kit HX1838 
    6. I2C 2004 SERIAL 20X4 LCD MODULE
    7. KY-012 Active Buzzer Module for Arduino
    8. MB102 Solder-less Breadboard
    9. 20cm DuPont jumper wires male-female, quantity: set of 10 or more
    10. 20cm DuPont jumper wires male-male, quantity: set of 10 or more
    11. CR2032 3 Volt Coin Button Cell Battery 
    12. Power adapter 12V 1A, quantity: 1

# Additional (nice to have) Hardware
    1. DHT22 Temperature and Humidity Sensor, quantity: 1
    2. 2 meters Thermometer Waterproof Cable Probe DS18B20, quantity: 1
    3. DS18B20 Shield (no sensor) or DS18B20 Module (with sensor), quantity: 1
    4. 5V 4-Channel Relay Module, quantity: 1

# Additional Hardware for ATO - automatic top off
    1. 6V-12V DC Brush-less Small Water Pump Submersible Motor Pump 1.8M 280L/H, quantity: 1
    2. water level sensor, quantity: 3
    3. 1 relay from the above relay module

# References
 * [Arduino the Object Oriented way](http://paulmurraycbr.github.io/ArduinoTheOOWay.html)
 * [USING MODERN C++ TECHNIQUES WITH ARDUINO](https://hackaday.com/2017/05/05/using-modern-c-techniques-with-arduino/)
 * [Adafruit Motor Shield V2](https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino?view=all#addressing-the-shields-13-2)
 * [Getting Started with Programming – Part 8: Typedef and Structs](https://www.norwegiancreations.com/2017/10/getting-started-with-programming-part-8-typedef-and-structs/)
 * [github / PaulStoffregen / Time / Arduino Time Library](https://github.com/PaulStoffregen/Time)
 * [github / JChristensen / DS3232RTC](https://github.com/JChristensen/DS3232RTC)
 * [github / ivanseidel / LinkedList](https://github.com/ivanseidel/LinkedList)
 * [Non-contact liquid sensor (XKC-Y25-V) with Arduino](https://alexkychen.wordpress.com/2017/12/03/non-contact-liquid-sensor-xkc-y25-v-with-arduino/)
 * [XKC-Y25-NPN Non-Contact Liquid Level Sensor](http://www.icstation.com/contact-liquid-level-sensor-ip67-waterproof-output-water-level-detector-p-12292.html)
 * [Non-contact Liquid Level Sensor XKC-Y25-T12V SKU: SEN0204](https://www.dfrobot.com/wiki/index.php/Non-contact_Liquid_Level_Sensor_XKC-Y25-T12V_SKU:_SEN0204)
 * [Liquid Level Sensor-FS-IR02 SKU: SEN0205](https://www.dfrobot.com/wiki/index.php/Liquid_Level_Sensor-FS-IR02_SKU:_SEN0205)


# External Libraries
 * [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor)
 * [Adafruit DHT](https://github.com/adafruit/DHT-sensor-library)
 * [Adafruit MotorShield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
 * [Arduino-Temperature-Control-Library](https://github.com/milesburton/Arduino-Temperature-Control-Library)
 * [DS3232RTC](https://github.com/JChristensen/DS3232RTC)
 * [LinkedList](https://github.com/ivanseidel/LinkedList)
 * [OneWire](https://github.com/PaulStoffregen/OneWire)