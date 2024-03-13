#include "config.h"
#include "eprom.h"
#include "wifi.h"
#include "mqtt.h"
#include "ads1115.h"


// WIFI
WiFiClient wifiClient;

// MQTT
float valueFactor = SKWNCR_VALUE_FACTOR;
float valueOffset = SKWNCR_VALUE_OFFSET;
int tickRate = SKWNCR_TICK_RATE;

// MQTT CALLBACK
void mqttCB(char* top, byte* payload, unsigned int length) {
  DynamicJsonDocument data(2048);
  data = MQTT_callback(top, payload, length, valueFactor, valueOffset, tickRate);
  
  String topicSet = data["topicSet"];
  String encoded = data["encoded"];

  EPROM_save(topicSet, encoded);
}

// MQTT ADD CALLBACK
PubSubClient client(SKWNCR_MQTT_SERVER, SKWNCR_MQTT_PORT, mqttCB, wifiClient);

// Data
DynamicJsonDocument values(2048);
DynamicJsonDocument valuesBefore(2048);
DynamicJsonDocument valuesAgeBefore(4096);

// ADS1115 ADC
Adafruit_ADS1115 ads;

// ---------------------------------------------
// SETUP
// ---------------------------------------------
void setup() {

  // Serial
  while (!Serial) {} // Wait
  Wire.begin();
  Serial.begin(SKWNCR_BAUD_RATE);

  // EPROM
  EEPROM.begin(512);
  
  EPROM_init();

  // ADC
  ads.begin();
  ads.setGain(GAIN_TWOTHIRDS);
  
  // WIFI
  WIFI_login();
}


// ---------------------------------------------
// LOOP
// ---------------------------------------------
void loop() {

  // WIFI reconnect
  if (WiFi.status() != WL_CONNECTED) {
    WIFI_login();
  }

  // MQTT (re)connect
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      MQTT_reconnect(client);
    } else {
      client.loop();
    }
  }

  // get data
  ADS_read(ads, values); 

  // Finally
  MQTT_publish(client, values, valuesBefore, valuesAgeBefore, valueFactor, valueOffset);

  // tick rate
  delay(tickRate);
}
