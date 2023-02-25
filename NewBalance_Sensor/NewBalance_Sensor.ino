#include <Adafruit_MPU6050.h>     // library for Inertial Measurement Unit(MPU6050) object
#include <Adafruit_Sensor.h>      // library for common sensor
#include <Wire.h>                 // library for communicate with I2C
#include <WiFi.h>                 // library for WiFi connect
#include <WiFiClientSecure.h>     // library for WiFi Secure
#include <ThingsBoard.h>          // library for connect ThingsBoard and ESP32
#include <ArduinoJson.h>          // library for JSON format(MQTT)


#define FORCE_SENSOR_PIN1 36 // ESP32 pin GIOP36 (ADC0): the FSR and 10K pulldown are connected to A0
#define FORCE_SENSOR_PIN2 39 // ESP32 pin GIOP39 (ADC3): the FSR and 10K pulldown are connected to A3
#define FORCE_SENSOR_PIN3 34 // ESP32 pin GIOP36 (ADC6): the FSR and 10K pulldown are connected to A6
#define FORCE_SENSOR_PIN4 35 // ESP32 pin GIOP36 (ADC7): the FSR and 10K pulldown are connected to A7
#define LED 2                // ESP32's built-in LED for checking status

// ESP32 will send data if "isRunning" is changed to 'true'
// the callback will be called for every shared attribute changed on the device
constexpr std::array<const char*, 1U> SUBSCRIBED_SHARED_ATTRIBUTES = {
  "isRunning"
};

// Baud rate for the debugging serial connection
constexpr uint32_t SERIAL_DEBUG_BAUD PROGMEM = 115200U;
// WiFi information
constexpr char WIFI_SSID[] PROGMEM = "YOUR_WIFI_SSID_NAME";
constexpr char WIFI_PASSWORD[] PROGMEM = "YOUR_WIFI_PASSWORD";
// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
constexpr char TOKEN[] PROGMEM = "YOUR_THINGSBOARD_DEVICE_TOKEN";
// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] PROGMEM = "YOUR_THINGBOARD_SERVER_IP";
// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
// Maximum size packets will ever be sent or received by the underlying MQTT client
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 256U;
// Initialize underlying client, used to establish a connection
WiFiClient espClient;
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoardSized<MAX_MESSAGE_SIZE> tb(espClient);
//create mpu object
Adafruit_MPU6050 mpu;
// create variables for save FSR signal
int fsrReading1, fsrReading2, fsrReading3, fsrReading4;
// create variables for save IMU signal,
//instances to read sensor value
// a: accelerometer, g: gyroscope, temp : temmperature
sensors_event_t a, g, temp;   

// Statuses for subscribing shared attributes
bool subscribed = false;
// Change when Start/Stop Button is clicked
// true: user is running & false: user is stop
bool runningStatus = false;

/// @brief Initialzes Inertial Measurement Sensor,
/// setting accelerometer and gyroscope range, and filter's bandwidth
void InitMPU() {
  Wire.begin();
  while(!Serial){
    delay(100);
  }
  // finding MPU
  if(!mpu.begin()) {
    Serial.println("Failed to find mpu chip");   // code for debugging in Serial Monitor: when ESP failed to find MPU
    while(1) {
      delay(100);
    }
  }

  // MPU Setting start
  // The measurement range of accelerometer : 16g
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  // The measurement range of Gyroscope: 250 dps
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  // Bandwidth of digital low pass filter (for callibration)
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  // Setting MPU end
  Serial.println("");   // code for debugging in Serial Monitor
  delay(100);
}

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // checking WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been succesfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
  // if WiFi is connected, built-in LED blinks once
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
const bool reconnect() {
  // Check to ensure we aren't connected yet
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }
  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}
/// @brief callback function that called when the ThingsBoard's shared attribute is changed,
/// in this code, variable 'runningStatus' value will be replaced to "isRunning" value.
void processRunningStatusUpdate(const Shared_Attribute_Data &data) {
  // replace runningStatus's value to given isRunnning's value
  runningStatus = data["isRunning"];
  // check the runningStatus
  if(!runningStatus){
    // if runningStatus is _false_ send message to ThingsBoard that ESP32 is also stopped.
    tb.sendTelemetryData("isStop", true);
  }
  // When the shared attribute 'isRunning' is changed, built-in LED blinks 3 times.
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
}

// callback when the shared attribute is changed
// it will check all shared attributes that includes in array "SUBSCRIBED_SHARED_ATTRIBUTES"
const Shared_Attribute_Callback callback(SUBSCRIBED_SHARED_ATTRIBUTES.cbegin(), SUBSCRIBED_SHARED_ATTRIBUTES.cend(), processRunningStatusUpdate);


// setup function for ESP32
void setup() {
  // If analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  // Initialize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  // set built-in LED for OUTPUT
  pinMode(LED, OUTPUT);
  // wait for a second
  delay(1000);
  InitWiFi();     // WiFi initializing
  InitMPU();      // MPU initializing
}

// ESP32 will conduct this code during its power is on
void loop() {
  // if the MPU6050's connection is lost, it will initialize MPU again.
  if(!mpu.begin()){
    InitMPU();
  }
  // if WiFi connection is lost, it will reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    reconnect();
  }
  // if ESP32 is not connected to ThingsBoard, it will try to connect with
  // ThinsgBoard Server IP, Device token, ThingsBoard MQTT port
  if(!tb.connected()) {
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if(!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }

  }
  if(!subscribed) {
    tb.Shared_Attributes_Subscribe(callback);
    subscribed = true;
    // checking for subscription status, if subscribing success, built-in led will blink twice.
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }
  // counting variable for sendingData
  int count = 0; 
  // when runningStatus is true, it will send data for 1s
  if (runningStatus) {
    while(count < 100){
      count++;
      sendData();
    }
  }
  tb.loop();
}

void sendData(){
  // Reading Start
  mpu.getEvent(&a, &g, &temp);
  fsrReading1 = analogRead(FORCE_SENSOR_PIN1);
  fsrReading2 = analogRead(FORCE_SENSOR_PIN2);
  fsrReading3 = analogRead(FORCE_SENSOR_PIN3);
  fsrReading4 = analogRead(FORCE_SENSOR_PIN4); 
  // Reading End

  // Make Json Document to send data
  StaticJsonDocument<500> SensorValues;
  SensorValues["acc_x"] = a.acceleration.x;   // MPU6050's acceleration value in x-axis direction
  SensorValues["acc_y"] = a.acceleration.y;   // MPU6050's acceleration value in y-axis direction
  SensorValues["acc_z"] = a.acceleration.z;   // MPU6050's acceleration value in z-axis direction
  SensorValues["gyro_x"] = g.gyro.x;          // MPU6050's gyro sensor value in x-axis direction
  SensorValues["gyro_y"] = g.gyro.y;          // MPU6050's gyro sensor value in y-axis direction
  SensorValues["gyro_z"] = g.gyro.z;          // MPU6050's gyro sensor value in z-axis direction
  SensorValues["fsr_1st"] = fsrReading1;      // The forefoot's force sensitive resistor value
  SensorValues["fsr_2nd"] = fsrReading2;      // The midfoot - left's force sensitive resistor value
  SensorValues["fsr_3rd"] = fsrReading3;      // The midfoot - right's force sensitive resistor value
  SensorValues["fsr_4th"] = fsrReading4;      // The rearfoot's force sensitive resistor value

  // Start sending data
  char buffer[500];
  serializeJson(SensorValues, buffer);
  tb.sendTelemetryJson(buffer);
  // End sending data

  delay(10);    //wait 0.01s for sending next data
}