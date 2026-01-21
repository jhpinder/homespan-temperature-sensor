#include <HomeSpan.h>
#include <DHT.h>

// DHT22 (AM2302) Configuration
#define DHT_PIN 15
#define DHT_TYPE DHT22

// Device Configuration
#define DEVICE_NAME "HomeSpan Temperature Sensor"
#define DEVICE_MANUFACTURER "HomeSpan"
#define DEVICE_SERIAL "HS-TEMP-001"
#define DEVICE_MODEL "AM2302"
#define DEVICE_VERSION "1.0.0"

// Sensor update interval in milliseconds (30 seconds)
#define UPDATE_INTERVAL 30000

DHT dhtSensor(DHT_PIN, DHT_TYPE);

struct TemperatureHumiditySensor : Service::TemperatureSensor {
  SpanCharacteristic *temperature;
  SpanCharacteristic *humidity;
  unsigned long lastUpdate;
  float lastTemp;
  float lastHumidity;
  bool sensorInitialized;

  TemperatureHumiditySensor() : Service::TemperatureSensor() {
    temperature = new Characteristic::CurrentTemperature(0.0);
    temperature->setRange(-40, 80);
    
    humidity = (new Characteristic::CurrentRelativeHumidity(0.0))->setRange(0, 100);
    
    lastUpdate = 0;
    lastTemp = 0.0;
    lastHumidity = 0.0;
    sensorInitialized = false;
    
    LOG1("Configured Temperature/Humidity Sensor on GPIO %d\n", DHT_PIN);
  }

  void loop() override {
    unsigned long currentTime = millis();
    
    // Check if it's time to update
    if (currentTime - lastUpdate < UPDATE_INTERVAL) {
      return;
    }
    
    lastUpdate = currentTime;
    
    // Read sensor data
    float temp = dhtSensor.readTemperature();
    float hum = dhtSensor.readHumidity();
    
    // Validate readings
    if (isnan(temp) || isnan(hum)) {
      if (sensorInitialized) {
        LOG1("Warning: Failed to read from DHT sensor\n");
      } else {
        LOG1("Initializing DHT sensor...\n");
      }
      return;
    }
    
    // Mark sensor as initialized after first successful read
    if (!sensorInitialized) {
      sensorInitialized = true;
      LOG1("DHT sensor initialized successfully\n");
    }
    
    // Only update if values have changed significantly
    if (abs(temp - lastTemp) >= 0.1 || abs(hum - lastHumidity) >= 1.0) {
      lastTemp = temp;
      lastHumidity = hum;
      
      temperature->setVal(temp);
      humidity->setVal(hum);
      
      LOG1("Temperature: %.1f°C, Humidity: %.1f%%\n", temp, hum);
    }
  }
};

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT sensor
  dhtSensor.begin();
  
  // Configure HomeSpan
  homeSpan.setLogLevel(1);
  homeSpan.begin(Category::Sensors, DEVICE_NAME);
  
  // Create accessory
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Manufacturer(DEVICE_MANUFACTURER);
      new Characteristic::SerialNumber(DEVICE_SERIAL);
      new Characteristic::Model(DEVICE_MODEL);
      new Characteristic::FirmwareRevision(DEVICE_VERSION);
    
    new TemperatureHumiditySensor();
  
  LOG1("HomeSpan Temperature/Humidity Sensor initialized\n");
}

void loop() {
  homeSpan.poll();
}