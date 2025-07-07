import serial

ser = serial.Serial('/dev/serial0', 115200, timeout=1)
while True:
	try:
		line = ser.readline().decode('utf-8').strip()
		if line.startswith("RPY:") or line.startswith("Raw:"):  # Only process valid lines
			print(f"Received: {line}")
	except UnicodeDecodeError:
		ser.reset_input_buffer()  # Clear garbage data

# while True:
# 	line = ser.readline().decode('utf-8').strip()
# 	if line:
# 		print(f"Received: {line}")
		

