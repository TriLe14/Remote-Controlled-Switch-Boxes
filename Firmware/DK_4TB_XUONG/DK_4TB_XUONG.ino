#include "WiFi.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "ota.h"
char ssid[] = "MEIGROUP_DC2.4";
char pass[] = "meigroupiot88";

#include <PubSubClient.h>
#define mqtt_server "meigroup2025.ddns.net"
#define mqtt_user "meigroup"
#define mqtt_pass "@778899"
const uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

int RELAY_1 = 27;
int TT_RELAY_1 = 0, TT_RELAY_1_OL = -1;
int RELAY_2 = 26;
int TT_RELAY_2 = 0, TT_RELAY_2_OL = -1;
int RELAY_3 = 25;
int TT_RELAY_3 = 0, TT_RELAY_3_OL = -1;
int RELAY_4 = 33;
int TT_RELAY_4 = 0, TT_RELAY_4_OL = -1;

int BUTTON_1 = 19;
int BUTTON_2 = 21;
int BUTTON_3 = 22;
int BUTTON_4 = 23;

int IC555 = 13;

int LED = 2;
int TT_LED = 0;

int kd = 0;

#include <ArduinoJson.h>
DynamicJsonDocument jsonDoc_send(1024);
void Pus_Info() {
  jsonDoc_send["IP"] = WiFi.localIP().toString();
  jsonDoc_send["RSSI"] = WiFi.RSSI();
  jsonDoc_send["MAC"] = WiFi.macAddress();
  jsonDoc_send["SSID"] = WiFi.SSID();
  String jsonString_send;
  serializeJson(jsonDoc_send, jsonString_send);
  client.publish("BOX_DK_DEN_QUAT/Info", jsonString_send.c_str());
  Serial.print("publish Topic: ");
  Serial.println(jsonString_send);
  jsonDoc_send.clear();
}

int Mqtt_connetion;
// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("TOPIC: ");
  String TOPIC = String(topic);
  Serial.print(TOPIC);

  if (kd == 1) {
    Serial.print("   PAYLOAD: ");
    String PAYLOAD;
    for (uint8_t i = 0; i < length; i++) {
      PAYLOAD += char(payload[i]);
    }
    Serial.println(PAYLOAD);

    if (TOPIC.equals("BOX_DK_DEN_QUAT/TB1")) {
      TT_RELAY_1 = PAYLOAD.toInt();
      TT_RELAY_1_OL = TT_RELAY_1;
      digitalWrite(RELAY_1, TT_RELAY_1);
    }

    if (TOPIC.equals("BOX_DK_DEN_QUAT/TB2")) {
      TT_RELAY_2 = PAYLOAD.toInt();
      TT_RELAY_2_OL = TT_RELAY_2;
      digitalWrite(RELAY_2, TT_RELAY_2);
    }

    if (TOPIC.equals("BOX_DK_DEN_QUAT/TB3")) {
      TT_RELAY_3 = PAYLOAD.toInt();
      TT_RELAY_3_OL = TT_RELAY_3;
      digitalWrite(RELAY_3, TT_RELAY_3);
    }

    if (TOPIC.equals("BOX_DK_DEN_QUAT/TB4")) {
      TT_RELAY_4 = PAYLOAD.toInt();
      TT_RELAY_4_OL = TT_RELAY_4;
      digitalWrite(RELAY_4, TT_RELAY_4);
    }
  }
}

// Hàm reconnect
void reconnect() {
  if (!client.connected()) {
    String clientId = "ESP32Client_Den_4TB_xuong-";
    clientId += String(random(0xffff), HEX);
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Đã kết nối MQTT:");

      client.subscribe("BOX_DK_DEN_QUAT/TB1");
      client.subscribe("BOX_DK_DEN_QUAT/TB2");
      client.subscribe("BOX_DK_DEN_QUAT/TB3");
      client.subscribe("BOX_DK_DEN_QUAT/TB4");

      Mqtt_connetion = 1;
    } else {
      Serial.print("Lỗi:, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      Mqtt_connetion = 0;
    }
  } else {
    Mqtt_connetion = 1;
  }
}


