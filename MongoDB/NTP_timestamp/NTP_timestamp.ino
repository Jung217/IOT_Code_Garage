#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFi.h>
// 儲存日期及時間的變數
String formattedDate;
String dayStamp;
String timeStamp;

// 設定無線基地台SSID跟密碼
const char* ssid     = "JUNGDLINK";
const char* password = "jung0217";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  timeClient.begin();

  // 設定時區 *60分 * 60秒，例如:
  // GMT +1 = 3600
  // GMT +8 = 28800  台灣時區
  // GMT 0 = 0
  timeClient.setTimeOffset(0);
}

void loop() {

  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // formattedDate函式取得的日期格式為： 2018-05-28T16:00:13Z
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);

  /*int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);

  // 取得時間
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  //Serial.print("TIME:");
  Serial.println(timeStamp);*/
 
  delay(1000);
}