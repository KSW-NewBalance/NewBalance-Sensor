#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ThingsBoard.h>
#include <ArduinoJson.h>


#define FORCE_SENSOR_PIN1 36 // ESP32 pin GIOP36 (ADC0): the FSR and 10K pulldown are connected to A0
#define FORCE_SENSOR_PIN2 39 // ESP32 pin GIOP39 (ADC3): the FSR and 10K pulldown are connected to A3
#define FORCE_SENSOR_PIN3 34 // ESP32 pin GIOP36 (ADC6): the FSR and 10K pulldown are connected to A6
#define FORCE_SENSOR_PIN4 35 // ESP32 pin GIOP36 (ADC7): the FSR and 10K pulldown are connected to A7


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
constexpr char TOKEN[] PROGMEM = "YOUR_DEVICE_TOKEN";
// Thingsboard we want to establish a connection too
constexpr char THINGSBOARD_SERVER[] PROGMEM = "YOUR_THINGSBOARD_SERVER_IP";
// MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883U;
// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint32_t MAX_MESSAGE_SIZE PROGMEM = 256U;   // Increased 128 -> 256
// Initialize underlying client, used to establish a connection
WiFiClient espClient;
// Initialize ThingsBoard instance with the maximum needed buffer size
ThingsBoardSized<MAX_MESSAGE_SIZE> tb(espClient);
//create mpu object
Adafruit_MPU6050 mpu;
// create variables for save FSR signal
int fsrReading1, fsrReading2, fsrReading3, fsrReading4;
// Statuses for subscribing to shared attributes
bool subscribed = false;
// Change when Start/Stop Button is clicked
bool runningStatus = false;

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

/// @brief Initalizes WiFi connection,
// will endlessly delay until a connection has been successfully established
void InitWiFi() {
  Serial.println("Connecting to AP ...");
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been succesfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
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

void processRunningStatusUpdate(const Shared_Attribute_Data &data) {
  for(auto it = data.begin(); it != data.end(); ++it) {
    Serial.println(it->key().c_str());
    // Shared attributes have to be parsed by their type.
    Serial.println(it->value().as<const char*>());
  }
  int jsonSize = JSON_STRING_SIZE(measureJson(data));
  char buffer[jsonSize];
  serializeJson(data, buffer, jsonSize);
  Serial.println(buffer);
  runningStatus = data["isRunning"];
}

//callback when the shared attribute is changed
const Shared_Attribute_Callback callback(SUBSCRIBED_SHARED_ATTRIBUTES.cbegin(), SUBSCRIBED_SHARED_ATTRIBUTES.cend(), processRunningStatusUpdate);


void setup() {
  // If analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  // Initialize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);
  InitWiFi();
  InitMPU();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    reconnect();
  }

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
    Serial.println("Subscribing for shared attribute updates...");
    if(!tb.Shared_Attributes_Subscribe(callback)) {
      Serial.println("Failed to subscribe for shared attribute updates");
      return;
    }
    Serial.println("Subscribe done");
    subscribed = true;
  }

  if (runningStatus) {
    sendData(); 
  }
  tb.loop();
}

void sendData(){
  sensors_event_t a, g, temp;    //instances to read sensor value
  // Reading Start
  mpu.getEvent(&a, &g, &temp);
  fsrReading1 = analogRead(FORCE_SENSOR_PIN1);
  fsrReading2 = analogRead(FORCE_SENSOR_PIN2);
  fsrReading3 = analogRead(FORCE_SENSOR_PIN3);
  fsrReading4 = analogRead(FORCE_SENSOR_PIN4); 
  // Make Json Document to send data
  StaticJsonDocument<500> SensorValues;
  SensorValues["acc_x"] = a.acceleration.x;   // MPU6050's acceleration value in x-axis direction
  SensorValues["acc_y"] = a.acceleration.y;   // MPU6050's acceleration value in y-axis direction
  SensorValues["acc_z"] = a.acceleration.z;   // MPU6050's acceleration value in z-axis direction
  SensorValues["gyro_x"] = g.gyro.x;   // MPU6050's gyro sensor value in x-axis direction
  SensorValues["gyro_y"] = g.gyro.y;   // MPU6050's gyro sensor value in y-axis direction
  SensorValues["gyro_z"] = g.gyro.z;   // MPU6050's gyro sensor value in z-axis direction
  SensorValues["fsr_1st"] = fsrReading1;    // The forefoot's force sensitive resistor value
  SensorValues["fsr_2nd"] = fsrReading2;    // The midfoot - left's force sensitive resistor value
  SensorValues["fsr_3rd"] = fsrReading3;    // The midfoot - right's force sensitive resistor value
  SensorValues["fsr_4th"] = fsrReading4;    // The rearfoot's force sensitive resistor value
  // Start sending data
  char buffer[500];
  serializeJson(SensorValues, buffer);
  tb.sendTelemetryJson(buffer);
  // End sending data
  delay(10);  // wait for next input
}