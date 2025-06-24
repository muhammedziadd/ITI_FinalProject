import boto3
import json

timestream = boto3.client('timestream-query')

DB_NAME = 'SmartGreenHouseDB'
TABLE_NAME = 'SmartGHTable'

def lambda_handler(event, context):
    query = f"""
        SELECT *
        FROM "{DB_NAME}"."{TABLE_NAME}"
        ORDER BY time DESC
        LIMIT 1
    """

    try:
        response = timestream.query(QueryString=query)
        row = response['Rows'][0]['Data']
        
        fields = ['temperature', 'humidity', 'light', 'tds', 'soil_moisture',
                  'soil_nitrogen', 'soil_phosphorus', 'soil_potassium', 'time']
        result = {}

        for i, field in enumerate(fields):
            if 'ScalarValue' in row[i]:
                result[field] = row[i]['ScalarValue']

        return {
            'statusCode': 200,
            'headers': {
                "Content-Type": "application/json",
                "Access-Control-Allow-Origin": "*"
            },
            'body': json.dumps(result)
        }

    except Exception as e:
        print("API error:", e)
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
