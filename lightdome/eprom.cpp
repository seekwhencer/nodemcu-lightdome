#include "eprom.h"

void EPROM_save(String topicSet, String encoded) {
  int address = EPROM_getAddress(topicSet);

  if(address == -1) {
    return;
  }

  Serial.print("EPROM SAVE PROP: ");
  Serial.print(topicSet);
  Serial.print(" AT ADDRESS: ");
  Serial.print(address);
  Serial.print(" WITH VALUE: ");
  Serial.println(encoded);

  EPROM_write(address, encoded);
}

void EPROM_write(int addr, String encoded) {
  const char* enc = encoded.c_str();
  unsigned int value = strtoul(enc, NULL, 2);

  EEPROM.write(addr, value);
  EEPROM.commit();
};

String EPROM_read(int addr) {
  byte value = EEPROM.read(addr);
  Serial.println(value);
  String result = String((char *)value);

  return result;
};

void EPROM_init() {
  Serial.println(">>>> EPROM INIT: ");
  //String read = EPROM_read(0);
  //Serial.println(read);
}

int EPROM_getAddress(String topicSet){
  DynamicJsonDocument addressMapping(256);
  addressMapping[SKWNCR_MQTT_TOPIC_SET_FACTOR] = 0;
  addressMapping[SKWNCR_MQTT_TOPIC_SET_OFFSET] = 1;
  addressMapping[SKWNCR_MQTT_TOPIC_SET_INTERVAL] = 2;

  if(addressMapping.containsKey(topicSet)){
    return addressMapping[topicSet];
  } else {
    return -1;
  }
}