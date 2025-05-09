# Heater-Control-System

This project simulates a smart heater control system using an ESP32 microcontroller and the DHT22 temperature sensor. It includes temperature-based state transitions, audio-visual indicators, BLE status broadcasting, and a simulated heating & cooling mechanism.

## Features

- Temperature monitoring using DHT22 sensor
- Five-state temperature-based state machine:
  - IDLE
  - HEATING
  - STABILIZING
  - TARGET_REACHED
  - OVERHEAT
- Heater control logic (simulated with an LED)
- RGB LED to indicate current system state
- Buzzer alerts:
  - One-time beep when target temperature is reached
  - Continuous buzzer when overheat condition is detected
- Cooling mechanism simulation (e.g., fan indicator LED)
- BLE advertising using ESP32 to broadcast:
  - Current temperature
  - Heater ON/OFF status
  - Current system state
- Serial logging with temperature, system state, heater status, and uptime

## Hardware Components (Simulated or Real)

- ESP32
- DHT22 Temperature & Humidity Sensor (4.7k ohm Pull-up Resistor Recommended)
- RGB LED (Red, Green, Blue pins separately controlled)
- Active Buzzer
- Heater (simulated using an LED on GPIO 5)
- Cooling mechanism (simulated using an LED on GPIO 18)

## Pin Configuration

-DHT22 Data Pin   | GPIO 15 
-Heater LED       | GPIO 5  
-Buzzer           | GPIO 19 
-RGB Red          | GPIO 21 
-RGB Green        | GPIO 22 
-RGB Blue         | GPIO 23 
-Cooling LED      | GPIO 18 

## Temperature Thresholds

- Target Temperature: 35°C
- Margin: ±1.5°C
- Overheat Temperature: 45°C

## BLE Advertisement Payload

The ESP32 advertises a short payload every 5 seconds containing:
- Temperature
- Heater status (On/Off)
- Current state


## States and Visual Indication (RGB LED)

-IDLE             | Green           
-HEATING          | Yellow (Red+Green) 
-STABILIZING      | Cyan (Green+Blue)  
-TARGET_REACHED   | Blue            
-OVERHEAT         | Red             

## Note

- BLE does not simulate in Wokwi; logic is preserved and can be tested on a real ESP32 using tools like the nRF Connect app.
- System will automatically enter `OVERHEAT` state if temperature exceeds 45°C.
- The buzzer and cooling mechanism are simulated and visually indicated in Wokwi.
- The system logs temperature, state, heater status, and uptime every second to Serial.


![image](https://github.com/user-attachments/assets/826675a9-5d96-4aa1-9560-5fea51a6a790)

