#!/bin/bash
set -e

echo "Stopping Smart Green House application..."

# Stop PM2 process
pm2 delete greenhouse-app 2>/dev/null || true

# Fallback: kill any remaining serve processes
pkill -f "serve" || true

echo "✅ Application stopped"