const byte relay[4]={0, 1, 3, 2};
void setup() {
  for(byte i=0 ; i<4 ; i++){
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], LOW);
  }
}

void loop() {
  for(byte j=0 ; j<4 ; j++){
    digitalWrite(relay[j], HIGH);
    delay(1000);
    digitalWrite(relay[j], LOW);
    delay(1000);
  }
}
