# Aquarium Controller

## Warning
This is work in progress. So far, the project has never been build and tested.

## Abstract
This project is for the joy of building a multi functional aquarium controller. 
From a financial perspective, probably it is more feasible to buy a fabricated product.

## Desired Components
- Auto Top Off Station (ATO)
- Ambient Control Station (heating & cooling)
- Dosing Station
- Alarm Station

## Global Requirements
- Stations should be independent enough to be built and implemented separately.
- Code should support interchangeable external libraries as much as possible.
- Code should be reusable and platform (Arduino, ESP, STM, ...) independent as much as possible. 

### ATO Station Requirements
- mandatory, support up to 4 liquid level sensors:
    - optional, high level sensor
    - mandatory, normal level sensor
    - optional, low level sensor
    - optional, reservoir low sensor
- mandatory, can control a dispensing device like a pump, electronic valve, ...
- optional, sleep/wake
- optional, raise/clear alarms
- optional, expose its value (sound, led, LDC, network, ...)  

### Dosing Station Requirements
- mandatory, can operate 1 - 4 dosing pumps
- optional, can be extended to operate up to 16 dosing pumps

### Ambient Control Station
- optional, can control a water heating device
- optional, can control a water cooling device
- optional, can control an ambient cooling device

## Hardware

### ATO Station
    Arduino Nano, quantity: 1
    6V-12V DC Brush-less Small Water Pump Submersible Motor Pump 1.8M 280L/H, quantity: 1
    water level sensor, quantity: 3
    1 relay from the above relay module

### Ambient Control Station
    Arduino Nano, quantity: 1
    DHT22 Temperature and Humidity Sensor, quantity: 1
    2 meters Thermometer Waterproof Cable Probe DS18B20, quantity: 1
    DS18B20 Shield (no sensor) or DS18B20 Module (with sensor), quantity: 1
    5V 4-Channel Relay Module, quantity: 1

### Dosing Station
     Arduino Mega, quantity: 1
     I2C TB6612 Stepper Motor PCA9685 Servo Driver Shield V2, quantity: 1 min, 4 max
     DS3231 RTC module, quantity: 1
     DC12V/24V Peristaltic Dosing pump, quantity: 1 to 16
     Infra-red IR Wireless Remote Control Module Kits DIY Kit HX1838 
     I2C 2004 SERIAL 20X4 LCD MODULE
     KY-012 Active Buzzer Module for Arduino
     MB102 Solder-less Breadboard
     20cm DuPont jumper wires female-female, quantity: set of 10 or more
     20cm DuPont jumper wires male-female, quantity: set of 10 or more
     20cm DuPont jumper wires male-male, quantity: set of 10 or more
     CR2032 3 Volt Coin Button Cell Battery 
     Power adapter 12V, min 1A*, quantity: 1 (*amps = (numberOfDosingPumps+1) * dosingPumpAmps)

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


## External Libraries
 * [Adafruit Sensor](https://github.com/adafruit/Adafruit_Sensor)
 * [Adafruit DHT](https://github.com/adafruit/DHT-sensor-library)
 * [Adafruit MotorShield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
 * [Arduino-IRremote](https://github.com/z3t0/Arduino-IRremote)
 * [Arduino-Temperature-Control-Library](https://github.com/milesburton/Arduino-Temperature-Control-Library)
 * [DS3232RTC](https://github.com/JChristensen/DS3232RTC)
 * [LinkedList](https://github.com/ivanseidel/LinkedList)
 * [OneWire](https://github.com/PaulStoffregen/OneWire)

## Acknowledgments
 * [Adafruit Industries](https://www.adafruit.com)
 * [Jetbrains CLion](https://www.jetbrains.com/clion/specials/clion/clion.html)
 * [Platform IO](https://platformio.org/)
 * [Arduino Forum](https://forum.arduino.cc)
 * [Stack Overflow](https://stackoverflow.com)
 * [Microsoft Visual Studio Code](https://code.visualstudio.com/)
 * [GitHub](https://github.com)
 * [AliExpress](https://www.aliexpress.com)

# Disclaimer
The author provides no warranty about the completeness, reliability and stability of this software. The author is not liable for any losses or damage obtained from the use of this software.
