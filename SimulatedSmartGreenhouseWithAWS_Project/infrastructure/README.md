# Static Website Deployment with CloudFormation

This CloudFormation template deploys a static website on AWS S3, using AWS CodePipeline and CodeBuild to automate deployment from a GitHub repository.

## What this template does

- Creates an **S3 bucket** configured for static website hosting.
- Sets up an **artifact bucket** for CodePipeline.
- Creates an **IAM roles** for CodeBuild and CodePipeline with necessary permissions.
- Defines a **CodeBuild project** to build your static site.
- Defines a **CodePipeline pipeline** that:
  - Pulls source code from your GitHub repository.
  - Builds the static site using CodeBuild.
  - Deploys the built site to the S3 website bucket.

## Prerequisites

- A GitHub repository containing your static website files.
- A GitHub personal access token with permissions to access the repository.
- AWS CLI configured or access to AWS Management Console with permissions to create CloudFormation stacks.

## How to create the CloudFormation stack

1. Go to the AWS CloudFormation Console.

2. Click **Create stack** → **With new resources (standard)**.

3. Upload this template file (`template.yaml`).

4. Provide the following parameters:
   - **GitHubRepo:** Your GitHub repository in the format `owner/repo` (e.g., `Smart-Green-houses/SmartGreenHouse`)
   - **GitHubBranch:** Branch to use (default: `master`)
   - **GitHubOAuthToken:** Your GitHub personal access token (this will be stored securely)

5. Follow the prompts and create the stack.

6. Wait until the stack status is `CREATE_COMPLETE`. This process creates the pipeline and starts deploying your site.

## How to access your static website

- Once the stack creation is complete, go to the **Outputs** tab of your CloudFormation stack.

- Find the `WebsiteURL` output.

- Open this URL in your browser — it points to your static site hosted on S3.

## Notes

- The pipeline automates deployment on new commits to the specified GitHub branch.
- Make sure your website files contain an `index.html` at minimum.
- The S3 bucket is configured with public read access for website content.
