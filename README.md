# Heater Control System (Arduino with Timer Interrupt)

### Overview
This project simulates a basic heater control system using Arduino, DS18B20 temperature sensor, and a relay.  
It uses a **Timer1 interrupt** for periodic temperature sampling and a **state machine** to manage heater states:
Idle → Heating → Stabilizing → Target Reached → Overheat.

An LED simulates the heater, and a buzzer alerts when the target temperature is reached.  
All data is logged via UART to the Serial Monitor.

---

### Features
- **State Machine**: Idle, Heating, Stabilizing, Target Reached, Overheat  
- **Non-blocking Design**: Timer1 interrupt instead of `delay()`  
- **Visual & Audible Feedback**: LED as heater indicator, buzzer for alerts  
- **Serial Logging**: Temperature and system state via UART  

---

### Hardware
- Arduino Uno  
- DS18B20 Temperature Sensor (OneWire protocol)  
- Relay Module (controls simulated heater)  
- LED (simulated heater indicator)  
- Buzzer  

---

### Future Enhancements
- Overheat lockout with manual reset  
- Multiple heating profiles  
- Send data to a **mobile app** using MQTT/Wi-Fi  
- FreeRTOS-based task scheduling  

---

### How to Run
1. Open `src/heater_control.ino` in Arduino IDE.  
2. Connect DS18B20 to pin 8, relay to pin 4, LED to pin 11, and buzzer to pin 10.  
3. Upload the code to your Arduino.  
4. Open Serial Monitor at **9600 baud** to view logs.  

---

### Diagrams
- **Block Diagram**: `diagrams/block_diagram.drawio`  
- **State Diagram**: `diagrams/state_diagram.drawio`  

---

### Simulation
https://wokwi.com/projects/437187864243347457
