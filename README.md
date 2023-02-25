# Sensor Part

## Specific Information
### Device
|      |ESP32(Micro Controller Unit)|IMU(Inertial Measurement Unit)|FSR(Forse Sensitive Resistor)|
|------|----------------------------|------------------------------|-----------------------------|
|Model|[HiLetgo ESP-WROOM-32 Development Board](https://www.amazon.com/HiLetgo-ESP-WROOM-32-Development-Microcontroller-Integrated/dp/B0718T232Z/ref=sxin_24_ac_d_mf_brs?ac_md=1-0-SGlMZXRnbw%3D%3D-ac_d_mf_brs_brs&content-id=amzn1.sym.1ad31f34-ba12-4dca-be4b-f62f7f5bb10d%3Aamzn1.sym.1ad31f34-ba12-4dca-be4b-f62f7f5bb10d&cv_ct_cx=esp32&keywords=esp32&pd_rd_i=B0718T232Z&pd_rd_r=e9d91274-e832-4ac9-865f-5cd8e8316ac7&pd_rd_w=jJt5D&pd_rd_wg=oiPhr&pf_rd_p=1ad31f34-ba12-4dca-be4b-f62f7f5bb10d&pf_rd_r=QJMH0D1SBGFXJHQ4FTER&qid=1673556271&sr=1-1-8b2f235a-dddf-4202-bbb9-592393927392)|[HiLetgo GY-521 MPU-6050 3 Axis Accelerometer Gyroscope Module](https://www.amazon.com/dp/B01DK83ZYQ/ref=sspa_dk_hqp_detail_aax_0?sp_csd=d2lkZ2V0TmFtZT1zcF9ocXBfc2hhcmVk&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUFEVU5XQTJGWUg5QkQmZW5jcnlwdGVkSWQ9QTAyNzU4OTUzOVBRRzM5MDRDUUVDJmVuY3J5cHRlZEFkSWQ9QTA3ODM3MDIyMUJJQzU1WjFJNTJZJndpZGdldE5hbWU9c3BfaHFwX3NoYXJlZCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU&th=1)|[Force Sensitive Resistor MD30-60 0~30KG Diameter 30mm](https://www.amazon.com/dp/B07MP4RL9Q/ref=sspa_dk_detail_0?psc=1&pd_rd_i=B07MP4RL9Q&pd_rd_w=f5a29&content-id=amzn1.sym.46bad5f6-1f0a-4167-9a8b-c8a82fa48a54&pf_rd_p=46bad5f6-1f0a-4167-9a8b-c8a82fa48a54&pf_rd_r=P2RZ9YF8KBWWQMH2X7C2&pd_rd_wg=Vu32E&pd_rd_r=86688960-d7af-44ce-971d-d3de411725f5&s=industrial&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWw&smid=A2KRDQ1AI5Y5G6&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExNTBLR09HM05SWDhBJmVuY3J5cHRlZElkPUEwOTQzNzM2M0EyVzRWWEtCRTFRRSZlbmNyeXB0ZWRBZElkPUEwMDAxMjEwMkZKM1BINTVIUjdXMyZ3aWRnZXROYW1lPXNwX2RldGFpbCZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)|
|image|![ESP32](https://m.media-amazon.com/images/S/aplus-media-library-service-media/a468c2b2-235d-4bef-8b93-7c056e7286b9.__CR51,0,1457,901_PT0_SX970_V1___.jpg)|![MPU6050](https://m.media-amazon.com/images/I/61Xr7VGOl1L._SX522_.jpg)| ![FSR](https://m.media-amazon.com/images/I/51l9-+OLKJL._AC_SX679_.jpg)|


## Development Environment
```
Arduino IDE 2.0.3
1. Add BoardManager
   Preference > Additional boards manager URLs 
   > Add "https://dl.espressif.com/dl/package_esp32_index.json"
2. Before you upload code, select the board 'ESP32 Dev Module'
```
### Libraries
```
ArduinoJson (Version 6.20.1)
Adafruit BusIO (Version 1.14.0)
Adafruit GFX Library (Version 1.11.4)
Adafruit MPU6050 (Version 2.2.3)
Adafruit SSD1306 (Version 2.5.6)
Adafruit Unified Sensor (Version 1.1.6)
ThingsBoard (Version 0.9.4)
PubSubClient (Version 2.8.0)
ArduinoHttpClient (Version 0.3.2)
ArduinoJson (Version 6.20.1)
DHT sensor library for ESPx (Version 1.18.0)
```

## Circuit Diagram
<div>
<img src="https://user-images.githubusercontent.com/81208791/221369476-bd97e5ca-676e-43a9-adc2-9052e0505192.png" width="500"/>
<img src="https://user-images.githubusercontent.com/81208791/221369594-dd8d1f0a-dc62-4bc9-bd91-2b70e5f11594.png" width="500"/>
</div>
