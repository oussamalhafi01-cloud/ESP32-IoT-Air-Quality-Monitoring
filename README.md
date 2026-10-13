# ESP32 IoT Air Quality Monitoring System

An IoT-based air quality monitoring project developed using ESP32, C++ and MQTT as part of a university practical course.

The system measures air quality indicators and communicates sensor data over a network using the MQTT protocol.

## Features

- **Air Quality Monitoring:** Measurement of eCO₂ and TVOC values.
- **Real-Time Data Transmission:** Communication using Wi-Fi and MQTT.
- **Sensor Integration:** Collection and processing of environmental sensor data.
- **Visual Feedback:** Display of air quality measurements and status indicators.
- **MQTT Communication:** Exchange of sensor data and control messages.
- **Home Assistant Integration:** Support for MQTT-based device discovery and monitoring.

## Technologies Used

- ESP32 / M5StickC
- C++
- Python
- PlatformIO
- MQTT
- Wi-Fi
- Home Assistant
- Environmental Sensors

## Project Structure

```text
src/
├── main.cpp
└── discovery.py

platformio.ini
README.md
```

## Requirements

- Visual Studio Code
- PlatformIO Extension
- Compatible ESP32 / M5StickC hardware
- Required sensors and libraries
- Wi-Fi network
- MQTT broker

## Setup

1. Clone or download this repository.
2. Open the project in VS Code with PlatformIO.
3. Install the required dependencies.
4. Configure the Wi-Fi and MQTT connection settings.
5. Build and upload the firmware to the ESP32.
6. Connect the required hardware components.

**Note:** The project requires physical hardware. Network credentials are replaced with placeholders for security. Additional sensor library configuration may be required.

## Project Context

Developed as part of *Fachpraktikum II* during an engineering-related university course.

The project focuses on embedded programming, network communication, sensor integration and IoT system development.
