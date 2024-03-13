#include "mqtt.h"


// ---------------------------------------------
// THE RECEIVE CALLBACK
// ---------------------------------------------
DynamicJsonDocument MQTT_callback(char* top, byte* payload, unsigned int length, float &valueFactor, float &valueOffset, int &tickRate) {
  String encoded;
  char* slash = (char*) "/";
  DynamicJsonDocument topicStack(2048);

  for (int i = 0; i < length; i++) {
    encoded += (char)payload[i];
  }

  Serial.print("MQTT RECEIVED: ");
  Serial.print(top);
  Serial.print(" WITH VALUE: ");
  Serial.println(encoded);

  char* d = strtok(top, "/");
  int i = 0;
  while (d != NULL) {
    topicStack[i] = d;
    i++;
    d = strtok(NULL, "/");
  }

  int lastStackNum = topicStack.size() - 1;
  String topicSet = topicStack[lastStackNum];

  if(topicSet == SKWNCR_MQTT_TOPIC_SET_FACTOR){
    valueFactor = encoded.toFloat();
  }
  if(topicSet == SKWNCR_MQTT_TOPIC_SET_OFFSET){
    valueOffset = encoded.toFloat();
  }
  if(topicSet == SKWNCR_MQTT_TOPIC_SET_INTERVAL){
    tickRate = encoded.toInt();
  }
  
  DynamicJsonDocument result(2048);
  result["topicSet"] = topicSet;
  result["encoded"] = encoded;
  return result;
}


// ---------------------------------------------
// (RE)CONNECT
// ---------------------------------------------
void MQTT_reconnect(PubSubClient &client) {
  char subscribeTopicSet[256] = SKWNCR_MQTT_TOPIC;
  strcat(subscribeTopicSet, "/set/#");

  while (!client.connected()) {
    Serial.println("");
    Serial.printf("MQTT CONNECTING TO %s ... \n", SKWNCR_MQTT_SERVER);

    if (client.connect(SKWNCR_MQTT_CLIENT_NAME)) {
      Serial.println("MQTT CONNECTED");
      Serial.printf("MQTT CLIENT NAME: %s \n", SKWNCR_MQTT_CLIENT_NAME);
      //client.publish(SKWNCR_MQTT_TOPIC, SKWNCR_MQTT_WELCOME);
      
      client.subscribe(subscribeTopicSet);
      
      Serial.print("MQTT SUBSCRIBED:");
      Serial.println(subscribeTopicSet);

    } else {
      Serial.print("MQTT CONNECTION FAILED, rc=");
      Serial.print(client.state());
      Serial.printf("waiting %s ms \n", SKWNCR_MQTT_RECONNECT_TIME);

      delay(SKWNCR_MQTT_RECONNECT_TIME);
    }
  }
}


// ---------------------------------------------
// PUBLISH
// ---------------------------------------------
void MQTT_publish(PubSubClient &client, DynamicJsonDocument &values, DynamicJsonDocument &valuesBefore, DynamicJsonDocument &valuesAgeBefore, float valueFactor, float valueOffset){
  MQTT_publishValues(client, values, valuesBefore, valuesAgeBefore, valueFactor, valueOffset);
}


// ---------------------------------------------
// PUBLISH VALUES
// ---------------------------------------------
void MQTT_publishValues(PubSubClient &client, DynamicJsonDocument &values, DynamicJsonDocument &valuesBefore, DynamicJsonDocument &valuesAgeBefore, float valueFactor, float valueOffset){
  MQTT_publishValue("01", values[0], client, valuesBefore, valuesAgeBefore, valueFactor, valueOffset);
  MQTT_publishValue("02", values[1], client, valuesBefore, valuesAgeBefore, valueFactor, valueOffset);
  MQTT_publishValue("03", values[2], client, valuesBefore, valuesAgeBefore, valueFactor, valueOffset);
  MQTT_publishValue("04", values[3], client, valuesBefore, valuesAgeBefore, valueFactor, valueOffset);
} 


// ---------------------------------------------
// PUBLISH VALUE
// ---------------------------------------------
void MQTT_publishValue(char* field, float value, PubSubClient &client, DynamicJsonDocument &valuesBefore, DynamicJsonDocument &valuesAgeBefore, float valueFactor, float valueOffset) {
  int ms = millis();
  char valString[256];
  char* slash = (char*) "/";
  char subTopic[256] = "";
  strcat(subTopic, SKWNCR_MQTT_TOPIC);
  strcat(subTopic, slash);
  strcat(subTopic, field);
    
  float rounded = roundFloat(((value * valueFactor) + valueOffset), 4);

  if(!valuesBefore[field]){
    SKWNCR_DEBUG ? Serial.print("NOT EXISTS: ") : NULL;
    SKWNCR_DEBUG ? Serial.println(field) : NULL;
    valuesBefore[field] = rounded;
    valuesAgeBefore[field] = ms;
  }

  int age = ms - valuesAgeBefore[field].as<int>();
  
  //SKWNCR_DEBUG ? Serial.print(field) : NULL;
  //SKWNCR_DEBUG ? Serial.print(" >>> ") : NULL;
  //SKWNCR_DEBUG ? Serial.print(ms) : NULL;
  //SKWNCR_DEBUG ? Serial.print(" | ") : NULL;
  //SKWNCR_DEBUG ? Serial.println(age) : NULL;
  //SKWNCR_DEBUG ? Serial.println("") : NULL;
  
  if(valuesBefore[field] == rounded && age < SKWNCR_VALUE_AGE){
    //SKWNCR_DEBUG ? Serial.print(">>> Skipping: ") : NULL;
    //SKWNCR_DEBUG ? Serial.print(field) : NULL;
    //SKWNCR_DEBUG ? Serial.print(" : ") : NULL;
    //SKWNCR_DEBUG ? Serial.println(rounded) : NULL;
    //SKWNCR_DEBUG ? Serial.println("----") : NULL;
    //SKWNCR_DEBUG ? Serial.println("") : NULL;
    return;
  }
  
  valuesBefore[field] = rounded;
  valuesAgeBefore[field] = ms;
  
  snprintf(valString, sizeof(valString), "%g", rounded);

  SKWNCR_DEBUG ? Serial.print("Send on Topic: ") : NULL;
  SKWNCR_DEBUG ? Serial.print(subTopic) : NULL;
  SKWNCR_DEBUG ? Serial.print(": ") : NULL;
  SKWNCR_DEBUG ? Serial.println(valString) : NULL;
 
  client.publish(subTopic, valString);
  
  //SKWNCR_DEBUG ? Serial.println("----") : NULL;
  //SKWNCR_DEBUG ? Serial.println("") : NULL;
}


// ---------------------------------------------
// HELPER
// ---------------------------------------------
float roundFloat(float val, int offset) {
  int th = 10^offset;
  float rounded = ceilf(val * th) / th;
  return rounded;
}
