#include <WiFi.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "JUNGDLINK";
const char* password = "jung0217";
const char* pd = "20030217";
String EndPoint = "https://ap-southeast-1.aws.data.mongodb-api.com/app/application-0-nbjpw/endpoint/esp32post";
const char* data_format = "%5.1f";
const unsigned long lastingInterval = 15L * 1000L;
unsigned long lastConnectTime = 0L;
WiFiClient espClient;

String formattedDate; //timestamp
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  initWiFi();

  timeClient.begin();
  timeClient.setTimeOffset(0); // 設定時區 *60分 * 60秒
}

void loop() {
  while(!timeClient.update()) { //update time
    timeClient.forceUpdate();
  }
  formattedDate = timeClient.getFormattedDate();
  //Serial.println(formattedDate);
  
  if ((millis() - lastConnectTime) > lastingInterval) {
    if(WiFi.status()== WL_CONNECTED){
      char msg1[5] ="80";
      char msg2[5] ="99";
      char msg3[5] ="589";
      char msg4[30] ="2023-11-25T14:36:18Z";
      int AA = random(95, 99);
      int BB = random(70, 90);
      int CC = random(300, 1000);
      itoa(AA, msg1, 10);  
      itoa(BB, msg2, 10);
      itoa(CC, msg3, 10); //sprintf  
      formattedDate.toCharArray(msg4, formattedDate.length()-1);
      http_update(msg1, msg2, msg3, msg4);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastConnectTime = millis();
  }
}

void http_update(char *msg1, char *msg2, char *msg3, char *msg4) {
  HTTPClient http;
  http.begin(EndPoint.c_str());
  http.addHeader("Content-Type", "application/json");
  String spo2 = "{\"spo2\": ";
  String beat = ", \"beat\": ";
  String mood = ", \"mood\": ";
  String tp = ", \"timestamp\": ";
  String pw = ", \"password\":";
  spo2.concat(msg1);
  beat.concat(msg2);
  mood.concat(msg3);
  tp.concat(msg4);
  pw.concat(pd);
  pw.concat("}");
  spo2.concat(beat.c_str());
  spo2.concat(mood.c_str());
  spo2.concat(tp.c_str());
  spo2.concat(pw.c_str());
  Serial.println(spo2);
  int httpResponseCode = http.POST(spo2);          
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
