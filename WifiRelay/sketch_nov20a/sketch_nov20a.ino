const byte relay = 1;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(relay0, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(relay0, HIGH);  // turn the LED on (HIGH is the voltage level)
  Serial.println("HIGL0");
  delay(1000);                      // wait for a second
  digitalWrite(relay0, LOW);   // turn the LED off by making the voltage LOW
  Serial.println("LOW0");
  delay(1000);     
  digitalWrite(relay1, HIGH);  // turn the LED on (HIGH is the voltage level)
  Serial.println("HIGL1");
  delay(1000);                      // wait for a second
  digitalWrite(relay1, LOW);   // turn the LED off by making the voltage LOW
  Serial.println("LOW1");
  delay(1000); 
  digitalWrite(relay2, HIGH);  // turn the LED on (HIGH is the voltage level)
  Serial.println("HIGL2");
  delay(1000);                      // wait for a second
  digitalWrite(relay2, LOW);   // turn the LED off by making the voltage LOW
  Serial.println("LOW2");
  delay(1000);     
  digitalWrite(relay3, HIGH);  // turn the LED on (HIGH is the voltage level)
  Serial.println("HIGL3");
  delay(1000);                      // wait for a second
  digitalWrite(relay3, LOW);   // turn the LED off by making the voltage LOW
  Serial.println("LOW3");
  delay(1000);                          // wait for a second
}