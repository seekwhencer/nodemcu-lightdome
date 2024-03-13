#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

DynamicJsonDocument MQTT_callback(char* topic, byte* payload, unsigned int length, float &valueFactor, float &valueOffset, int &tickRate);
void MQTT_reconnect(PubSubClient &client);
void MQTT_publish(PubSubClient &client, DynamicJsonDocument &values, DynamicJsonDocument &valuesBefore, DynamicJsonDocument &valuesAgeBefore, float valueFactor, float valueOffset);
void MQTT_publishValue(char* field, float value, PubSubClient &client, DynamicJsonDocument &valuesBefore, DynamicJsonDocument &valuesAgeBefore, float valueFactor, float valueOffset);
void MQTT_publishValues(PubSubClient &client, DynamicJsonDocument &values, DynamicJsonDocument &valuesBefore, DynamicJsonDocument &valuesAgeBefore, float valueFactor, float valueOffset);
float roundFloat(float val, int offset);
