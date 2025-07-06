import serial
import numpy as np
from ahrs.filters import Mahony
from ahrs.common.orientation import q2euler

# Set up serial port
ser = serial.Serial('/dev/serial0', 115200)

# Mahony filter instance
mahony = Mahony()

# Orientation state (quaternion)
q = np.array([1.0, 0.0, 0.0, 0.0])

while True:
    try:
        line = ser.readline().decode().strip()
        if not line.startswith("A:"): continue
        
        # Parse the string
        parts = line.replace("A:", "").replace("G:", "").split()
        acc = list(map(int, parts[0].split(',')))
        gyro = list(map(int, parts[1].split(',')))

        # Convert to proper units
        acc = np.array(acc) / 16384.0     # g → m/s²
        gyro = np.radians(np.array(gyro) / 131.0)  # deg/s → rad/s

        # Apply Mahony filter
        q = mahony.updateIMU(q=q, acc=acc, gyr=gyro)

        # Convert quaternion to Euler angles (in radians)
        euler = q2euler(q)  # [roll, pitch, yaw]
        print("Roll: {:.2f}, Pitch: {:.2f}, Yaw: {:.2f}".format(
            np.degrees(euler[0]), np.degrees(euler[1]), np.degrees(euler[2])
        ))

    except Exception as e:
        print("Error:", e)
