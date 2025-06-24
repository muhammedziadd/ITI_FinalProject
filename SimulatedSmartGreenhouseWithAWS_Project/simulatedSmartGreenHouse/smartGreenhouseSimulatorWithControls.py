import json
import random
import time
from datetime import datetime
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

# AWS IoT Core Configuration
CLIENT_ID = "SmartGreenhouseSimulator"
ENDPOINT = "a37ue7489als1o-ats.iot.us-west-2.amazonaws.com"  # <-- Replace this
PORT = 8883
DATA_TOPIC = "greenhouse/data"
CONTROL_TOPIC = "greenhouse/control"

# Certificates
ROOT_CA = "AmazonRootCA1.pem"
PRIVATE_KEY = "private.pem.key"
CERTIFICATE = "certificate.pem.crt"

# Device status (fan/pump)
device_status = {
    "fan": "off",
    "pump": "off"
}

# Setup MQTT Client
mqtt_client = AWSIoTMQTTClient(CLIENT_ID)
mqtt_client.configureEndpoint(ENDPOINT, PORT)
mqtt_client.configureCredentials(ROOT_CA, PRIVATE_KEY, CERTIFICATE)
mqtt_client.configureOfflinePublishQueueing(-1)
mqtt_client.configureDrainingFrequency(2)
mqtt_client.configureConnectDisconnectTimeout(10)
mqtt_client.configureMQTTOperationTimeout(5)

# Connect
print("Connecting to AWS IoT Core...")
mqtt_client.connect()
print("Connected.")

# Callback for control topic
def control_callback(client, userdata, message):
    print(f"[Control] Received command: {message.payload.decode()}")
    try:
        control_msg = json.loads(message.payload.decode())
        if "fan" in control_msg:
            device_status["fan"] = control_msg["fan"]
        if "pump" in control_msg:
            device_status["pump"] = control_msg["pump"]
        print(f"[Status] Fan: {device_status['fan']}, Pump: {device_status['pump']}")
    except Exception as e:
        print(f"[Error] Invalid control message: {e}")

# Subscribe to control topic
mqtt_client.subscribe(CONTROL_TOPIC, 1, control_callback)

# Simulate sensor data
def generate_sensor_data():
    return {
        "temperature": round(random.uniform(18, 35), 2),     # °C
        "humidity": round(random.uniform(40, 80), 1),        # %
        "light": random.randint(1000, 20000),                # Lux
        "soil_moisture": random.randint(20, 60),             # %
        "tds": random.randint(200, 800),                     # ppm
        "soil_npk": {
            "nitrogen": random.randint(50, 150),             # mg/kg
            "phosphorus": random.randint(20, 80),            # mg/kg
            "potassium": random.randint(40, 120)             # mg/kg
        },
        "devices": device_status.copy(),
        "timestamp": datetime.utcnow().isoformat() + "Z"
    }

# Publish loop
try:
    while True:
        payload = generate_sensor_data()
        json_payload = json.dumps(payload)
        mqtt_client.publish(DATA_TOPIC, json_payload, 1)
        print(f"[Data] Published: {json_payload}")
        time.sleep(300)  # Wait 5 minutes
except KeyboardInterrupt:
    print("Stopping simulator.")
    mqtt_client.disconnect()
