import serial

ser = serial.Serial('/dev/serial0', 115200, timeout=1)

while True:
	line = ser.readline().decode('utf-8').strip()
	if line:
		print(f"Received: {line}")