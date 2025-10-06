#include <Wire.h>  // Wire library - used for I2C communication
#include <MPU6050.h>  // MPU6050 library for easier use

MPU6050 mpu;

void setup() {
  Serial.begin(115200);  // Start the serial communication at 115200 baud rate
  Wire.begin(21, 22);    // Start I2C on GPIO 21 (SDA) and GPIO 22 (SCL)

  mpu.initialize();  // Initialize the MPU6050 sensor

  // Check if the sensor is connected correctly
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);  // Stop execution if the sensor is not connected
  }
  Serial.println("MPU6050 initialized successfully");
}

void loop() {
  // Read accelerometer data from the MPU6050
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);  // Get raw accelerometer data

  // Convert the raw data to g's (assuming +/- 2g range, divide by 16384)
  float ax_g = ax / 16384.0;
  float ay_g = ay / 16384.0;
  float az_g = az / 16384.0;

  // Print the accelerometer data to the serial monitor
  /*Serial.print("Xa= ");
  Serial.println(ax_g);
  Serial.print("   Ya= ");
  Serial.println(ay_g);*/
  Serial.print("   Za= ");
  Serial.println(az_g);

  delay(50);  // Delay to slow down the loop
}
