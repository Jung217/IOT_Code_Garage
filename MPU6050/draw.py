import serial
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import time

# 串口設置
ser = serial.Serial('COM3', 9600)
time.sleep(2)  # 等待 Arduino 初始化

# 初始化位置與速度
x, y, z = 0.0, 0.0, 0.0
vx, vy, vz = 0.0, 0.0, 0.0
dt = 0.01  # 時間間隔（秒）

trajectory = []

# === 取得偏移校正值（假設裝置靜止）===
print("Calibrating... Please keep device still for 2 seconds.")
offsets = []
start_time = time.time()
while time.time() - start_time < 2.0:
    try:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        parts = line.split(',')
        if len(parts) != 3:
            continue
        ax_raw, ay_raw, az_raw = map(int, parts)
        offsets.append([ax_raw, ay_raw, az_raw])
    except:
        continue

offsets = np.mean(offsets, axis=0)
print(f"Calibration offsets: ax={offsets[0]:.2f}, ay={offsets[1]:.2f}, az={offsets[2]:.2f}")

# 繪圖初始化
plt.ion()
fig = plt.figure()
ax_3d = fig.add_subplot(111, projection='3d')

# 主迴圈
while True:
    try:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        parts = line.split(',')
        if len(parts) != 3:
            continue

        ax_raw, ay_raw, az_raw = map(int, parts)

        # 扣除 offset，轉換為 g 值
        ax = (ax_raw - offsets[0]) / 16384.0
        ay = (ay_raw - offsets[1]) / 16384.0
        az = (az_raw - offsets[2]) / 16384.0

        # 積分計算速度與位置
        vx += ax * dt
        vy += ay * dt
        vz += az * dt
        x += vx * dt
        y += vy * dt
        z += vz * dt

        # 儲存並裁剪尾巴長度
        trajectory.append([x, y, z])
        if len(trajectory) > 50:  # 只保留最新的 50 筆資料（尾巴效果）
            trajectory.pop(0)

        # 繪圖
        ax_3d.clear()
        data = np.array(trajectory)
        ax_3d.plot(data[:, 0], data[:, 1], data[:, 2], label='Path', color='blue')
        ax_3d.scatter(data[-1, 0], data[-1, 1], data[-1, 2], color='red', s=50, label='Current')
        ax_3d.set_xlabel('X')
        ax_3d.set_ylabel('Y')
        ax_3d.set_zlabel('Z')
        ax_3d.set_title('3D Motion Trace')
        ax_3d.legend()
        plt.draw()
        plt.pause(0.001)

    except Exception as e:
        print(f"Error: {e}")
        continue
