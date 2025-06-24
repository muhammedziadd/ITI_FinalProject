#!/bin/bash
set -e
echo "Setting file permissions..."
cd /var/www/html
chown -R ec2-user:ec2-user /var/www/html
chmod -R 755 /var/www/html

# Ensure webapp log file exists and has correct permissions
touch /var/log/webapp.log
chown ec2-user:ec2-user /var/log/webapp.log
chmod 644 /var/log/webapp.log

echo "Permissions set successfully"
