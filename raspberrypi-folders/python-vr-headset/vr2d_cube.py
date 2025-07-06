import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from ahrs.filters import Mahony
from ahrs.common.orientation import q2euler
import time

# --------------------------- Configuration ---------------------------
PORT = '/dev/serial0'  # Update if necessary
BAUD_RATE = 115200
SCREEN_WIDTH = 1920
SCREEN_HEIGHT = 1080
CUBE_SIZE = 0.5

# --------------------------- Cube Definition ---------------------------
cube_vertices = np.array([
    [-1, -1, -1], [ 1, -1, -1],
    [ 1,  1, -1], [-1,  1, -1],
    [-1, -1,  1], [ 1, -1,  1],
    [ 1,  1,  1], [-1,  1,  1]
]) * CUBE_SIZE

cube_edges = [
    (0,1), (1,2), (2,3), (3,0),
    (4,5), (5,6), (6,7), (7,4),
    (0,4), (1,5), (2,6), (3,7)
]

# --------------------------- Projection & Rotation ---------------------------
def project(point, eye_offset=0.0):
    # Simple orthographic projection for now
    x, y, z = point
    return x + eye_offset, y

def rotate(vertices, R):
    return np.dot(vertices, R.T)

# --------------------------- Orientation Reader ---------------------------
mahony_filter = Mahony()
q = np.array([1.0, 0.0, 0.0, 0.0])  # Initial quaternion

def parse_imu_line(line):
    try:
        line = line.decode().strip()
        if not line.startswith("A:"):
            return None
        parts = line.split(" ")
        accel = list(map(int, parts[0][2:].split(",")))
        gyro = list(map(int, parts[1][2:].split(",")))
        return accel, gyro
    except:
        return None

def read_orientation(ser):
    global q
    while ser.in_waiting:
        line = ser.readline()
        result = parse_imu_line(line)
        if result:
            accel, gyro = result
            acc = np.array(accel, dtype=np.float64) / 16384.0
            gyr = np.radians(np.array(gyro, dtype=np.float64) / 131.0)
            q = mahony_filter.updateIMU(q, gyr=gyr, acc=acc)
    euler = q2euler(q)
    return euler  # roll, pitch, yaw in radians

# --------------------------- Matplotlib Setup ---------------------------
fig, ax = plt.subplots(figsize=(12, 6))
plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
ax.set_xlim(-2, 2)
ax.set_ylim(-1.5, 1.5)
ax.set_aspect('equal')
ax.axis('off')

# Create line objects for both eyes
left_lines = [ax.plot([], [], 'b')[0] for _ in cube_edges]
right_lines = [ax.plot([], [], 'r')[0] for _ in cube_edges]

# --------------------------- Main Update Function ---------------------------
ser = serial.Serial(PORT, BAUD_RATE, timeout=0.01)
last_debug = time.time()

def update(frame):
    global last_debug
    roll, pitch, yaw = read_orientation(ser)

    # Construct rotation matrix from roll, pitch, yaw
    Rx = np.array([
        [1, 0, 0],
        [0, np.cos(roll), -np.sin(roll)],
        [0, np.sin(roll),  np.cos(roll)]
    ])
    Ry = np.array([
        [ np.cos(pitch), 0, np.sin(pitch)],
        [0, 1, 0],
        [-np.sin(pitch), 0, np.cos(pitch)]
    ])
    Rz = np.array([
        [np.cos(yaw), -np.sin(yaw), 0],
        [np.sin(yaw),  np.cos(yaw), 0],
        [0, 0, 1]
    ])
    R = Rz @ Ry @ Rx

    rotated = rotate(cube_vertices, R)

    # Left eye (-eye_offset), Right eye (+eye_offset)
    eye_offset = 1.5 # Adjust this value for inter-pupillary distance
    left_proj = [project(p, -eye_offset) for p in rotated]
    right_proj = [project(p, +eye_offset) for p in rotated]

    for i, (start, end) in enumerate(cube_edges):
        x1, y1 = left_proj[start]
        x2, y2 = left_proj[end]
        left_lines[i].set_data([x1, x2], [y1, y2])

        x1, y1 = right_proj[start]
        x2, y2 = right_proj[end]
        right_lines[i].set_data([x1, x2], [y1, y2])

    if time.time() - last_debug > 1:
        print(f"Roll: {np.degrees(roll):.2f}, Pitch: {np.degrees(pitch):.2f}, Yaw: {np.degrees(yaw):.2f}")
        last_debug = time.time()

ani = FuncAnimation(fig, update, interval=33)
plt.show()
