# Arduino implementation for Jarvis-Arduino-Json project

## What is it?

This arduino implementation is designed to send temperature/hygrometer/light/human detection data from arduino board (with ethernet shield) to the (local) web on JSON format.

<img src="https://github.com/QuentinCG/jarvis-arduino-json/blob/master/arduino/images/web_browser_result.png">


## How to install

1) Download those libraries : <a target="_blank" href="https://github.com/QuentinCG/Arduino-LM75A-Temperature-Sensor-Library">LM75A temperature sensor</a>, 
<a target="_blank" href="https://github.com/QuentinCG/Arduino-Hygrometer-Sensor-Library">hygrometer sensor</a>,
<a target="_blank" href="https://github.com/QuentinCG/Arduino-Light-Dependent-Resistor-Library">light dependent resistor sensor</a>,
<a target="_blank" href="https://github.com/QuentinCG/Arduino-Digital-Sensor-Library">digital sensor (for PIR sensor)</a> and
<a target="_blank" href="https://github.com/bblanchon/ArduinoJson">JSON</a>.

2) On your Arduino IDE, click "Sketch" menu and then "Include Library > Add .ZIP Libraries" and add all those libraries

3) You can now compile the <a target="_blank" href="https://github.com/QuentinCG/jarvis-arduino-json/blob/master/arduino/arduino_sensors_to_json/arduino_sensors_to_json.ino">arduino source code</a> with Arduino IDE.


## How to connect the sensors and ethernet board to your Arduino

<img src="https://github.com/QuentinCG/jarvis-arduino-json/blob/master/arduino/images/schematics.png" width="400">


LMR Sensor with arduino : Check schematics


Ethernet shield with arduino : Check schematics


LM75A sensor | Arduino
-------- | --------
VCC      | 3.3V
GND      | GND
SDA      | A4 (SDA)
SCL      | A5 (SCL)
A0      | GND (or VCC to increase I2C address of 1)
A1      | GND (or VCC to increase I2C address of 2)
A2      | GND (or VCC to increase I2C address of 4)
OS      | None


Hygrometer Sensor  | Arduino
-------- |  --------
VCC      | 5V
GND      | GND
A0       | A1 (or any other analog pin)


PIR Sensor  | Arduino
-------- |  --------
VCC      | 5V
GND      | GND
D0       | D2 (or any other digital pin)


## License

This project is under MIT license. This means you can use it as you want (just don't delete the library header).


## Contribute

If you want to add more examples or improve the library, just create a pull request with proper commit message and right wrapping.
