#include "Control.h"
#include "Mqtt.h"
#include "Timer.h"

void setup() {
  Serial.begin(9600);
  Control_Init();
  Timer_Init();

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  STATE_RELAY_REF = STATE_RELAY = EEPROM.read(0);
  digitalWrite(RELAY, STATE_RELAY);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  OTA_Init();

  esp_task_wdt_init(50, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch

}

void loop() {
  esp_task_wdt_reset(); 

  server.handleClient();
  delay(1);

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
  }

  if(STATE_RELAY_REF != STATE_RELAY){
    STATE_RELAY_REF = STATE_RELAY;
    EEPROM.write(0, STATE_RELAY);
    EEPROM.commit();
    Serial.println(EEPROM.read(0));
    if(STATE_RELAY_REF == 1){
      publishMessage(TOPIC, "1"); 
    } else if(STATE_RELAY_REF == 0){
      publishMessage(TOPIC, "0"); 
    }
  }
}