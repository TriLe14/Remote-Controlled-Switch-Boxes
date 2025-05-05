#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "OTA.h"
#include "IC555.h"
#include <EEPROM.h>

// define the number of bytes you want to access
#define EEPROM_SIZE 512

DynamicJsonDocument jsonDoc_send(1024);

//------Wifi Information---------------

// const char* ssid = "MEIGROUP01 - 2.4GHZ";  //Wifi connect
// const char* password = "77namhai88";       //Password

const char* ssid = "MEIGROUP COMPANY";
const char* password = "@meigroup2025";

//------MQTT Information---------------
const char* mqtt_server = "meigroup2025.ddns.net";
const int mqtt_port = 1883;
const char* mqtt_username = "meigroup";  //User
const char* mqtt_password = "@778899";   //Password

const char* TOPIC = "MEIGROUP/SMARTHOME/CEO/DEN1";
const char* TOPIC2 = "MEIGROUP/SMARTHOME/CEO/DEN2";

uint8_t khoidong = 0;

WiFiClient espClient;
PubSubClient client(espClient);

uint8_t connect_mqtt = 0;

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if(STATE_RELAY_REF != STATE_RELAY){
      STATE_RELAY_REF = STATE_RELAY;
      EEPROM.write(0, STATE_RELAY);
      EEPROM.commit();
    }
    if(STATE_LED2_REF != STATE_LED2){
      STATE_LED2_REF = STATE_LED2;
      EEPROM.write(2, STATE_LED2);
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
      client.subscribe(TOPIC2);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 200ms");
      delay(500);
      Serial.println();
      IC555_Set();

      if(STATE_RELAY_REF != STATE_RELAY){
        STATE_RELAY_REF = STATE_RELAY;
        EEPROM.write(0, STATE_RELAY);
        EEPROM.commit();
      }

      if(STATE_LED2_REF != STATE_LED2){
        STATE_LED2_REF = STATE_LED2;
        EEPROM.write(2, STATE_LED2);
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

  if (khoidong >= 2) {
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

    if (String(topic) == TOPIC2) {
      if (payload[0] == '1') {
        STATE_LED2_REF = STATE_LED2 = 1;
        EEPROM.write(2, STATE_LED2);
        EEPROM.commit();
      }
      if (payload[0] == '0') {
        STATE_LED2_REF = STATE_LED2 = 0;
        EEPROM.write(2, STATE_LED2);
        EEPROM.commit();
      }
    }
  } else {
    khoidong++;
  }
}

void Pus_Info() {
  jsonDoc_send["IP"] = WiFi.localIP().toString();
  jsonDoc_send["RSSI"] = WiFi.RSSI();
  jsonDoc_send["MAC"] = WiFi.macAddress();
  jsonDoc_send["SSID"] = WiFi.SSID();
  String jsonString_send;
  serializeJson(jsonDoc_send, jsonString_send);
  publishMessage("MEIGROUP/OTA/CEO/DEN", jsonString_send.c_str());
  jsonDoc_send.clear();
}