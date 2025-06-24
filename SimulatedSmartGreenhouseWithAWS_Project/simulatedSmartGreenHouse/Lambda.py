import json
import boto3
import os
import time
from datetime import datetime

timestream = boto3.client('timestream-write')
sns = boto3.client('sns')

# Use environment variables
DATABASE_NAME = 'GreenhouseDB'
TABLE_NAME = 'SensorData'
SNS_TOPIC_ARN = 'arn:aws:sns:us-west-2:664751201482:GreenhouseAlerts'

def lambda_handler(event, context):
    print("Received event:", json.dumps(event))

    try:
        # Try to handle both formats: test & IoT
        if 'temperature' in event:
            payload = event
        elif 'body' in event:
            payload = json.loads(event['body'])
        elif isinstance(event, dict):
            payload = event
        else:
            raise ValueError("Unsupported event format")

        timestamp = str(int(time.time() * 1000))  # epoch in ms

        dimensions = [{'Name': 'device', 'Value': 'smart-greenhouse'}]
        records = []

        # Basic sensor data
        for key in ['temperature', 'humidity', 'light', 'soil_moisture', 'tds']:
            if key in payload:
                records.append({
                    'Dimensions': dimensions,
                    'MeasureName': key,
                    'MeasureValue': str(payload[key]),
                    'MeasureValueType': 'DOUBLE',
                    'Time': timestamp
                })

        # NPK values
        if 'soil_npk' in payload:
            for nutrient, value in payload['soil_npk'].items():
                records.append({
                    'Dimensions': dimensions + [{'Name': 'nutrient', 'Value': nutrient}],
                    'MeasureName': 'soil_npk',
                    'MeasureValue': str(value),
                    'MeasureValueType': 'DOUBLE',
                    'Time': timestamp
                })

        # Write to Timestream
        timestream.write_records(
            DatabaseName=DATABASE_NAME,
            TableName=TABLE_NAME,
            Records=records
        )
        print("✅ Data written to Timestream.")

        # Alerts
        alerts = []
        if payload.get('tds', 0) > 600:
            alerts.append(f"⚠️ TDS too high: {payload['tds']} ppm")
        if 'soil_npk' in payload:
            if payload['soil_npk'].get('nitrogen', 100) < 80:
                alerts.append("⚠️ Nitrogen deficiency detected")
            if payload['soil_npk'].get('phosphorus', 100) < 30:
                alerts.append("⚠️ Phosphorus deficiency detected")
            if payload['soil_npk'].get('potassium', 100) < 50:
                alerts.append("⚠️ Potassium deficiency detected")

        if alerts:
            sns.publish(
                TopicArn=SNS_TOPIC_ARN,
                Subject="🚨 Smart Greenhouse Alert",
                Message="\n".join(alerts)
            )
            print("🚨 Alert published to SNS.")

        return {
            'statusCode': 200,
            'body': 'Success'
        }

    except Exception as e:
        print("❌ Error processing event:", e)
        return {
            'statusCode': 500,
            'body': str(e)
        }