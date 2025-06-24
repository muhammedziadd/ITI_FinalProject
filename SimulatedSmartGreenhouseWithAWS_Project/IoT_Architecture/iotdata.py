import paho.mqtt.client as mqtt
import ssl
import json
import time
import random

broker_address = "agagh2vl598fr-ats.iot.us-west-2.amazonaws.com"
port = 8883
topic = "greenhouse/data"

ca_path = "AmazonRootCA1.pem"
cert_path = "certificate.pem.crt"
key_path = "private.pem.key"

client = mqtt.Client()
client.tls_set(ca_certs="AmazonRootCA1.pem",
               certfile="certificate.pem.crt",
               keyfile="private.pem.key",
               tls_version=ssl.PROTOCOL_TLSv1_2)

client.connect(broker_address, port)

while True:
   
    temperature = round(random.uniform(20.0, 35.0), 2)
    humidity = round(random.uniform(40.0, 80.0), 2)
    light= round(random.uniform(1000, 20000), 2)              
    tds= round(random.uniform(200, 800),2)                    
    soil_moisture= round(random.uniform(40.0, 80.0), 2)
    soil_nitrogen= round(random.uniform(50, 150), 2)
    soil_phosphorus= round(random.uniform(20, 80), 2)
    soil_potassium= round(random.uniform(40, 120), 2) 
    

    payload = {
        "temperature": temperature,
        "humidity": humidity,
        "light": light,
        "tds":tds,
	    "soil_moisture":soil_moisture,
        "soil_nitrogen":soil_nitrogen,
        "soil_phosphorus":soil_phosphorus,
        "soil_potassium":soil_potassium
    }

    client.publish(topic, json.dumps(payload))
    print(f"Sent: {payload}")
    time.sleep(5)