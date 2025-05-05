#define SPEAKER   27  // LOA
#define LIGHT     26  // ĐÈN BÀN
#define FAN1      25  // QUẠT TRẦN 1
#define FAN2      33  // QUẠT TRẦN 2

#define BUTTON_1  19
#define BUTTON_2  21
#define BUTTON_3  22
#define BUTTON_4  23

#define LED       2
#define IC555     13

#define SPEAKER_ON   digitalWrite(SPEAKER, 1)
#define SPEAKER_OFF  digitalWrite(SPEAKER, 0)

#define LIGHT_ON     digitalWrite(LIGHT, 1)
#define LIGHT_OFF    digitalWrite(LIGHT, 0)

#define FAN1_ON      digitalWrite(FAN1, 1)
#define FAN1_OFF     digitalWrite(FAN1, 0)

#define FAN2_ON      digitalWrite(FAN2, 1)
#define FAN2_OFF     digitalWrite(FAN2, 0)

uint8_t REF_BT1 = 1;
uint8_t REF_BT2 = 1;
uint8_t REF_BT3 = 1;
uint8_t REF_BT4 = 1;

uint8_t STATE_SPEAKER    = 0;
uint8_t STATE_SPEAKER_OL = 0;

uint8_t STATE_LIGHT    = 0;
uint8_t STATE_LIGHT_OL = 0;

uint8_t STATE_FAN1    = 0;
uint8_t STATE_FAN1_OL = 0;

uint8_t STATE_FAN2    = 0;
uint8_t STATE_FAN2_OL = 0;


void IC555_Set(){ // Tạo xung cho IC555 để thiết bị không bị reset
  digitalWrite(IC555, 1);
  delay(100);
  digitalWrite(IC555, 0);
}

void Control_Init(){

  pinMode(SPEAKER, OUTPUT);
  pinMode(LIGHT,   OUTPUT);
  pinMode(FAN1,    OUTPUT);
  pinMode(FAN2,    OUTPUT);
  pinMode(LED,     OUTPUT);
  pinMode(IC555,   OUTPUT);
  
  IC555_Set();

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);

  Serial.println("- CONTROL INIT DONE");

}

void Read_Button(){
  if(digitalRead(BUTTON_1) != REF_BT1){
    if(digitalRead(BUTTON_1) == 0){
      if(STATE_SPEAKER == 0){
        STATE_SPEAKER = 1;
        digitalWrite(SPEAKER, STATE_SPEAKER);
      } else {
        STATE_SPEAKER = 0;
        digitalWrite(SPEAKER, STATE_SPEAKER);
      }
    }
  }
  REF_BT1 = digitalRead(BUTTON_1);

  if(digitalRead(BUTTON_2) != REF_BT2){
    if(digitalRead(BUTTON_2) == 0){
      if(STATE_LIGHT == 0){
        STATE_LIGHT = 1;
        digitalWrite(LIGHT, STATE_LIGHT);
      } else {
        STATE_LIGHT = 0;
        digitalWrite(LIGHT, STATE_LIGHT);
      }
    }
  }
  REF_BT2 = digitalRead(BUTTON_2);

  if(digitalRead(BUTTON_3) != REF_BT3){
    if(digitalRead(BUTTON_3) == 0){
      if(STATE_FAN1 == 0){
        STATE_FAN1 = 1;
        digitalWrite(FAN1, STATE_FAN1);
      } else {
        STATE_FAN1 = 0;
        digitalWrite(FAN1, STATE_FAN1);
      }
    }
  }
  REF_BT3 = digitalRead(BUTTON_3);

  if(digitalRead(BUTTON_4) != REF_BT4){
    if(digitalRead(BUTTON_4) == 0){
      if(STATE_FAN2 == 0){
        STATE_FAN2 = 1;
        digitalWrite(FAN2, STATE_FAN2);
      } else {
        STATE_FAN2 = 0;
        digitalWrite(FAN2, STATE_FAN2);
      }
    }
  }
  REF_BT4 = digitalRead(BUTTON_4);
}