#!/bin/bash
# Update & install dependencies
yum update -y
yum install -y python3 git

# Upgrade pip and install Python dependencies
pip3 install --upgrade pip
pip3 install AWSIoTPythonSDK boto3

# Create working directory
mkdir -p /home/ec2-user/greenhouse
cd /home/ec2-user/greenhouse

# Create the simulation script
cat <<EOF > smart_greenhouse_simulator.py
<PASTE THE FULL PYTHON SIMULATOR WITH CONTROL HERE>
EOF

# Replace <PASTE...> with the simulator you have

# Place your cert files in this directory (or pull from S3/git as needed)
# certificate.pem.crt
# private.pem.key
# AmazonRootCA1.pem

# Launch simulator
nohup python3 smart_greenhouse_simulator_with_controls.py > simulator.log 2>&1 &