import serial
import time
import sys

if len(sys.argv) < 2:
    print("Usage: python3 script.py <serial_port>")
    sys.exit(1)

port = sys.argv[1]
baudrate = 115200 

ser = serial.Serial(port, baudrate, timeout=1)

start_time = None
at_count = 0

try:
    print("Waiting start ^")
    while True:
        char = ser.read().decode(errors='ignore')
        if not char:
            continue

        if start_time is None:
            if char == '^':
                print("start ^ received")
                start_time = time.time()
                at_count = 0
            else:
                print(char, end='', flush=True)
        else:
            if char == '$':
                end_time = time.time()
                delta = end_time - start_time
                rate = at_count / delta if delta > 0 else 0
                print(f"Delta time: {delta:.6f} seconds")
                print(f"@ per second: {rate:.2f}")
                break
            elif char == '-':
                print('.', end='', flush=True)
                at_count += 256
finally:
    ser.close()

