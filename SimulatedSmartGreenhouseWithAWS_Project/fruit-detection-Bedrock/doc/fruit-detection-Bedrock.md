#  Fruit Disease Detection using AWS Bedrock AI

An AI-powered serverless system for real-time detection of fruit diseases using image input, built as part of the AWS re/Start graduation project.

##  Project Overview

This project demonstrates how serverless architecture and generative AI can be applied to a real-world agricultural problem: **detecting diseases in fruits**. Users can upload a fruit image and receive a diagnosis with recommended treatments using AI via AWS Bedrock.

---

##  Goal

- Rapidly and accurately detect fruit diseases.
- Leverage **serverless architecture** to reduce cost and complexity.
- Use **generative AI (Claude 3.5 Sonnet)** via Amazon Bedrock.
- Build a real-time, accessible API with a pay-per-use model.

---

##  Architecture

The system uses a **serverless-first, event-driven** design for scalability and minimal maintenance.
![Architecture Diagram](AI_architecture.drawio.svg)

###  Services Used

| Service | Role |
|--------|------|
| **Amazon API Gateway** | Entry point for the image analysis API |
| **AWS Lambda** | Executes the Python logic and communicates with Bedrock |
| **Amazon Bedrock** | Hosts Claude 3.5 Sonnet model for disease prediction |
| **AWS IAM** | Manages secure, scoped access to services |
| **Amazon S3** | Stores deployment artifacts |
| **AWS SAM** | Defines and deploys the infrastructure as code |
| **Amazon CloudWatch** | Collects logs for monitoring and debugging |

---

##  Sample AI Output

```json
{
  "fruit_type": "strawberry",
  "disease_detected": true,
  "disease_name": "gray mold",
  "confidence": 0.8,
  "severity": "moderate",
  "treatment_recommendations": [
    "Remove infected fruits and plant debris",
    "Improve air circulation around plants",
    "Apply fungicide specifically formulated for Botrytis"
  ]
}
```
##  Future Enhancements

-  **Store analysis history** with Amazon DynamoDB.
-  **Add asynchronous support** for larger files (e.g., videos) using S3 events + SQS.
-  **Integrate visualization dashboards** with Amazon QuickSight.

##  Deployment & Tools

This project was built and deployed using:

- **AWS SAM (Serverless Application Model)**
- **Python** (for Lambda functions)
- **API Gateway + Lambda Integration**
- **Claude 3.5 Sonnet via Amazon Bedrock**

