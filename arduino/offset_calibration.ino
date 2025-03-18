#include <Wire.h>
#include <MPU6050.h>

// Create an MPU6050 object
MPU6050 imu;

int16_t ax, ay, az, gx, gy, gz;
long gyroXOffset = 0, gyroYOffset = 0, gyroZOffset = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  imu.initialize();

  // Calibration loop
  Serial.println("Calibrating... Keep the sensor stationary!");
  for (int i = 0; i < 1000; i++) {
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    gyroXOffset += gx;
    gyroYOffset += gy;
    gyroZOffset += gz;

    delay(2);
  }

  gyroXOffset /= 1000;
  gyroYOffset /= 1000;
  gyroZOffset /= 1000;

  Serial.println("Calibration complete!");
  Serial.print("Offsets - X: ");
  Serial.print(gyroXOffset);
  Serial.print(", Y: ");
  Serial.print(gyroYOffset);
  Serial.print(", Z: ");
  Serial.println(gyroZOffset);
}

void loop() {
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  gx -= gyroXOffset;
  gy -= gyroYOffset;
  gz -= gyroZOffset;

  Serial.print("Gyro X: ");
  Serial.print(gx);
  Serial.print(", Y: ");
  Serial.print(gy);
  Serial.print(", Z: ");
  Serial.println(gz);

  delay(500);
}
