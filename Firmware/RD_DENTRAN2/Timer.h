hw_timer_t* timer = NULL;  //
int cnt_blink = 0;
int cnt_ota = 0;
uint8_t send_ip = 0;
uint8_t cnt_ic555 = 0;

void IRAM_ATTR Timer() {  // Hàm ngắt Timer 50ms

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
    if(cnt_blink == 20){
      cnt_blink = 0;
      digitalWrite(LED, !digitalRead(LED));
    }
    else {
      cnt_blink++;
    }

    if(cnt_ota == 400){
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
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &Timer, true);
  timerAlarmWrite(timer, 50000, true);  //ngắt 50ms
  timerAlarmEnable(timer);
  Serial.println("- TIMER START");
}