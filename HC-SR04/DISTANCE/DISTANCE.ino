#include <ESP32_Servo.h>
#include <LiquidCrystal_I2C.h>

#define TrigP 19
#define EchoP 18
#define dW digitalWrite

LiquidCrystal_I2C lcd(0x27,16,2); // 位址0x27，16字2列

int Lp, pos, pc;
float Large, distance, predis=0.0;  

void setup() {
  Serial.begin(9600);
  
  lcd.init(); //21,22 I^2C
  lcd.backlight();
  pinMode(TrigP,OUTPUT);
  pinMode(EchoP,INPUT);
}

void show(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(distance); 
  lcd.print("cm");
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
  Serial.println(distance);
  show();
  delay(500);
}
