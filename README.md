## 🐾 Google Assistant Enabled Smart Pet Feeding Management System

### 📘 Overview

This project is an **IoT-based automated pet feeder** built using the **ESP8266 NodeMCU** microcontroller. It allows pet owners to **feed their pets remotely** using **Google Assistant** via the **SinricPro platform**, and it can also operate on **automatic schedules**.

The system controls a **servo motor** to dispense food, uses **ultrasonic sensors** to measure food levels, and displays status updates on an **I2C LCD display**. It also uses an **NTP client** to synchronize time, ensuring feeding happens accurately.

---

### 🎯 Objectives

* Automate the feeding process for pets using IoT.
* Enable **remote control** through Google Assistant (via SinricPro).
* Monitor food levels using **ultrasonic sensors**.
* Display real-time feeding status and food levels on an **LCD screen**.
* Maintain accurate feeding schedules using **time synchronization (NTP)**.

---

### 🧠 System Architecture

1. **ESP8266 NodeMCU** connects to Wi-Fi and communicates with SinricPro servers.
2. **Servo motor** rotates to dispense food for the dog or cat based on commands.
3. **Ultrasonic sensors** detect remaining food levels in the containers.
4. **LCD (I2C)** displays current time, food levels, and feeding status.
5. **SinricPro platform** handles Google Assistant voice commands and app-based control.

---

### ⚙️ Hardware Requirements

| Component                           | Function                                      |
| ----------------------------------- | --------------------------------------------- |
| **ESP8266 NodeMCU**                 | Central microcontroller with Wi-Fi capability |
| **Servo Motor**                     | Dispenses pet food                            |
| **Ultrasonic Sensors (2x HC-SR04)** | Measure food level for each feeder            |
| **I2C LCD Display (16x2)**          | Shows current time and status                 |
| **Breadboard & Jumper Wires**       | Prototyping and connections                   |
| **5V Power Supply / USB Cable**     | Power source for the system                   |

---

### 💻 Software Requirements

| Software / Library                 | Purpose                           |
| ---------------------------------- | --------------------------------- |
| **Arduino IDE**                    | Development and code upload       |
| **ESP8266WiFi.h**                  | Connect NodeMCU to Wi-Fi          |
| **Servo.h**                        | Control the servo motor           |
| **SinricPro.h, SinricProSwitch.h** | Connect with SinricPro cloud      |
| **TimeLib.h**                      | Handle time and scheduling        |
| **WiFiUdp.h, NTPClient.h**         | Sync time with NTP server         |
| **Wire.h, LiquidCrystal_I2C.h**    | I2C communication and LCD display |

---

### 🔧 Circuit Connections

| Component                          | ESP8266 Pin        |
| ---------------------------------- | ------------------ |
| Servo Signal                       | D8                 |
| Ultrasonic Sensor 1 (Trigger/Echo) | D5 / D6            |
| Ultrasonic Sensor 2 (Trigger/Echo) | D7 / D4            |
| LCD (I2C)                          | SDA → D2, SCL → D1 |
| Power                              | 5V / GND           |

---

### 🧩 Setup Instructions

1. **Install Arduino IDE**
   Download and install from [Arduino official site](https://www.arduino.cc/en/software).

2. **Add ESP8266 Board**

   * Go to *File → Preferences → Additional Board Manager URLs*
   * Add:

     ```
     https://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
   * Then go to *Tools → Board → Board Manager* and install **ESP8266 by ESP8266 Community**.

3. **Install Required Libraries**
   In Arduino IDE → *Sketch → Include Library → Manage Libraries…*
   Install the following:

   * `ESP8266WiFi`
   * `Servo`
   * `SinricPro`
   * `TimeLib`
   * `WiFiUdp`
   * `NTPClient`
   * `Wire`
   * `LiquidCrystal_I2C`

4. **Configure Wi-Fi and SinricPro Credentials**
   In the code (`prog9.ino`), update:

   ```cpp
   #define WIFI_SSID "your_wifi_name"
   #define WIFI_PASSWORD "your_wifi_password"
   #define APP_KEY "your_sinricpro_app_key"
   #define APP_SECRET "your_sinricpro_app_secret"
   ```

   Also replace the **device IDs** with your own from the SinricPro dashboard.

5. **Upload the Code**

   * Select **Tools → Board → NodeMCU 1.0 (ESP-12E Module)**
   * Select correct **Port**
   * Click **Upload**

6. **Test via SinricPro App or Google Assistant**

   * Say: *“Hey Google, feed the dog”* or *“Check cat food level.”*
   * Observe servo rotation and LCD updates.

---

### 🕒 Automatic Feeding Schedule

| Time         | Action            |
| ------------ | ----------------- |
| **08:00 AM** | Morning feeding   |
| **01:00 PM** | Afternoon feeding |
| **06:00 PM** | Evening feeding   |

All schedules reset automatically at midnight to ensure proper triggering for the next day.

---

### 📟 LCD Display Info

| Display               | Meaning                                               |
| --------------------- | ----------------------------------------------------- |
| “Time: HH:MM”         | Current synchronized time                             |
| “Feeding the dog/cat” | Feeding action in progress                            |
| “D1: XXcm D2: YYcm”   | Distance measured by ultrasonic sensors (food levels) |
| “Feeding done.”       | Action completed                                      |

---

### 🧪 Testing & Validation

* Tested servo rotation for dispensing accuracy.
* Validated ultrasonic sensor readings for food level detection.
* Verified NTP time synchronization accuracy.
* Checked remote control via **SinricPro + Google Assistant** integration.
* Confirmed LCD displays correct and real-time feedback.

---

### 🧾 Results & Conclusion

This IoT-based pet feeder successfully automates pet feeding through scheduled and remote operations.
It ensures:

* Accurate and timely feeding.
* Real-time food monitoring.
* Remote voice-controlled operation.

The system is **low-cost, scalable, and user-friendly**, demonstrating how IoT can simplify pet care.

---

### 🧑‍💻 Author

**Tharun B L Ganapathy**
Department of Computer Science & Engineering
CIT, Ponnampet (2023–2024)

---

### 📄 License

This project is open-source under the **MIT License**.
You’re free to modify, share, and build upon it with attribution.

Would you like me to include **a section for cloning and running the project (like `git clone` + directory setup + uploading .ino)** so others can directly try it on their ESP8266? I can append that too neatly.
