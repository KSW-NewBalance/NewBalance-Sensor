#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define FORCE_SENSOR_PIN1 36 // ESP32 pin GIOP36 (ADC0): the FSR and 10K pulldown are connected to A0
#define FORCE_SENSOR_PIN2 39 // ESP32 pin GIOP39 (ADC3): the FSR and 10K pulldown are connected to A3
#define FORCE_SENSOR_PIN3 34 // ESP32 pin GIOP36 (ADC6): the FSR and 10K pulldown are connected to A6
#define FORCE_SENSOR_PIN4 35 // ESP32 pin GIOP36 (ADC7): the FSR and 10K pulldown are connected to A7

//create mpu object
Adafruit_MPU6050 mpu;
// create variables for save FSR signal
int fsrReading1, fsrReading2, fsrReading3, fsrReading4;

/// @brief Initialzes Inertial Measurement Sensor,
/// setting accelerometer and gyroscope range, and filter's bandwidth
void InitMPU() {
  Wire.begin();
  while(!Serial){
    delay(100);
  }

  if(!mpu.begin()) {
    Serial.println("Failed to find mpu chip");
    while(1) {
      delay(100);
    }
  }

  // Setting MPU start
  // The measurement range of accelerometer : 16g
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  // The measurement range of Gyroscope: 250 dps
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  // Bandwidth of digital low pass filter (for callibration)
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // Setting MPU end
  Serial.println("");
  delay(100);
}


void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200U);
  delay(1000);
  InitMPU();
}

void loop() {
  sensors_event_t a, g, temp;    //instances to read sensor value
  // Reading Start
  mpu.getEvent(&a, &g, &temp);
  fsrReading1 = analogRead(FORCE_SENSOR_PIN1);
  fsrReading2 = analogRead(FORCE_SENSOR_PIN2);
  fsrReading3 = analogRead(FORCE_SENSOR_PIN3);
  fsrReading4 = analogRead(FORCE_SENSOR_PIN4); 

  Serial.print("acc_x: "); Serial.print(a.acceleration.x); Serial.print(" ");
  Serial.print("acc_y: "); Serial.print(a.acceleration.y); Serial.print(" ");
  Serial.print("acc_z: "); Serial.print(a.acceleration.z); Serial.println("");

  Serial.print("gyro_x: "); Serial.print(g.gyro.x); Serial.print(" ");
  Serial.print("gyro_y: "); Serial.print(g.gyro.y); Serial.print(" ");
  Serial.print("gyro_z: "); Serial.print(g.gyro.z); Serial.println("");

  Serial.print("fsr1: "); Serial.print(fsrReading1); Serial.print(" ");
  Serial.print("fsr2: "); Serial.print(fsrReading2); Serial.print(" ");
  Serial.print("fsr3: "); Serial.print(fsrReading3); Serial.print(" ");
  Serial.print("fsr4: "); Serial.print(fsrReading4); Serial.println("");

  Serial.println("");
  
  delay(10);
}

