#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


#define FORCE_SENSOR_PIN1 36 // ESP32 pin GIOP36 (ADC0): the FSR and 10K pulldown are connected to A0
#define FORCE_SENSOR_PIN2 39 // ESP32 pin GIOP39 (ADC3): the FSR and 10K pulldown are connected to A3
#define FORCE_SENSOR_PIN3 34 // ESP32 pin GIOP36 (ADC6): the FSR and 10K pulldown are connected to A6
#define FORCE_SENSOR_PIN4 35 // ESP32 pin GIOP36 (ADC7): the FSR and 10K pulldown are connected to A7


// Baud rate for the debugging serial connection
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;





//create mpu object
Adafruit_MPU6050 mpu;
// create variables for save FSR signal
int analogReading1, analogReading2, analogReading3, analogReading4;

void setup() {
  // Initialize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  Wire.begin();
  while(!Serial){
    delay(100);
  }

  if(!mpu.begin()) {
    // Print error message
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

void loop() {
  sensors_event_t a, g, temp;    //instances to read sensor value
  mpu.getEvent(&a, &g, &temp);
  analogReading1 = analogRead(FORCE_SENSOR_PIN1);
  analogReading2 = analogRead(FORCE_SENSOR_PIN2);
  analogReading3 = analogRead(FORCE_SENSOR_PIN3);
  analogReading4 = analogRead(FORCE_SENSOR_PIN4); 

  // Print of MPU
  Serial.print("MPU value [");
  // Adafruit lib
  Serial.print("AccelX: "); Serial.print(a.acceleration.x); Serial.print(", ");
  Serial.print("AccelY: "); Serial.print(a.acceleration.y); Serial.print(", ");
  Serial.print("AccelZ: "); Serial.print(a.acceleration.z); Serial.print(", ");
  Serial.print("GyroX: "); Serial.print(g.gyro.x); Serial.print(", ");
  Serial.print("GyroY: "); Serial.print(g.gyro.y); Serial.print(", ");
  Serial.print("GyroZ: "); Serial.print(g.gyro.z); Serial.print("]\t");
  // Print of FSR
  Serial.print("FSR value ["); 
  Serial.print("1st FSR: "); Serial.print(analogReading1); Serial.print(", ");
  Serial.print("2nd FSR: "); Serial.print(analogReading2); Serial.print(", ");
  Serial.print("3rd FSR: "); Serial.print(analogReading3); Serial.print(", ");
  Serial.print("4th FSR: "); Serial.print(analogReading4); Serial.println("]");

  delay(100);

}
