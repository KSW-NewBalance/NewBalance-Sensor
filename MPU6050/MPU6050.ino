#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// create mpu object
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  while(!Serial){
    delay(100);
  }

  // initialize
  if (!mpu.begin()){
    Serial.println("Failed to find mpu chip");
    while(1){
      delay(100);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);  // 가속도계의 측정 범위 설정: 16g
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);        // 자이로스코프의 측정 범위 설정: 250 dps
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);     // 디지털 low pass filter 대역폭 설정
  Serial.println("");
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;    // 센서를 읽기 위한 instance 생성
  mpu.getEvent(&a, &g, &temp);

  // Adafruit lib 
  Serial.print("AccelX: "); Serial.print(a.acceleration.x); Serial.print(", ");
  Serial.print("AccelY: "); Serial.print(a.acceleration.y); Serial.print(", ");
  Serial.print("AccelZ: "); Serial.print(a.acceleration.z); Serial.print(", ");
  Serial.print("GyroX: "); Serial.print(g.gyro.x); Serial.print(", ");
  Serial.print("GyroY: "); Serial.print(g.gyro.y); Serial.print(", ");
  Serial.print("GyroZ: "); Serial.print(g.gyro.z); Serial.println("");

  delay(100);
}
