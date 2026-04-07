# MostControl
This is a universal system that can monitor bridge structures, overpasses, hydraulic structures, mines, and power facilities.

## Features:
My system is unique in that it reduces the cost of other systems,
because Arduino is a cheap and stable component, while similar systems are very, VERY expensive (ranging from 2 to tens of millions).
And even when transferring these sensors to a real bridge (meaning, selecting sensors for the specific bridge parameters) the price will change little compared to similar systems.
The system also has a Telegram bot to which emergency alerts are sent.

## System components:
Arduino UNO
ESP32
DC-DC converter
Potentiometer
LCD I2C display
7 LEDs - yellow, green, red
Flexible sensor
MPU6050
HX711
HX711
Piezo buzzer
Humidity sensor
Motion sensor
Servo motor
## System logic:
The system primarily operates on the ultrasonic sensor; all other sensors are confirmatory. The sensors collect data and then feed it to the controller (Arduino), which compares and displays the reading: critical, pre-critical, or normal. It also works with the motion sensor; if water reaches the sensor (flooding the bridge), the bridge level is critical. At a critical level, a red light is illuminated and a beep is sent to the bot. If the motion sensor detects movement (indicating that all vehicles have left the bridge), the barrier closes. At a pre-critical level, a yellow light is displayed, a message is sent to the bot, and a beep is emitted. I didn't model the bridge itself because that project preceded the Parking Sensor project, and I didn't know how to model it back then. Now I'm pretty good at modeling. I have a patent on the EVM code for this system.

## Prospects:
In the future, I want to replace the motion sensor with an ESP32 camera, so it can track vehicles on the road.

## Photo Products:
https://github.com/user-attachments/assets/7ced11a1-9a1e-4262-b2aa-e0df4f15f86f
https://github.com/user-attachments/assets/86ebec16-ee96-41d5-987d-9a42a194b60b
https://github.com/user-attachments/assets/d640cfb4-a7dd-42f2-b2c7-ab6605e9e89c
https://github.com/user-attachments /assets/2e05a6e7-e55f-460c-85dd-882c00ca0ef9
https://github.com/user-attachments/assets/4824e4fc-7fc6-4282-98c6-18ee5775a734
https://github.com/user-attachments/assets/2db2289f-9f86-40bb-a9b7-2e24e740ad25
https://github.com/user-attachments/assets/57a8c874-9916-48a8-bd09-f5e1c3245b17
