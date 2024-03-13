#include "wifi.h"

void WIFI_login() {
  Serial.printf("WIFI connecting to %s ", SKWNCR_WIFI_SSID);
  //WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(SKWNCR_WIFI_HOSTNAME);
  WiFi.begin(SKWNCR_WIFI_SSID, SKWNCR_WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(SKWNCR_WIFI_RECONNECT_TIME);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("");
  Serial.println("WIFI CONNECTED");
  Serial.println("");
  
  Serial.print("IP:       ");
  Serial.println(WiFi.localIP());

  Serial.printf("Hostname: %s \n", SKWNCR_WIFI_HOSTNAME);
  
  Serial.print("Netmask:  ");
  Serial.println(WiFi.subnetMask());
  
  Serial.print("Gateway:  ");
  Serial.println(WiFi.gatewayIP());

  delay(SKWNCR_WIFI_RECONNECT_TIME);
}
