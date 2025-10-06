#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>

MPU6050 accelgyro(0x68);

int16_t ax, ay, az, gx, gy, gz;
int16_t ax_offset = 0, ay_offset = 0, az_offset = 0;
int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

float pitch = 0.0, roll = 0.0;
unsigned long lastTime = 0;

// 校正函式
void calibrateMPU6050() {
  long ax_sum = 0, ay_sum = 0, az_sum = 0;
  long gx_sum = 0, gy_sum = 0, gz_sum = 0;
  const int samples = 1000;

  Serial.println("校正中，請保持靜止...");
  for (int i = 0; i < samples; i++) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    ax_sum += ax;
    ay_sum += ay;
    az_sum += az;
    gx_sum += gx;
    gy_sum += gy;
    gz_sum += gz;
    delay(2);
  }

  ax_offset = ax_sum / samples;
  ay_offset = ay_sum / samples;
  az_offset = (az_sum / samples) - 16384;
  gx_offset = gx_sum / samples;
  gy_offset = gy_sum / samples;
  gz_offset = gz_sum / samples;

  Serial.println("校正完成！");
}

void setup() {
  Wire.begin(21, 22);
  Wire.setClock(100000);
  Serial.begin(115200);

  Serial.println("初始化 MPU6050...");
  accelgyro.initialize();

  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 連線成功");
  } else {
    Serial.println("MPU6050 連線失敗");
    while (1);
  }

  calibrateMPU6050();
  lastTime = millis();
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // 校正
  ax -= ax_offset; ay -= ay_offset; az -= az_offset;
  gx -= gx_offset; gy -= gy_offset; gz -= gz_offset;

  // 計算時間差
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;
  lastTime = now;

  // 加速度角度 (單位為度)
  float accel_pitch = atan2(ay, sqrt(ax * ax + az * az)) * 180 / PI;
  float accel_roll = atan2(-ax, az) * 180 / PI;

  // 陀螺儀角速度 (MPU6050 輸出每度秒值需除以 131)
  float gyro_pitch_rate = gy / 131.0;
  float gyro_roll_rate = gx / 131.0;

  // 互補濾波：融合加速度與陀螺儀
  pitch = 0.96 * (pitch + gyro_pitch_rate * dt) + 0.04 * accel_pitch;
  roll = 0.96 * (roll + gyro_roll_rate * dt) + 0.04 * accel_roll;

  // 輸出
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print("\tRoll: ");
  Serial.println(roll);

  delay(20);  // 建議更新頻率 50Hz
}
