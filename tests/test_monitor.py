#!/usr/bin/env python3
import subprocess
import time
import os

def test_pe_detection():
    # Create test PE file and check if logged
    with open("/home/test.exe", "wb") as f:
        f.write(b"MZ\x90\x00")  # Minimal PE header
    
    time.sleep(2)  # Wait for detection
    
    with open("/var/log/pe_monitor.log", "r") as log:
        assert "test.exe" in log.read()
    
    os.remove("/home/test.exe")

if __name__ == "__main__":
    test_pe_detection()
