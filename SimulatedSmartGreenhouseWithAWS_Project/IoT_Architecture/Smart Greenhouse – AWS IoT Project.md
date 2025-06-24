# 🌱 Smart Greenhouse – AWS IoT Project

A serverless IoT-based system to monitor real-time environmental data from a smart greenhouse using AWS services.

---

## Architecture Overview

![IoT FinalArchitecture.drawio2.svg](https://github.com/Smart-Green-houses/SmartGreenHouse/blob/master/AWS-Project/IoT_Architecture/Architecture/IoT%20FinalArchitecture.drawio2.svg)

> Flow Summary:
> 
- Sensors send data (Temp, Humidity, Light, etc. ) and images from ESP32 device.
- Data is published to **AWS IoT Core** via MQTT.
- Store images in **S3 Bucket**.
- **AWS Lambda** is triggered to:
    - Save data in **Amazon Timestream**.
    - Trigger **SNS** alerts if thresholds are exceeded.
- **Another Lambda Function** is exposed via **API Gateway** to provide frontend data for a Website and WebApp.

---

## Architecture Components

- **ESP32 (MQTT Publisher)**
    
    Microcontroller used to collect data from environmental sensors (temperature, humidity, soil, etc.) and send it via MQTT protocol to AWS IoT Core. It also sends base64-encoded images.
    
- **AWS IoT Core**
    
    Acts as the entry point for incoming sensor data. It receives MQTT messages and routes them using **IoT Rules** to trigger Lambda functions or store data.
    
- **AWS Lambda**
    - Parses incoming IoT data.
    - Stores data in Amazon Timestream.
    - Triggers email alerts using SNS.
    - Sends data from Amazon Timestream to API Gateway.
- **Amazon Timestream**
    
    Time-series database used to store and query the latest environmental data like temperature, humidity, soil stats, etc.
    
- **Amazon Simple Notification Services (SNS)**
    
    Sends automatic email alerts when specific thresholds are exceeded (e.g., temperature > 30°C or humidity < 50%).
    
- **Amazon S3 (for images)**
    
    Stores image files that are sent from the ESP32 by IoT core in base64 format.
    
- **API Gateway (HTTP API)**
    
    Exposes a RESTful `GET /data` endpoint which is connected to a Lambda function. The WebApp fetches real-time greenhouse data from this API.
    
- **IAM Roles & Policies**
    
    Securely manage permissions for each service:
    
    - Lambda can write to Timestream and SNS.
    - IoT Core can invoke Lambda.
    - API Gateway can trigger Lambda securely.
- **WebApp (HTML, JS)**
    
    A simple frontend (website) that fetches data from the API and displays real-time greenhouse sensor values, possibly using Chart.js or plain HTML.
    

---

## API Gateway Endpoint

- **Endpoint**: `GET /data`
- **Returns**: Last record from Timestream to WebApp.
- **Example Output:**

```json
{
  "temperature": "27.8",
  "humidity": "55.2",
  "light": "14000",
  "tds": "600",
  "soil_moisture": "50.2",
  "soil_nitrogen": "90",
  "soil_phosphorus": "40",
  "soil_potassium": "85",
  "time": "2025-06-18T10:22:00Z"
}

```
