#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <esp_task_wdt.h>
#include <ArduinoJson.h>
#include "OTA.h"
#include <EEPROM.h>

// define the number of bytes you want to access
#define EEPROM_SIZE 512

DynamicJsonDocument jsonDoc_send(1024);

//------Wifi Information---------------

// const char* ssid = "MEIGROUP_DC2.4";     //Wifi connect
// const char* password = "meigroupiot88";  //Password

// const char* ssid = "MEIGROUP01 - 2.4GHZ";  //Wifi connect
// const char* password = "77namhai88";       //Password

const char* ssid = "MEIGROUP COMPANY";
const char* password = "@meigroup2025";


//------MQTT Information---------------
const char* mqtt_server = "meigroup2025.ddns.net";
const int mqtt_port = 1883;
const char* mqtt_username = "meigroup";  //User
const char* mqtt_password = "@778899";   //Password

const char* TOPIC = "MEIGROUP/RD/TB6";

bool khoidong = 0;

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t connect_mqtt = 0;

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, 1);
    Serial.print(".");
    delay(500);
    if(STATE_RELAY_REF != STATE_RELAY){
      STATE_RELAY_REF = STATE_RELAY;
      EEPROM.write(0, STATE_RELAY);
      EEPROM.commit();
    }
  } 
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientID = "ESPClient-";
    clientID += String(random(0xffff), HEX);
    connect_mqtt = 0;
    if (client.connect(clientID.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe(TOPIC);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 500 ms");
      delay(500);
      Serial.println();
      digitalWrite(LED, 1);
      if(STATE_RELAY_REF != STATE_RELAY){
        STATE_RELAY_REF = STATE_RELAY;
        EEPROM.write(0, STATE_RELAY);
        EEPROM.commit();
      }
    }
  }
}

void publishMessage(const char* topic, String payload) {
  client.publish(topic, payload.c_str());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage += (char)payload[i];
  //Serial.println("Massage arived [" + String(topic) + "]" + incommingMessage);

  if (khoidong == 1) {
    if (String(topic) == TOPIC) {
      if (payload[0] == '1') {
        RELAY_ON;
        STATE_RELAY_REF = STATE_RELAY = 1;
        EEPROM.write(0, STATE_RELAY);
        EEPROM.commit();
      }
      if (payload[0] == '0') {
        RELAY_OFF;
        STATE_RELAY_REF = STATE_RELAY = 0;
        EEPROM.write(0, STATE_RELAY);
        EEPROM.commit();
      }
    }
  }
  khoidong = 1;
}

void Pus_Info() {
  jsonDoc_send["IP"] = WiFi.localIP().toString();
  jsonDoc_send["RSSI"] = WiFi.RSSI();
  jsonDoc_send["MAC"] = WiFi.macAddress();
  jsonDoc_send["SSID"] = WiFi.SSID();
  String jsonString_send;
  serializeJson(jsonDoc_send, jsonString_send);
  publishMessage("MEIGROUP/OTA/RD/DENTRAN2", jsonString_send.c_str());
  jsonDoc_send.clear();
}