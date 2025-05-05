#define RELAY    16
#define LED      5
#define BUTTON   27

#define RELAY_ON   digitalWrite(RELAY, 1)
#define RELAY_OFF  digitalWrite(RELAY, 0)

uint8_t REF_BT = 1;
uint8_t STATE_RELAY = 0;
uint8_t STATE_RELAY_REF = 0;

void Control_Init(){
  pinMode(RELAY, OUTPUT);
  pinMode(LED,   OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  //RELAY_OFF;
  Serial.println("- CONTROL INIT DONE");
}