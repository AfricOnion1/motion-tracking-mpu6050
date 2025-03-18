#include <Wire.h>
#include <MPU6050.h>

// MPU6050 object
MPU6050 imu;

const int16_t gyroXOffset = -503;
const int16_t gyroYOffset = 29;
const int16_t gyroZOffset = 101;

const int16_t accXOffset  = 0;
const int16_t accYOffset  = 0;
const int16_t accZOffset  = 0;

double roll = 0, pitch = 0, yaw = 0;
unsigned long lastTime = 0;

double smoothedRoll = 0, smoothedPitch = 0, smoothedYaw = 0;

const double smoothFactor = 0.8;  

void setup() {
  Serial.begin(115200);
  Wire.begin();

  imu.initialize();
  if (!imu.testConnection()) {
    Serial.println("Failed to connect to MPU6050!");
    while (1);
  }
  Serial.println("MPU6050 connected successfully!");

  imu.setDLPFMode(MPU6050_DLPF_BW_5);
  lastTime = millis();
}

void loop() {
  int16_t axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw;

  // Get raw sensor readings
  imu.getMotion6(&axRaw, &ayRaw, &azRaw, &gxRaw, &gyRaw, &gzRaw);

  gxRaw -= gyroXOffset;
  gyRaw -= gyroYOffset;
  gzRaw -= gyroZOffset;
  axRaw -= accXOffset;
  ayRaw -= accYOffset;
  azRaw -= accZOffset;

  unsigned long currentTime = millis();
  double deltaTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;


  // Calculate angles from the accelerometer
  double accelRoll  = atan2((double)ayRaw, (double)azRaw + 1e-6) * 180.0 / PI;
  double accelPitch = atan2(-(double)axRaw,
                            sqrt((double)ayRaw * (double)ayRaw + 
                                 (double)azRaw * (double)azRaw) + 1e-6)
                      * 180.0 / PI;

  // Integrate gyroscope data to get angles.
  roll  += (gxRaw / 131.0) * deltaTime;
  pitch += (gyRaw / 131.0) * deltaTime;
  yaw   += (gzRaw / 131.0) * deltaTime;

  const double alpha = 0.98;
  roll  = alpha * roll + (1.0 - alpha) * accelRoll;
  pitch = alpha * pitch + (1.0 - alpha) * accelPitch;

  smoothedRoll  = smoothFactor * smoothedRoll  + (1.0 - smoothFactor) * roll;
  smoothedPitch = smoothFactor * smoothedPitch + (1.0 - smoothFactor) * pitch;
  smoothedYaw   = smoothFactor * smoothedYaw   + (1.0 - smoothFactor) * yaw;

  // Print out the final smoothed angles
  Serial.print(smoothedRoll);  Serial.print(",");
  Serial.print(smoothedPitch); Serial.print(",");
  Serial.println(smoothedYaw);

  delay(5);
}
