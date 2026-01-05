🚧 Pipe Inspection Robot using ESP32 & Blynk IoT
📌 Project Overview

This project implements a WiFi-controlled Pipe Inspection Robot using an ESP32 microcontroller and the Blynk IoT platform.
The robot is designed to operate in confined environments such as pipelines, where it can:

Be remotely controlled in real time

Detect obstacles ahead

Monitor gas leakage

Measure temperature and humidity

Alert users instantly via buzzer and Blynk notifications

This system is suitable for industrial inspection, safety monitoring, and smart robotics applications.

🎯 Key Features

📱 Remote control via Blynk mobile app

🚗 Bidirectional motor control (Forward, Backward, Left, Right)

📏 Obstacle detection using Ultrasonic Sensor

🚨 Gas leakage detection using MQ-4 sensor

🌡️ Temperature & humidity monitoring using DHT11

🔔 Buzzer alert for nearby obstacles

☁️ Real-time data visualization on Blynk dashboard

📢 Instant event notifications for gas leak & obstacle detection

🧠 System Architecture
Blynk Mobile App
       |
   WiFi (IoT)
       |
     ESP32
 ┌─────┼───────────┐
 │     │           │
Motors Sensors   Alerts
 │     │           │
L298N DHT11     Buzzer
      MQ-4
   Ultrasonic

🔧 Hardware Components

ESP32 Development Board

L298N Motor Driver

DC Motors (Left & Right)

Ultrasonic Sensor (HC-SR04)

DHT11 Temperature & Humidity Sensor

MQ-4 Gas Sensor

Buzzer

Power Supply / Battery

Robot Chassis & Wheels

🖥️ Software & Tools

Arduino IDE

ESP32 Board Package

Blynk IoT Platform

WiFi Communication

📲 Blynk Virtual Pin Mapping
Function	Virtual Pin
Forward	V0
Backward	V1
Left	V2
Right	V3
Temperature	V4
Humidity	V5
Distance	V6
Gas Value	V7
Gas Alert LED	V8
⚙️ Working Principle
🔹 Motor Control

The robot uses two DC motors controlled through an L298N motor driver.

Direction is controlled via ESP32 GPIO pins.

Commands are received from Blynk buttons.

🔹 Obstacle Detection

Ultrasonic sensor measures distance ahead.

If an object is detected within 15 cm:

Buzzer is activated

Blynk event notification is sent

🔹 Gas Leakage Detection

MQ-4 sensor continuously monitors gas concentration.

If gas value exceeds threshold:

Blynk alert is triggered

Warning LED turns ON in the app

🔹 Environmental Monitoring

DHT11 sensor measures temperature and humidity.

Data is updated every 2 seconds on Blynk dashboard.

🧪 Safety & Alerts

🔔 Buzzer warns for close obstacles

🚨 Gas leakage triggers instant cloud notification

📱 Real-time monitoring from anywhere

📸 Output & Monitoring

Live robot movement from Blynk app

Sensor data visualization (gauges & LEDs)

Event notifications on mobile

🚀 Future Enhancements

Camera integration for visual inspection

Autonomous navigation using PID control

Data logging to cloud server

Battery level monitoring

AI-based anomaly detection

👨‍💻 Author

Jusaina Jamal
Engineering Graduate | Robotics & IoT Enthusiast
Project: Pipe Inspection Robot
