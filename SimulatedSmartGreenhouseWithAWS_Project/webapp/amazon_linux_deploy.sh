#!/bin/bash

# Configuration for Amazon Linux 2023
EC2_HOST="18.237.109.214" # Replace with your EC2 instance's public IP or DNS
KEY_PATH="labsuser.pem"  # Replace with your key pair file
REMOTE_USER="ec2-user"
APP_DIR="/var/www/html/webapp"
LOCAL_BUILD_DIR="dist"
WEB_SERVER="nginx"  # Change to "httpd" if using Apache

echo "🚀 Starting deployment to Amazon Linux 2023..."

# Build the application
echo "📦 Building application..."
rm -rf dist
npm run build

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo "✅ Build completed successfully"

# Create backup on server
echo "💾 Creating backup on server..."
ssh -i $KEY_PATH $REMOTE_USER@$EC2_HOST "
    sudo mkdir -p /var/backups/webapp
    if [ -d '$APP_DIR' ]; then
        sudo cp -r $APP_DIR /var/backups/webapp/backup-\$(date +%Y%m%d-%H%M%S)
    fi
"

# Upload new files
echo "📤 Uploading files to server..."
ssh -i $KEY_PATH $REMOTE_USER@$EC2_HOST "mkdir -p /tmp/webapp-new"
scp -i $KEY_PATH -r $LOCAL_BUILD_DIR/* $REMOTE_USER@$EC2_HOST:/tmp/webapp-new/

if [ $? -ne 0 ]; then
    echo "❌ Upload failed!"
    exit 1
fi

# Deploy on server
echo "🔄 Deploying on server..."
ssh -i $KEY_PATH $REMOTE_USER@$EC2_HOST "
    # Create app directory if it doesn't exist
    sudo mkdir -p $APP_DIR
    
    # Remove old files
    sudo rm -rf $APP_DIR/*
    
    # Move new files
    sudo mv /tmp/webapp-new/* $APP_DIR/
    
    # Set permissions based on web server
    if [ '$WEB_SERVER' = 'nginx' ]; then
        sudo chown -R nginx:nginx $APP_DIR
    else
        sudo chown -R apache:apache $APP_DIR
    fi
    sudo chmod -R 755 $APP_DIR
    
    # Clean up
    rm -rf /tmp/webapp-new
    
    # Restart web server
    sudo systemctl reload $WEB_SERVER
    
    # Check if service is running
    sudo systemctl is-active $WEB_SERVER
"

if [ $? -eq 0 ]; then
    echo "✅ Deployment successful!"
    echo "🌐 Your app should be available at: http://$EC2_HOST"
    echo "📋 To check logs: ssh -i $KEY_PATH $REMOTE_USER@$EC2_HOST 'sudo tail -f /var/log/$WEB_SERVER/access.log'"
else
    echo "❌ Deployment failed!"
    
    # Show recent logs for debugging
    echo "📋 Recent error logs:"
    ssh -i $KEY_PATH $REMOTE_USER@$EC2_HOST "sudo tail -20 /var/log/$WEB_SERVER/error.log"
    exit 1
fi