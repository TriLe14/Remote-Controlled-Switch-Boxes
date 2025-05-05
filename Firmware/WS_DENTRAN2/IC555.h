// IC555
#define IC555 5

void IC555_Set(){ // Tạo xung cho IC55 để không bị reset
  digitalWrite(IC555, HIGH);
  delay(100);
  digitalWrite(IC555, LOW);
}

void IC555_Init(){

  pinMode(IC555, OUTPUT);
  IC555_Set();
  
  Serial.println("- IC555 INIT DONE");

}