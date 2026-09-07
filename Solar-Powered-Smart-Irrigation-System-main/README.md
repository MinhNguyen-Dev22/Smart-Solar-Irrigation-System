# Solar-Powered-Smart-Irrigation-System
An ESP8266-based solar-powered smart irrigation system with real-time environmental monitoring, Firebase integration, and automatic irrigation control.

![ESP8266](https://img.shields.io/badge/ESP8266-IoT-blue)
![Blynk](https://img.shields.io/badge/Blynk-IoT-green)
![Firebase](https://img.shields.io/badge/Firebase-Realtime%20Database-orange)
![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen)

> An ESP8266-based solar-powered smart irrigation system featuring automatic watering, environmental monitoring, Firebase cloud storage, and remote control through the Blynk IoT mobile application.

<img width="669" height="431" alt="image" src="https://github.com/user-attachments/assets/54f91281-cff6-49a1-b4b3-ab8ee1f606d4" />

# Table of Contents
1. Introduction
2. Features
3. Hardware
4. Software
5. Getting Started
6. Results
7. Future Work
8. Contact
9. License

# 1. Introduction

This project presents a solar-powered smart irrigation system designed to automatically monitor environmental conditions and irrigate plants only when necessary.

The system is built around the ESP8266 microcontroller and integrates multiple sensors including soil moisture, temperature, humidity, light intensity, and CO₂ concentration. Sensor data are uploaded to Firebase Realtime Database while users can remotely monitor and manually control the irrigation process through the Blynk IoT mobile application.

The primary objective of this project is to reduce water consumption, enable renewable-energy-powered agriculture, and provide a low-cost IoT solution suitable for home gardens and small-scale farming.
# 2. Features

🌞 Solar Powered
- Operates entirely from a photovoltaic charging system with rechargeable Li-ion batteries.

💧 Automatic Irrigation
- Automatically activates the water pump based on soil moisture conditions.

📱 Remote Monitoring
- Monitor environmental parameters and manually control the irrigation system using the Blynk IoT mobile application.

☁️ Cloud Data Logging
- Uploads sensor data to Firebase Realtime Database for cloud storage.

🌡️ Environmental Monitoring
- Measures temperature, humidity, light intensity, soil moisture, and CO₂ concentration in real time.

🔋 Battery Monitoring
- Estimates remaining battery voltage using a voltage divider circuit.

🛠️ Open Source
- Complete hardware design, firmware source code, PCB files, and documentation are included.

# 3. Hardware

## 3.1 BOM
| Component | Qty |
| --------- | --- |
| Solar panel 6v 10W | 1 |
| ESP8266 | 1 |
| DHT11 | 1 |
| 18650 (4200mAh) | 3 |
| TP4056 | 1 |
| Relay | 1 |
| XL6009 | 2 |
| LCD1602 | 1 |
| Pump | 1 | 

## Energy Consumption Analysis

The estimated daily energy consumption of the system is summarized in the table below.

| Component | Operating Time | Power | Daily Energy Consumption |
| :--- | :---: | ---: | ---: |
| ESP8266 + LCD + DHT11 | 24 hours | ≈ 1.2 W | 1.2 × 24 = **28.8 Wh** |
| Water Pump (12 V, 6 W) | 5 cycles × 10 s = 50 s/day | 6 W | 6 × (50 / 3600) ≈ **0.085 Wh** |
| **Total** | | | **≈ 28.885 Wh/day** |

Assuming the **XL6009 boost converter** has an efficiency of approximately **90%**, the total energy drawn from the battery per day is

```text
E_battery = 28.885 / 0.9 ≈ 32 Wh/day
```

---

## Solar Energy Estimation

The system uses a **6 V, 10 W solar panel**.

Assuming approximately **6 hours of effective sunlight** per day, the theoretical harvested energy is

```text
10 W × 6 h = 60 Wh/day
```

Considering practical conditions such as sunlight angle, dust, and weather, the actual solar panel efficiency is assumed to be approximately **70%**.

```text
Effective solar energy = 60 × 0.7 = 42 Wh/day
```

After accounting for the charging efficiency of the **TP4056 charging module** (approximately **90%**), the actual energy stored in the battery becomes

```text
42 × 0.9 ≈ 37 Wh/day
```

Therefore, the daily energy surplus is

```text
37 Wh − 32 Wh ≈ 5 Wh/day
```

This surplus compensates for battery losses and helps maintain the battery at a safe operating level.

---

## Battery Backup Estimation

The system is powered by **three 18650 Li-ion batteries connected in parallel**.

- Battery capacity (per cell): **4200 mAh**
- Nominal voltage: **3.7 V**

Therefore, the total stored battery energy is

```text
E = 3.7 × 12.6 ≈ 46.6 Wh
```

During nighttime or in the absence of solar energy, the system is powered solely by the battery pack.

The estimated battery backup time is

```text
Battery Backup Time = 46.6 Wh / 32 Wh/day
                    ≈ 1.456 days
                    ≈ 35 hours
```

## 3.2 Block Diagram

<img width="671" height="516" alt="image" src="https://github.com/user-attachments/assets/1e1a40fe-1b07-475a-9774-b566083ccca7" />

## 3.3 Schematic 

<img width="833" height="532" alt="image" src="https://github.com/user-attachments/assets/52f880c4-d74b-4bec-aaa0-a83580f1fd0d" />

# 4. Software

<img width="420" height="648" alt="image" src="https://github.com/user-attachments/assets/310ed8e9-b935-4fc0-ae00-3dde36f608d2" />



# 5. Getting Started

## Software Requirements

- Arduino IDE
- ESP8266 Board Package
- Blynk Library
- Firebase ESP Client
- DHT Library
- ArduinoJson

## Blynk Configuration
### Configure Blynk
<img width="286" height="635" alt="image" src="https://github.com/user-attachments/assets/f8767e9a-fac0-4de3-a2f7-bc4fa29f9d22" />

1. Create a Template on Blynk IoT.

2. Create Datastreams.

3. Copy

- Template ID
- Device Name
- Authentication Token

4. Replace them in
```cpp
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""
```

# 6. Results
**Battery Backup Test:** The system was experimentally tested without solar charging and achieved approximately **32 hours of continuous operation** using only the battery pack.

**Long-term Stability Test:** The system was continuously evaluated for **two weeks** under normal operating conditions and demonstrated stable performance in environmental monitoring, scheduled irrigation, and wireless communication.


# 7. Future Work

- Integrate weather forecast APIs for predictive irrigation.
- Add machine learning models for irrigation optimization.
- Support multiple irrigation zones.
- Improve battery charging efficiency using MPPT algorithms.
- Develop a web dashboard in addition to the Blynk mobile application.

# 8. Contact
👤 Huynh Thanh Phuong

📧 phuong0342098446@gmail.com

💻 https://github.com/phuonght098

🔗 LinkedIn: https://www.linkedin.com/in/phuonght098/

If you find this project useful, please consider giving it a ⭐ on GitHub.

# . License
This project is licensed under the MIT License.

See the LICENSE file for details.
