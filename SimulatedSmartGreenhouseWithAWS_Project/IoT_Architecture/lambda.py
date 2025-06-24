import boto3
import time
import json

timestream = boto3.client('timestream-write')
sns = boto3.client('sns')

DB_NAME = 'SmartGreenHouseDB'
TABLE_NAME = 'SmartGHTable'
SNS_TOPIC_ARN = 'arn:aws:sns:us-west-2:353734994300:SmartGreenHouseSNS'

def lambda_handler(event, context):
    print("Event:", event)
    
    try:
        payload = event  
        if isinstance(event, str):
            payload = json.loads(event)

        timestamp = str(int(time.time() * 1000))  # milliseconds

        records = []

        # Add temperature
        if 'temperature' in payload:
            temp = float(payload['temperature'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'temperature',
                'MeasureValue': str(temp),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })
            
        # Add humidity
        if 'humidity' in payload:
            hum = float(payload['humidity'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'humidity',
                'MeasureValue': str(hum),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })

        # Add light
        if 'light' in payload:
            lig = float(payload['light'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'light',
                'MeasureValue': str(lig),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })
            
        # Add tds
        if 'tds' in payload:
            td = float(payload['tds'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'tds',
                'MeasureValue': str(td),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })
           
        # Add soil moisture
        if 'soil_moisture' in payload:
            moi = float(payload['soil_moisture'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'soil_moisture',
                'MeasureValue': str(moi),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })
            
        # Add soil nitrogen
        if 'soil_nitrogen' in payload:
            nit = float(payload['soil_nitrogen'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'soil_nitrogen',
                'MeasureValue': str(nit),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })
            
        # Add soil phosphorus
        if 'soil_phosphorus' in payload:
            phos = float(payload['soil_phosphorus'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'soil_phosphorus',
                'MeasureValue': str(phos),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })
            
        # Add soil potassium
        if 'soil_potassium' in payload:
            pota = float(payload['soil_potassium'])
            records.append({
                'Dimensions': [{'Name': 'device', 'Value': 'greenhouse_1'}],
                'MeasureName': 'soil_potassium',
                'MeasureValue': str(pota),
                'MeasureValueType': 'DOUBLE',
                'Time': timestamp
            })

        # Write to Timestream
        if records:
            timestream.write_records(
                DatabaseName=DB_NAME,
                TableName=TABLE_NAME,
                Records=records
            )

        # Alerts
        alerts = []
        if 'temperature' in payload and float(payload['temperature']) > 30:
            alerts.append(f"🌡️ Temperature exceeded 30°C: {payload['temperature']}°C.")

        if 'humidity' in payload and float(payload['humidity']) < 50:
            alerts.append(f"💧 Humidity dropped to {payload['humidity']}%.")

        if 'light' in payload and float(payload['light']) > 15000:
            alerts.append(f"💡 Light intensity is too high: {payload['light']}lux.")

        if 'tds' in payload and float(payload['tds']) > 700:
            alerts.append(f"🧪 TDS level is high: {payload['tds']}ppm.")

        if 'soil_moisture' in payload and float(payload['soil_moisture']) < 45:
            alerts.append(f"🌱 Soil moisture is low: {payload['soil_moisture']}%.")

        if 'soil_nitrogen' in payload and float(payload['soil_nitrogen']) > 120:
            alerts.append(f"🧬 Soil Nitrogen level is high: {payload['soil_nitrogen']}mg/kg.")

        if 'soil_phosphorus' in payload and float(payload['soil_phosphorus']) > 60:
            alerts.append(f"⚗️ Soil Phosphorus level is high: {payload['soil_phosphorus']}mg/kg.")

        if 'soil_potassium' in payload and float(payload['soil_potassium']) > 100:
            alerts.append(f"🧪 Soil Potassium level is high: {payload['soil_potassium']}mg/kg.")


        if alerts:
            sns.publish(
                TopicArn=SNS_TOPIC_ARN,
                Subject='🚨 Greenhouse Alert!',
                Message="\n".join(alerts)
            )
        
        return {
            'statusCode': 200,
            'body': 'Data stored and metrics sent successfully.'
        }

    except Exception as e:
        print("Error:", e)
        return {
            'statusCode': 500,
            'body': f"Error: {str(e)}"
        }
