# Smart Green House CI/CD Pipeline

This CloudFormation template sets up a complete CI/CD pipeline for deploying the Smart Green House webapp on AWS. The pipeline automates the process of building, testing, and deploying your application from GitHub to an EC2 instance.

## Architecture Overview

The template creates the following AWS resources:

- **CodePipeline**: Orchestrates the entire CI/CD workflow
- **CodeBuild**: Builds and packages the webapp
- **CodeDeploy**: Deploys the application to EC2 instances
- **EC2 Instance**: Hosts the webapp with Node.js runtime
- **S3 Bucket**: Stores build artifacts
- **IAM Roles**: Provides necessary permissions for each service
- **Security Group**: Controls network access to the web server

## Prerequisites

Before deploying this template, ensure you have:

1. **AWS CLI** configured with appropriate permissions
2. **GitHub Personal Access Token** with repository access
3. **EC2 Key Pair** created in your target AWS region
4. **GitHub Repository** containing your Smart Green House webapp code

### Repository Structure Requirements

Your GitHub repository should have the following structure:
```
your-repo/
├── AWS-Project/
│   └── webapp/
│       ├── src/
│       ├── package.json
│       ├── appspec.yml
│       └── scripts/
│           ├── install_dependencies.sh
│           ├── start_server.sh
│           └── stop_application.sh
```

## Required Files in Your Repository

The following files are required in your repository for the deployment to work:

- `appspec.yml` - CodeDeploy application specification file
- `scripts/install_dependencies.sh` - Pre-deployment setup script
- `scripts/start_application.sh` - Application startup script  
- `scripts/stop_application.sh` - Application shutdown script

## Deployment Instructions

### 1. Create GitHub Personal Access Token

1. Go to GitHub Settings → Developer settings → Personal access tokens
2. Generate a new token with `repo` permissions
3. Copy the token for use in the CloudFormation parameters

### 2. Deploy the CloudFormation Stack

#### Using AWS CLI:
```bash
aws cloudformation create-stack \
  --stack-name smartgreenhouse-cicd \
  --template-body file://template.yaml \
  --parameters \
    ParameterKey=GitHubToken,ParameterValue=your-github-token \
    ParameterKey=KeyPairName,ParameterValue=your-key-pair-name \
    ParameterKey=GitHubRepo,ParameterValue=your-username/your-repo \
  --capabilities CAPABILITY_NAMED_IAM
```

#### Using AWS Console:
1. Open AWS CloudFormation console
2. Click "Create stack" → "With new resources"
3. Upload the template file
4. Fill in the required parameters
5. Enable "I acknowledge that AWS CloudFormation might create IAM resources with custom names"
6. Click "Create stack"

### 3. Monitor Deployment

1. Check the CloudFormation stack status in the AWS Console
2. Once complete, note the outputs (Web Server IP, DNS name, etc.)
3. The pipeline will automatically trigger when you push changes to the specified GitHub branch

## Pipeline Workflow

1. **Source Stage**: Retrieves code from GitHub repository
2. **Build Stage**: 
   - Navigates to `AWS-Project/webapp` directory
   - Runs `npm install` to install dependencies
   - Runs `npm run build` to build the application
   - Packages the build artifacts with deployment scripts
3. **Deploy Stage**: Deploys the application to EC2 using CodeDeploy

## Accessing Your Application

After successful deployment:

- **Web Application**: `http://<WebServerPublicIP>:3000`
- **SSH Access**: `ssh -i your-key.pem ec2-user@<WebServerPublicIP>`

## Monitoring and Troubleshooting

### CodePipeline Monitoring
- Navigate to AWS CodePipeline console
- Select your pipeline to view execution history
- Check individual stage logs for any failures

### CodeBuild Logs
- Go to AWS CodeBuild console
- Select your build project
- View build history and logs for troubleshooting

### EC2 Instance Logs
SSH into your instance and check:
```bash
# User data execution logs
sudo tail -f /var/log/cloud-init-output.log

# CodeDeploy agent logs
sudo tail -f /var/log/aws/codedeploy-agent/codedeploy-agent.log

# Application logs
sudo tail -f /var/log/user-data.log
```








## Cleanup

To delete all resources created by this template:

```bash
aws cloudformation delete-stack --stack-name stackName
```

Note: You may need to empty the S3 artifacts bucket before deletion.

## Support

For issues with the pipeline:
1. Check AWS CloudFormation events
2. Review CodePipeline execution details
3. Examine CodeBuild and CodeDeploy logs
4. Verify GitHub webhook configuration

## Contributing

When making changes to your application:
1. Push changes to the configured GitHub branch
2. Pipeline will automatically trigger
3. Monitor deployment through AWS Console
4. Verify application functionality after deployment