int TT_BNT_1 = 1, TT_BNT_1_OL = 1;
int TT_BNT_2 = 1, TT_BNT_2_OL = 1;
int TT_BNT_3 = 1, TT_BNT_3_OL = 1;
int TT_BNT_4 = 1, TT_BNT_4_OL = 1;
hw_timer_t* timer = NULL;
void IRAM_ATTR onTimer() {
  TT_BNT_1 = digitalRead(BUTTON_1);
  if (TT_BNT_1 != TT_BNT_1_OL) {
    TT_BNT_1_OL = TT_BNT_1;
    if (TT_BNT_1 == 1) {  // thả ra
      TT_RELAY_1 = !TT_RELAY_1;
      digitalWrite(RELAY_1, TT_RELAY_1);
    }
  }

  TT_BNT_2 = digitalRead(BUTTON_2);
  if (TT_BNT_2 != TT_BNT_2_OL) {
    TT_BNT_2_OL = TT_BNT_2;
    if (TT_BNT_2 == 1) {  // thả ra
      TT_RELAY_2 = !TT_RELAY_2;
      digitalWrite(RELAY_2, TT_RELAY_2);
    }
  }

  TT_BNT_3 = digitalRead(BUTTON_3);
  if (TT_BNT_3 != TT_BNT_3_OL) {
    TT_BNT_3_OL = TT_BNT_3;
    if (TT_BNT_3 == 1) {  // thả ra
      TT_RELAY_3 = !TT_RELAY_3;
      digitalWrite(RELAY_3, TT_RELAY_3);
    }
  }

  TT_BNT_4 = digitalRead(BUTTON_4);
  if (TT_BNT_4 != TT_BNT_4_OL) {
    TT_BNT_4_OL = TT_BNT_4;
    if (TT_BNT_4 == 1) {  // thả ra
      TT_RELAY_4 = !TT_RELAY_4;
      digitalWrite(RELAY_4, TT_RELAY_4);
    }
  }

  static byte dem_IC555 = 0;
  dem_IC555++;
  if (dem_IC555 > 10) {  // tưng ướng 1s
    dem_IC555 = 0;
    digitalWrite(IC555, !digitalRead(IC555));  // tạo xung IC555
  }

  static byte dem_led = 0;
  dem_led++;
  if (TT_LED == 1) {
    if (dem_led >= 10) {
      dem_led = 0;
      digitalWrite(LED, !digitalRead(LED));  // nhấy led 1s/lần
    }
  } else if (TT_LED == 0) {
    if (dem_led >= 2) {
      dem_led = 0;
      digitalWrite(LED, !digitalRead(LED));  // nhấy led 200ms/lần
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  pinMode(LED, OUTPUT);
  pinMode(IC555, OUTPUT);

  // Cấu hình timer phần cứng
  timer = timerBegin(0, 80, true);              // Timer 0, tần số 80MHz/80 = 1MHz (1us)
  timerAttachInterrupt(timer, &onTimer, true);  // Gắn hàm ngắt
  timerAlarmWrite(timer, 100000, true);         // 100ms = 100,000us
  timerAlarmEnable(timer);                      // Bật timer

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Địa chỉ IP: ");
  Serial.println(WiFi.localIP().toString());
  Serial.print("Địa chỉ MAC của thiết bị: ");
  Serial.println(WiFi.macAddress());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
  delay(1000);

  setOTA();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    server.handleClient();
    delay(1);

    // Kiểm tra kết nối MQTT
    if (!client.connected()) {
      static uint32_t TIME_CHECK_MQTT = millis();
      if (millis() - TIME_CHECK_MQTT >= 1000) {
        TIME_CHECK_MQTT = millis();
        reconnect();
      }
    } else {
      Mqtt_connetion = 1;
    }

    if (Mqtt_connetion) {
      TT_LED = 1;
      client.loop();

      if (TT_RELAY_1 != TT_RELAY_1_OL) {
        TT_RELAY_1_OL = TT_RELAY_1;
        if (TT_RELAY_1) {
          client.publish("BOX_DK_DEN_QUAT/TB1", "1");
        } else {
          client.publish("BOX_DK_DEN_QUAT/TB1", "0");
        }
      }

      if (TT_RELAY_2 != TT_RELAY_2_OL) {
        TT_RELAY_2_OL = TT_RELAY_2;
        if (TT_RELAY_2) {
          client.publish("BOX_DK_DEN_QUAT/TB2", "1");
        } else {
          client.publish("BOX_DK_DEN_QUAT/TB2", "0");
        }
      }

      if (TT_RELAY_3 != TT_RELAY_3_OL) {
        TT_RELAY_3_OL = TT_RELAY_3;
        if (TT_RELAY_3) {
          client.publish("BOX_DK_DEN_QUAT/TB3", "1");
        } else {
          client.publish("BOX_DK_DEN_QUAT/TB3", "0");
        }
      }

      if (TT_RELAY_4 != TT_RELAY_4_OL) {
        TT_RELAY_4_OL = TT_RELAY_4;
        if (TT_RELAY_4) {
          client.publish("BOX_DK_DEN_QUAT/TB4", "1");
        } else {
          client.publish("BOX_DK_DEN_QUAT/TB4", "0");
        }
      }

      static uint32_t Time_Run_Set = millis() - 10000;
      if (millis() - Time_Run_Set > 10000) {
        Time_Run_Set = millis();

        uint32_t sogiaychay = millis() / 1000;
        if (sogiaychay > 864000 && digitalRead(RELAY_1) == 0 && digitalRead(RELAY_2) == 0 && digitalRead(RELAY_3) == 0 && digitalRead(RELAY_4) == 0) {
          ESP.restart();  // chay được 10 ngày reset esp lần
        }

        String random = String(sogiaychay);
        client.publish("BOX_DK_DEN_QUAT/RANDOM", random.c_str());

        static int dem1 = 2;
        dem1++;
        if (dem1 > 3) {
          dem1 = 0;

          if (digitalRead(RELAY_1)) client.publish("BOX_DK_DEN_QUAT/TB1", "1");
          else client.publish("BOX_DK_DEN_QUAT/TB1", "0");

          if (digitalRead(RELAY_2)) client.publish("BOX_DK_DEN_QUAT/TB2", "1");
          else client.publish("BOX_DK_DEN_QUAT/TB2", "0");

          if (digitalRead(RELAY_3)) client.publish("BOX_DK_DEN_QUAT/TB3", "1");
          else client.publish("BOX_DK_DEN_QUAT/TB3", "0");

          if (digitalRead(RELAY_4)) client.publish("BOX_DK_DEN_QUAT/TB4", "1");
          else client.publish("BOX_DK_DEN_QUAT/TB4", "0");


          static int dem2 = 2;
          dem2++;
          if (dem2 > 2) {
            dem2 = 0;
            Pus_Info();
          }
        }
      }
    } else {
      TT_LED = 0;
    }
  } else {
    TT_LED = 0;
    static uint32_t TIME_RUN = millis();
    if (millis() - TIME_RUN >= 1000) {
      TIME_RUN = millis();

      static int dem_begin_wifi = 0;
      dem_begin_wifi++;
      if (dem_begin_wifi >= 30) {
        dem_begin_wifi = 0;
        WiFi.begin(ssid, pass);
        Serial.print("BEGIN LAI WIFI!");
      }
    }
  }
  kd = 1;
}
