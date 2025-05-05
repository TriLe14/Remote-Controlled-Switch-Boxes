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

// const char* ssid = "Tri Le";      //Wifi connect
// const char* password = "14052002";   //Password

const char* ssid = "MEIGROUP COMPANY";
const char* password = "@meigroup2025";

//------MQTT Information---------------
const char* mqtt_server = "meigroup2025.ddns.net";
const int mqtt_port = 1883;
const char* mqtt_username = "meigroup";  //User
const char* mqtt_password = "@778899";   //Password

const char* TOPIC1 = "BOX_DK_DEN_QUAT/TB1";
const char* TOPIC2 = "BOX_DK_DEN_QUAT/TB2";
const char* TOPIC3 = "BOX_DK_DEN_QUAT/TB3";
const char* TOPIC4 = "BOX_DK_DEN_QUAT/TB4";

uint8_t khoidong = 0;
uint8_t connect_mqtt = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, 1);
    Serial.print(".");
    delay(500);
    if(STATE_SPEAKER_OL != STATE_SPEAKER){
      STATE_SPEAKER_OL = STATE_SPEAKER;
      EEPROM.write(0, STATE_SPEAKER);
      EEPROM.commit();
    }
    if(STATE_LIGHT_OL != STATE_LIGHT){
      STATE_LIGHT_OL = STATE_LIGHT;
      EEPROM.write(1, STATE_LIGHT);
      EEPROM.commit();
    }
    if(STATE_FAN1_OL != STATE_FAN1){
      STATE_FAN1_OL = STATE_FAN1;
      EEPROM.write(2, STATE_FAN1);
      EEPROM.commit();
    }
    if(STATE_FAN2_OL != STATE_FAN2){
      STATE_FAN2_OL = STATE_FAN2;
      EEPROM.write(3, STATE_FAN2);
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
    if (client.connect(clientID.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected");
      client.subscribe(TOPIC1);
      client.subscribe(TOPIC2);
      client.subscribe(TOPIC3);
      client.subscribe(TOPIC4);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Try again in 500 ms");
      delay(500);
      Serial.println();
      digitalWrite(LED, 1);
      if(STATE_SPEAKER_OL != STATE_SPEAKER){
        STATE_SPEAKER_OL = STATE_SPEAKER;
        EEPROM.write(0, STATE_SPEAKER);
        EEPROM.commit();
      }
      if(STATE_LIGHT_OL != STATE_LIGHT){
        STATE_LIGHT_OL = STATE_LIGHT;
        EEPROM.write(1, STATE_LIGHT);
        EEPROM.commit();
      }
      if(STATE_FAN1_OL != STATE_FAN1){
        STATE_FAN1_OL = STATE_FAN1;
        EEPROM.write(2, STATE_FAN1);
        EEPROM.commit();
      }
      if(STATE_FAN2_OL != STATE_FAN2){
        STATE_FAN2_OL = STATE_FAN2;
        EEPROM.write(3, STATE_FAN2);
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
  Serial.println("Massage arived [" + String(topic) + "]" + incommingMessage);

  if (khoidong >= 4) {
    if (String(topic) == TOPIC1) {
      if (payload[0] == '1') {
        SPEAKER_ON;
        STATE_SPEAKER_OL = STATE_SPEAKER = 1;
        EEPROM.write(0, STATE_SPEAKER_OL);
        EEPROM.commit();
      }
      if (payload[0] == '0') {
        SPEAKER_OFF;
        STATE_SPEAKER_OL = STATE_SPEAKER = 0;
        EEPROM.write(0, STATE_SPEAKER_OL);
        EEPROM.commit();
      }
    }
    else if (String(topic) == TOPIC2) {
      if (payload[0] == '1') {
        LIGHT_ON;
        STATE_LIGHT_OL = STATE_LIGHT = 1;
        EEPROM.write(1, STATE_LIGHT_OL);
        EEPROM.commit();
      }
      if (payload[0] == '0') {
        LIGHT_OFF;
        STATE_LIGHT_OL = STATE_LIGHT = 0;
        EEPROM.write(1, STATE_LIGHT_OL);
        EEPROM.commit();
      }
    }
    else if (String(topic) == TOPIC3) {
      if (payload[0] == '1') {
        FAN1_ON;
        STATE_FAN1_OL = STATE_FAN1 = 1;
        EEPROM.write(2, STATE_FAN1_OL);
        EEPROM.commit();
      }
      if (payload[0] == '0') {
        FAN1_OFF;
        STATE_FAN1_OL = STATE_FAN1 = 0;
        EEPROM.write(2, STATE_FAN1_OL);
        EEPROM.commit();
      }
    }
    else if (String(topic) == TOPIC4) {
      if (payload[0] == '1') {
        FAN2_ON;
        STATE_FAN2_OL = STATE_FAN2 = 1;
        EEPROM.write(3, STATE_FAN2_OL);
        EEPROM.commit();
      }
      if (payload[0] == '0') {
        FAN2_OFF;
        STATE_FAN2_OL = STATE_FAN2 = 0;
        EEPROM.write(3, STATE_FAN2_OL);
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
  publishMessage("MEIGROUP/OTA/WS/BOXDENQUAT", jsonString_send.c_str());
  jsonDoc_send.clear();
}

void Pus_State(){
  if(STATE_SPEAKER_OL != STATE_SPEAKER){
    STATE_SPEAKER_OL = STATE_SPEAKER;
    EEPROM.write(0, STATE_SPEAKER);
    EEPROM.commit();
    if(STATE_SPEAKER_OL == 1){
      publishMessage(TOPIC1, "1"); 
    } else if(STATE_SPEAKER_OL == 0){
      publishMessage(TOPIC1, "0"); 
    }
  } 

  if(STATE_LIGHT_OL != STATE_LIGHT){
    STATE_LIGHT_OL = STATE_LIGHT;
    EEPROM.write(1, STATE_LIGHT);
    EEPROM.commit();
    if(STATE_LIGHT_OL == 1){
      publishMessage(TOPIC2, "1"); 
    } else if(STATE_LIGHT_OL == 0){
      publishMessage(TOPIC2, "0"); 
    }
  } 

  if(STATE_FAN1_OL != STATE_FAN1){
    STATE_FAN1_OL = STATE_FAN1;
    EEPROM.write(2, STATE_FAN1);
    EEPROM.commit();
    if(STATE_FAN1_OL == 1){
      publishMessage(TOPIC3, "1"); 
    } else if(STATE_FAN1_OL == 0){
      publishMessage(TOPIC3, "0"); 
    }
  }

  if(STATE_FAN2_OL != STATE_FAN2){
    STATE_FAN2_OL = STATE_FAN2;
    EEPROM.write(3, STATE_FAN2);
    EEPROM.commit();
    if(STATE_FAN2_OL == 1){
      publishMessage(TOPIC4, "1"); 
    } else if(STATE_FAN2_OL == 0){
      publishMessage(TOPIC4, "0"); 
    }
  }
}

void Update_After_LostConnection(){
  if(STATE_SPEAKER_OL == 1){
    publishMessage(TOPIC1, "1"); 
  } else if(STATE_SPEAKER_OL == 0){
    publishMessage(TOPIC1, "0"); 
  }

  if(STATE_LIGHT_OL == 1){
    publishMessage(TOPIC2, "1"); 
  } else if(STATE_LIGHT_OL == 0){
    publishMessage(TOPIC2, "0"); 
  }

  if(STATE_FAN1_OL == 1){
    publishMessage(TOPIC3, "1"); 
  } else if(STATE_FAN1_OL == 0){
    publishMessage(TOPIC3, "0"); 
  }

  if(STATE_FAN2_OL == 1){
    publishMessage(TOPIC4, "1"); 
  } else if(STATE_FAN2_OL == 0){
    publishMessage(TOPIC4, "0"); 
  }
}
