#include "Control.h"
#include "Mqtt.h"
#include "Timer.h"

unsigned long currentMillis = millis();

void setup() {
  Serial.begin(9600);
  Control_Init();
  IC555_Init();
  Timer_Init();

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  STATE_RELAY_REF = STATE_RELAY = EEPROM.read(0);
  digitalWrite(RELAY, STATE_RELAY);

  STATE_LED2_REF = STATE_LED2 = EEPROM.read(2);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  Setup_OTA();

}

void loop() {

  iot47_wifi_ota_loop();

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost");
    connect_mqtt = 0;
    setup_wifi(); // Gọi hàm để kết nối lại với WiFi
  }

  if (!client.connected()) {  
    reconnect();
  } else {
    client.loop();
  }

  if(send_ip == 1){
    Pus_Info();
    send_ip = 0;
  }

  if(connect_mqtt == 0){
    connect_mqtt = 1;
    khoidong = 0;

    STATE_RELAY_REF = STATE_RELAY = EEPROM.read(0);
    digitalWrite(RELAY, STATE_RELAY);
    if(STATE_RELAY_REF == 1){
      publishMessage(TOPIC, "1"); 
    } else if(STATE_RELAY_REF == 0){
      publishMessage(TOPIC, "0"); 
    }

    STATE_LED2_REF = STATE_LED2 = EEPROM.read(2);
    if(STATE_LED2_REF == 1){
      publishMessage(TOPIC2, "1"); 
    } else if(STATE_LED2_REF == 0){
      publishMessage(TOPIC2, "0"); 
    }
  }

  if(STATE_RELAY_REF != STATE_RELAY){
    STATE_RELAY_REF = STATE_RELAY;
    EEPROM.write(0, STATE_RELAY);
    EEPROM.commit();
    if(STATE_RELAY_REF == 1){
      publishMessage(TOPIC, "1"); 
    } else if(STATE_RELAY_REF == 0){
      publishMessage(TOPIC, "0"); 
    }
  }

  if(STATE_LED2_REF != STATE_LED2){
    STATE_LED2_REF = STATE_LED2;
    EEPROM.write(2, STATE_LED2);
    EEPROM.commit();
    if(STATE_LED2_REF == 1){
      publishMessage(TOPIC2, "1"); 
    } else if(STATE_LED2_REF == 0){
      publishMessage(TOPIC2, "0"); 
    }
  }

  // if(millis() - currentMillis >= 2000) { 
  //   currentMillis = millis();
  //   IC555_Set();
  // }
}
