#include <BluetoothSerial.h>
BluetoothSerial BT;

void setup() {
  Serial.begin(115200);
  BT.begin("Eric1030");//請改名
}
void loop() {
  //檢查藍芽內是否有資料
  while (BT.available()) {
    //讀取藍芽資料
    String BTdata=BT.readString();
    //顯示在序列視窗
    Serial.println(BTdata);
  }
  delay(1);
}