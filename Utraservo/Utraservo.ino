#include <ESP32_Servo.h>
#include <LiquidCrystal_I2C.h>

#define TrigP 19
#define EchoP 18
#define servoP 12
#define dW digitalWrite

LiquidCrystal_I2C lcd(0x27,16,2); // 位址0x27，16字2列
Servo servo;

int Lp, pos, pc;
float Large, distance, predis=0.0;  

void setup() {
  Serial.begin(9600);
  
  lcd.init(); //21,22 I^2C
  lcd.backlight();
  pinMode(TrigP,OUTPUT);
  pinMode(EchoP,INPUT);

  servo.attach(servoP, 500, 2400); // 設定伺服馬達的接腳
  search();    
}

void search(){
  Large=1000.0;
  Lp = -1;
  pos = 0;
  pc = 1;
  for(int i=0 ; i<36 ; i++){
    detec();
    control();
    show();
    if(distance < Large) {
      Large = distance;
      Lp = pos;
    }
    if(Large == 0) break;
    delay(500);
  }
  if(Lp != -1){
    predis = distance;
    Serial.println();  
    Serial.println(Large);
    Serial.println(Lp);  
    servo.write(Lp);    
  }
}

void show(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(distance); 
  lcd.print("cm");
  lcd.setCursor(0,1);
  lcd.print(Lp);
  lcd.print("/");
  lcd.print(pos);
}

void control() {
   if(pos >= 180) pc = 1;
   if(pos <= 0) pc = 0;
   if(pc == 1) pos -= 5;
   if(pc == 0) pos += 5;
   servo.write(pos); 
   delay(30);
}

void detec(){
  unsigned long pulseTime;
  dW(TrigP,1); // 觸發超音波進行偵測
  delayMicroseconds(10); // 延遲等待10微秒(0.00001秒)
  dW(TrigP,0); // 停止超音波偵測
  pulseTime = pulseIn(EchoP,HIGH); // 取得超音波回傳的脈波時間
  distance = pulseTime * 0.0173; // 計算距離（公分）0.034/2  
}

void loop() {
  detec();
  show();
  delay(500);
  if(distance != predis) search();

  delay(500);
}
