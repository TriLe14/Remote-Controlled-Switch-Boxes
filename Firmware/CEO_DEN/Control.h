#define RELAY     4
#define BUTTON2   12
#define BUTTON1   13

#define RELAY_ON   digitalWrite(RELAY, 1)
#define RELAY_OFF  digitalWrite(RELAY, 0)

uint8_t REF_BT1 = 1;
uint8_t REF_BT2 = 1;

uint8_t STATE_RELAY = 0;
uint8_t STATE_RELAY_REF = 0;

uint8_t STATE_LED2 = 0;
uint8_t STATE_LED2_REF = 0;

void Control_Init(){
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  Serial.println("- CONTROL INIT DONE");
}