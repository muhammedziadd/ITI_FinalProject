# Configuration for Amazon Linux 2023
$EC2_HOST = "18.237.109.214"
$KEY_PATH = "labsuser.pem"
$REMOTE_USER = "ec2-user"
$APP_DIR = "/var/www/html/webapp"
$LOCAL_BUILD_DIR = "dist"
$WEB_SERVER = "nginx"  # Change to "httpd" if using Apache

Write-Host "🚀 Starting deployment to Amazon Linux 2023..." -ForegroundColor Green

# Build the application
Write-Host "📦 Building application..." -ForegroundColor Yellow
if (Test-Path $LOCAL_BUILD_DIR) {
    Remove-Item -Path $LOCAL_BUILD_DIR -Recurse -Force
}

npm run build

if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Build failed!" -ForegroundColor Red
    exit 1
}

Write-Host "✅ Build completed successfully" -ForegroundColor Green

# Create backup on server
Write-Host "💾 Creating backup on server..." -ForegroundColor Yellow
$backupCommand = @"
sudo mkdir -p /var/backups/webapp
if [ -d '$APP_DIR' ]; then
    sudo cp -r $APP_DIR /var/backups/webapp/backup-`$(date +%Y%m%d-%H%M%S)
fi
"@

& ssh -i $KEY_PATH "$REMOTE_USER@$EC2_HOST" $backupCommand

# Upload new files
Write-Host "📤 Uploading files to server..." -ForegroundColor Yellow
& ssh -i $KEY_PATH "$REMOTE_USER@$EC2_HOST" "mkdir -p /tmp/webapp-new"

# Use scp to upload files (PowerShell equivalent)
& scp -i $KEY_PATH -r "$LOCAL_BUILD_DIR/*" "$REMOTE_USER@${EC2_HOST}:/tmp/webapp-new/"

if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Upload failed!" -ForegroundColor Red
    exit 1
}

# Deploy on server
Write-Host "🔄 Deploying on server..." -ForegroundColor Yellow
$deployCommand = @"
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
"@

& ssh -i $KEY_PATH "$REMOTE_USER@$EC2_HOST" $deployCommand

if ($LASTEXITCODE -eq 0) {
    Write-Host "✅ Deployment successful!" -ForegroundColor Green
    Write-Host "🌐 Your app should be available at: http://$EC2_HOST" -ForegroundColor Cyan
    Write-Host "📋 To check logs: ssh -i $KEY_PATH $REMOTE_USER@$EC2_HOST 'sudo tail -f /var/log/$WEB_SERVER/access.log'" -ForegroundColor Gray
} else {
    Write-Host "❌ Deployment failed!" -ForegroundColor Red
    
    # Show recent logs for debugging
    Write-Host "📋 Recent error logs:" -ForegroundColor Yellow
    & ssh -i $KEY_PATH "$REMOTE_USER@$EC2_HOST" "sudo tail -20 /var/log/$WEB_SERVER/error.log"
    exit 1
}