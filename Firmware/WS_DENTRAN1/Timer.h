#include <ESP8266TimerInterrupt.h> // Thư viện ngắt timer phần cứng

int cnt_blink = 0;             // Biến đếm thời gian chớp tắt Led

int cnt_ota = 0;               // Biến đếm thời gian gửi IP
uint8_t send_ip = 0;           // Biến Enable gửi IP

uint8_t cnt_ic555 = 0;         // Biến đếm thời gian cập nhật IC555

ESP8266Timer ITimer; //tạo đối tượng timer

void ICACHE_RAM_ATTR Timer() {  // Hàm ngắt Timer 50ms

  if(cnt_ic555 >= 50){// 5s sẽ kích IC555 1 lần
    digitalWrite(IC555, 1);
    cnt_ic555 = 0;
  } else {
    digitalWrite(IC555, 0);
    cnt_ic555++;
  }
  
  if(digitalRead(BUTTON) != REF_BT){
    if(digitalRead(BUTTON) == 0){
      if(STATE_RELAY == 0){
        STATE_RELAY = 1;
        digitalWrite(RELAY, STATE_RELAY);
      } else {
        STATE_RELAY = 0;
        digitalWrite(RELAY, STATE_RELAY);
      }
    }
  }
  REF_BT = digitalRead(BUTTON);

  if(connect_mqtt == 1){
    if(cnt_blink >= 20){ // 1s chớp 1 lần
      cnt_blink = 0;
      digitalWrite(LED, !digitalRead(LED));
    }
    else {
      cnt_blink++;
    }

    if(cnt_ota >= 400){ // 20s gửi IP 1 lần
      if(send_ip == 0){
        send_ip = 1;
      }
      cnt_ota = 0;
    } else {
      cnt_ota++;
    }
  } else {
    digitalWrite(LED, 1);
  }
}

void Timer_Init(){
  ITimer.attachInterruptInterval(50000, Timer);
  Serial.println("- TIMER START");
}