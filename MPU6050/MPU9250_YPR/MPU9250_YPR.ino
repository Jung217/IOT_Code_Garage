#include <Arduino.h>
#include <Wire.h>
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa imu;

float pitch = 0, roll = 0, yaw = 0;
unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(21, 22);  // 若使用 ESP32 須指定腳位（SDA, SCL）

  imu.setWire(&Wire);
  imu.beginAccel();
  imu.beginGyro();
  imu.beginMag(); // 啟用磁力計（MPU6500 無內建，需外接 AK8963）

  Serial.println("MPU6500 初始化完成");
}

void loop() {
  imu.accelUpdate();
  imu.gyroUpdate();
  imu.magUpdate(); // 磁力計更新（如果有連接）

  float ax = imu.accelX();
  float ay = imu.accelY();
  float az = imu.accelZ();
  float gx = imu.gyroX();
  float gy = imu.gyroY();
  float gz = imu.gyroZ();

  float mx = imu.magX();
  float my = imu.magY();
  float mz = imu.magZ();

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // 加速度求姿態
  float accel_pitch = atan2(ay, sqrt(ax * ax + az * az)) * 180 / PI;
  float accel_roll  = atan2(-ax, az) * 180 / PI;

  // 陀螺儀 (deg/s)
  pitch = 0.96 * (pitch + gy * dt) + 0.04 * accel_pitch;
  roll  = 0.96 * (roll  + gx * dt) + 0.04 * accel_roll;

  // 計算 yaw（需磁力計）
  float mag_yaw = atan2(my, mx) * 180 / PI;
  if (mag_yaw < 0) mag_yaw += 360;
  yaw = 0.96 * (yaw + gz * dt) + 0.04 * mag_yaw;

  // 輸出姿態角
  Serial.print("Pitch: "); Serial.print(pitch, 2);
  Serial.print("  Roll: "); Serial.print(roll, 2);
  Serial.print("  Yaw: "); Serial.println(yaw, 2);

  delay(20);  // 50Hz 更新頻率
}
