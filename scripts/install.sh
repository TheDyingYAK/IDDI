#!/bin/bash

# Check if run as root
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit 1
fi

# Install dependencies
apt update
apt install -y libssl-dev

# Download or assume pe_monitor.c is present
# For simplicity, assume the C code is in the current directory

# Compile
gcc pe_monitor.c -o pe_monitor -lssl -lcrypto

# Move binary
mv pe_monitor /usr/local/bin/

# Create service file
cat << EOF > /etc/systemd/system/pe-monitor.service
[Unit]
Description=PE File Monitor Service
After=network.target

[Service]
ExecStart=/usr/local/bin/pe_monitor
Restart=always
User=root
Group=root
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=pe_monitor

[Install]
WantedBy=multi-user.target
EOF

# Reload and enable
systemctl daemon-reload
systemctl enable pe-monitor.service
systemctl start pe-monitor.service

echo "Installation complete. Check status with: systemctl status pe-monitor.service"
