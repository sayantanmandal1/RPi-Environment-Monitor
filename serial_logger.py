import serial
import re
import csv
import os
from datetime import datetime

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200
CSV_FILE = 'sensor_data.csv'

# Updated regex: optional "Sent: " at the beginning
pattern = re.compile(r'(?:Sent:\s*)?T:(?P<temp>[-+]?[0-9]*\.?[0-9]+),H:(?P<hum>[-+]?[0-9]*\.?[0-9]+),M:(?P<moist>\d+)')

def write_header_if_needed():
    if not os.path.exists(CSV_FILE):
        with open(CSV_FILE, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(['timestamp', 'temperature', 'humidity', 'soil_moisture'])

def main():
    write_header_if_needed()
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print("Started logging sensor data...")
    
    while True:
        line = ser.readline().decode('utf-8').strip()
        if not line:
            continue

        match = pattern.match(line)
        if match:
            timestamp = datetime.now().isoformat()
            temp = float(match.group('temp'))
            hum = float(match.group('hum'))
            moist = int(match.group('moist'))

            with open(CSV_FILE, 'a', newline='') as f:
                writer = csv.writer(f)
                writer.writerow([timestamp, temp, hum, moist])
            print(f"Logged: T={temp}, H={hum}, M={moist}")
        else:
            print(f"Ignored line: {line}")

if __name__ == "__main__":
    main()
