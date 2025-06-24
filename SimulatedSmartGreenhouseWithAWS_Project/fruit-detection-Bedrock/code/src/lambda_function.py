import json
import base64
import boto3
import logging
import os

# Configure logging
logger = logging.getLogger()
logger.setLevel(os.environ.get('LOG_LEVEL', 'INFO'))

# Initialize clients for both S3 and Bedrock
s3 = boto3.client('s3')
bedrock = boto3.client('bedrock-runtime')
MODEL_ID = os.environ.get('MODEL_ID', 'anthropic.claude-3-5-sonnet-20240620-v1:0') 

def lambda_handler(event, context):
    try:
        # 1. Parse the bucket and key from the incoming API Gateway event body
        logger.info(f"Received event: {event}")
        body = json.loads(event.get('body', '{}'))
        
        bucket = body.get('bucket')
        key = body.get('key')

        if not bucket or not key:
            return {
                'statusCode': 400,
                'body': json.dumps({'error': 'Request body must be a JSON object with "bucket" and "key" properties.'})
            }

        # 2. Fetch the image from the specified S3 bucket
        logger.info(f"Fetching image from bucket: {bucket}, key: {key}")
        s3_response = s3.get_object(Bucket=bucket, Key=key)
        image_bytes = s3_response['Body'].read()
        
        # 3. Base64 encode the image for the Bedrock model
        image_base64_string = base64.b64encode(image_bytes).decode('utf-8')

        # 4. Use our detailed prompt for the AI model
        prompt_text = """
You are a plant pathology expert. Analyze the attached fruit image and determine if there is a disease present.

Return a strictly formatted JSON response:
{
  "fruit_type": "orange/strawberry/mango",
  "disease_detected": true/false,
  "disease_name": "specific disease name or 'healthy'",
  "confidence": float between 0.0 and 1.0,
  "severity": "none/mild/moderate/severe",
  "treatment_recommendations": ["treatment 1", "treatment 2"]
}

Focus only on these fruits and associated diseases:

- **Oranges**
  - black spot
  - citrus canker
  - greening disease

- **Strawberries**
  - gray mold
  - leaf spot
  - powdery mildew

- **Mangoes**
  - anthracnose
  - powdery mildew

If the fruit is healthy, set:
  - "disease_detected": false
  - "disease_name": "healthy"
  - "severity": "none"
  - "treatment_recommendations": []

Respond in clear, compact JSON. No explanation or extra text.
"""

        # 5. Invoke the Bedrock model
        logger.info(f"Invoking Bedrock model: {MODEL_ID}")
        response = bedrock.invoke_model(
            modelId=MODEL_ID,
            body=json.dumps({
                "anthropic_version": "bedrock-2023-05-31",
                "max_tokens": 1000,
                "messages": [{
                    "role": "user",
                    "content": [
                        {
                            "type": "image",
                            "source": {
                                "type": "base64",
                                "media_type": "image/jpeg", # This can be improved to be dynamic if needed
                                "data": image_base64_string
                            }
                        },
                        { "type": "text", "text": prompt_text }
                    ]
                }]
            }),
            contentType='application/json',
            accept='application/json'
        )

        result = json.loads(response['body'].read())
        model_response_content = result['content'][0]['text']

        return {
            'statusCode': 200,
            'headers': {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*'
            },
            'body': model_response_content
        }

    except Exception as e:
        logger.error(f"Error processing image: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps({'error': str(e)})
        }
