import serial
import json
import re
import time

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200
THRESHOLDS_FILE = 'thresholds.json'

pattern = re.compile(r'T:(?P<temp>[-+]?[0-9]*\.?[0-9]+),H:(?P<hum>[-+]?[0-9]*\.?[0-9]+),M:(?P<moist>\d+)')

def load_thresholds():
    try:
        with open(THRESHOLDS_FILE, 'r') as f:
            return json.load(f)
    except:
        return None

def check_alert(temp, hum, moist, thresholds):
    if not thresholds:
        return False
    return (
        temp < thresholds['tempLow'] or temp > thresholds['tempHigh'] or
        hum < thresholds['humLow'] or hum > thresholds['humHigh'] or
        moist < thresholds['moistLow'] or moist > thresholds['moistHigh']
    )

def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    thresholds = load_thresholds()
    if not thresholds:
        print("Thresholds not found, exiting.")
        return
    print("Using thresholds:", thresholds)

    while True:
        line = ser.readline().decode('utf-8').strip()
        if not line:
            continue
        match = pattern.match(line)
        if match:
            temp = float(match.group('temp'))
            hum = float(match.group('hum'))
            moist = int(match.group('moist'))

            alert = check_alert(temp, hum, moist, thresholds)

            if alert:
                ser.write(b"ALERT_ON\n")
                print(f"ALERT ON: T={temp},H={hum},M={moist}")
            else:
                ser.write(b"ALERT_OFF\n")
                print(f"Alert off: T={temp},H={hum},M={moist}")

            time.sleep(1)

if __name__ == "__main__":
    main()
