
# Smart IoT Camera & LED Control System with AWS IoT Core

## 📖 Project Overview

This project uses an ESP32-CAM module connected to AWS IoT Core via MQTT to capture images, upload them to AWS services, process them using Lambda functions, and control physical LEDs based on image analysis.

---

## 📐 System Architecture

**Devices & Services involved:**
- **ESP32-CAM**: Captures images and sends them via MQTT.
- **AWS IoT Core**: Acts as the MQTT broker and message router.
- **IoT Rules**:
  1. **Image Upload Rule**: Triggers a Lambda when image is published to `esp32-image` topic, saves image to **S3**.
  2. **LED Control Rule**: Sends analyzed LED color result from Lambda to `led-color-topic` for the ESP32 to subscribe to and act upon.
  3. **Emergency Detection Rule**: If image analysis detects abnormal brightness, publishes a message to `ESP32/Emergency` topic, and triggers both an SNS notification and saves event to **DynamoDB**.

- **AWS Lambda**: Processes images and determines LED color.
- **Amazon S3**: Stores received images.
- **Amazon SNS**: Sends alerts for emergency cases.
- **Amazon DynamoDB**: Logs emergency events.

---

## 🖥️ ESP32 Workflow

- Connect to WiFi.
- Sync time (for SSL/TLS certs).
- Connect to AWS IoT Core with secure credentials.
- Every **5 minutes**:
  - Capture image via onboard camera.
  - Encode image in Base64.
  - Send to `esp32-image` MQTT topic.
  - Wait for response on `led-color-topic` to light up the specified LED color until next image capture.

---

## 📊 MQTT Topics

| Topic               | Role                                              |
|:--------------------|:--------------------------------------------------|
| `esp32-image`        | ESP32 publishes captured image in Base64 JSON.    |
| `led-color-topic`    | Lambda publishes result color for ESP32 LEDs.     |
| `ESP32/Emergency`    | Lambda publishes emergency events if detected.    |

---

## 📌 Issues Faced & Solutions

- **ESP32 not capturing image feedback**:  
  🛠️ Added debug serial prints inside the capture function to confirm image capture or error.

- **MQTT disconnection after publish**:  
  🛠️ Added reconnect logic inside loop when disconnected.

- **Time sync delay for SSL certs**:  
  🛠️ Added a blocking loop with time check until `time(nullptr)` synced.

- **ESP32 not receiving LED topic message**:  
  🛠️ Used `client.onMessage()` callback in `setup()` and `client.subscribe()` to the topic.

- **Delayed image sending after connection**:  
  🛠️ Moved initial `captureAndSendImage()` call after MQTT connection confirmation.

- **AWS IoT Rule misfire on wrong topics**:  
  🛠️ Double-checked topic names and attached correct Lambda actions.

- **No notification on emergency detection**:  
  🛠️ Verified SNS topic subscription email endpoint and permissions.

---

## 📦 To-Do / Improvements
- Optimize image size before base64 encoding.
- Add retry mechanism on failed publish attempts.
- Build front-end dashboard for real-time event monitoring.

---

## ✅ Status
Fully functional with image capture, cloud processing, LED control, emergency detection, and cloud alerting.

---

© 2025 Ranon | Embedded IoT & Cloud Integration Lab
