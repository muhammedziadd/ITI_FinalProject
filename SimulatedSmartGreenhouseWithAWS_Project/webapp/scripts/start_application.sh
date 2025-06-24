#!/bin/bash
set -e

echo "=== Starting Smart Green House Application ==="

# Set explicit paths
export HOME="/home/ec2-user"
NODE_VERSION="v16.20.2"
NODE_BIN="/home/ec2-user/.nvm/versions/node/$NODE_VERSION/bin"
export PATH="$NODE_BIN:$PATH"

NODE_CMD="$NODE_BIN/node"
NPM_CMD="$NODE_BIN/npm"
PM2_CMD="$NODE_BIN/pm2"

cd /var/www/html

# Install compatible version of serve (version 13.x works with Node 16)
echo "Installing compatible serve version..."
$NPM_CMD install -g serve@13.0.4

# Check if PM2 is installed
if [ ! -f "$PM2_CMD" ]; then
    echo "Installing PM2 globally..."
    $NPM_CMD install -g pm2
fi

# Stop existing processes
$PM2_CMD delete greenhouse-app 2>/dev/null || true
sleep 2

# Find build directory
if [ -d "dist" ]; then
    BUILD_DIR="dist"
elif [ -d "build" ]; then
    BUILD_DIR="build"
else
    echo "❌ ERROR: No dist or build directory found"
    exit 1
fi

# Start with PM2 using the compatible serve version
echo "Starting application with serve@13.0.4..."
$PM2_CMD start "$NODE_BIN/serve" --name "greenhouse-app" -- -s $BUILD_DIR -l 3000

$PM2_CMD save

sleep 3

if $PM2_CMD list | grep -q "greenhouse-app.*online"; then
    echo "✅ Application started successfully"
    echo "📂 Serving from: $BUILD_DIR"
    $PM2_CMD list
else
    echo "❌ ERROR: Application failed to start"
    $PM2_CMD logs greenhouse-app --lines 10
    exit 1
fi