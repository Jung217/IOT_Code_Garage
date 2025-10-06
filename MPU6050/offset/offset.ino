#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

// MPU6050 I2C 位址
MPU6050 accelgyro(0x68);

// 原始與修正後的讀值
int16_t ax, ay, az, gx, gy, gz;
int16_t ax_offset = 0, ay_offset = 0, az_offset = 0;
int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

// 校正函式
void calibrateMPU6050() {
  long ax_sum = 0, ay_sum = 0, az_sum = 0;
  long gx_sum = 0, gy_sum = 0, gz_sum = 0;
  const int samples = 1000;

  Serial.println("校正中，請保持模組靜止...");

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
  az_offset = (az_sum / samples) - 16384; // Z 軸應為 1g ≈ 16384
  gx_offset = gx_sum / samples;
  gy_offset = gy_sum / samples;
  gz_offset = gz_sum / samples;

  Serial.println("校正完成！");
  Serial.print("加速度 offset:\t");
  Serial.print(ax_offset); Serial.print("\t");
  Serial.print(ay_offset); Serial.print("\t");
  Serial.println(az_offset);
  Serial.print("角速度 offset:\t");
  Serial.print(gx_offset); Serial.print("\t");
  Serial.print(gy_offset); Serial.print("\t");
  Serial.println(gz_offset);
}

void setup() {
  Wire.setClock(100000);
  Wire.begin(21, 22);  // ESP32 的 SDA (21), SCL (22)
  Serial.begin(115200);

  Serial.println("初始化 MPU6050...");
  accelgyro.initialize();

  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 連線成功");
  } else {
    Serial.println("MPU6050 連線失敗");
    while (1);
  }

  calibrateMPU6050(); // 執行 offset 校正
}

void loop() {
  int16_t ax_corr, ay_corr, az_corr;
  int16_t gx_corr, gy_corr, gz_corr;

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // 套用 offset 修正
  ax_corr = ax - ax_offset;
  ay_corr = ay - ay_offset;
  az_corr = az - az_offset;
  gx_corr = gx - gx_offset;
  gy_corr = gy - gy_offset;
  gz_corr = gz - gz_offset;

  Serial.print("a/g corrected:\t");
  Serial.print(ax_corr); Serial.print("\t");
  Serial.print(ay_corr); Serial.print("\t");
  Serial.print(az_corr); Serial.print("\t");
  Serial.print(gx_corr); Serial.print("\t");
  Serial.print(gy_corr); Serial.print("\t");
  Serial.println(gz_corr);

  delay(500);
}
