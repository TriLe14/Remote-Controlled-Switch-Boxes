#include "Control.h"
#include "Mqtt.h"
#include "Timer.h"

void setup() {
  Serial.begin(9600);
  Control_Init();
  Timer_Init();

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);

  STATE_LIGHT_OL = STATE_LIGHT = EEPROM.read(1);
  digitalWrite(LIGHT, STATE_LIGHT);

  STATE_FAN1_OL = STATE_FAN1 = EEPROM.read(2);
  digitalWrite(FAN1, STATE_FAN1);

  STATE_FAN2_OL = STATE_FAN2 = EEPROM.read(3);
  digitalWrite(FAN2, STATE_FAN2);

  STATE_SPEAKER_OL = STATE_SPEAKER = EEPROM.read(0);
  digitalWrite(SPEAKER, STATE_SPEAKER);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  OTA_Init();
}

void loop() {

  server.handleClient();
  delay(1);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost");
    connect_mqtt = 0;
    setup_wifi(); // Gọi hàm để kết nối lại với WiFi
  }

  if (!client.connected()) {
    connect_mqtt = 0;   
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
    Update_After_LostConnection();
  }
  
  Pus_State(); // Cập nhật khi có sự thay đổi trạng thái cho MQTT

}
