hw_timer_t* timer = NULL;  //
int cnt_blink = 0;
int cnt_ota = 0;
uint8_t send_ip = 0;
uint8_t cnt_ic555 = 0;

void IRAM_ATTR Timer() {  // Hàm ngắt Timer 50ms

  cnt_ic555++;
  if(cnt_ic555 >= 100){// 5s sẽ kích IC555 1 lần
    digitalWrite(IC555, !digitalRead(IC555));
    if(digitalRead(IC555) == 0){
      cnt_ic555 = 0;
    }
  }

  Read_Button();

  if(connect_mqtt == 1){
    if(cnt_blink == 20){ // Chớp tắt sau mỗi 1s nếu kết nối MQTT thành công
      cnt_blink = 0;
      digitalWrite(LED, !digitalRead(LED));
    }
    else {
      cnt_blink++;
    }

    if(cnt_ota == 400){ // 20s gửi IP lên 1 lần
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

