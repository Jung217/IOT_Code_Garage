uint8_t spo2 = 99;
uint8_t beat = 100;
uint32_t mood = 1000;

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "JUNGDLINK";
const char* password = "jung0217";
const char* pd = "20030217";

const unsigned long lastingInterval = 1L * 1000L;
unsigned long lastConnectTime = 0L;

WiFiClient espClient;
String EndPoint = "https://ap-southeast-1.aws.data.mongodb-api.com/app/application-0-nbjpw/endpoint/esp32post";

/*void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}*/

void setup() {
  Serial.begin(115200);
  /*WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  initWiFi();*/
}

void loop() {
  if ((millis() - lastConnectTime) > lastingInterval) {
    //if(WiFi.status()== WL_CONNECTED){
      char msg1[5] ="80";
      char msg2[5] ="99";
      char msg3[5] ="589";
      int AA = spo2;
      int BB = beat;
      int CC = mood;
      itoa(AA, msg1, 10);  
      itoa(BB, msg2, 10);
      itoa(CC, msg3, 10); //sprintf    
      //http_update(msg1, msg2, msg3);
    /*}
    else {
      Serial.println("WiFi Disconnected");
    }*/
    Serial.println(msg1);
    Serial.println(msg2);
    Serial.println(msg3);
    lastConnectTime = millis();
  }
}

void http_update(char *msg1, char *msg2, char *msg3) {
  HTTPClient http;
  http.begin(EndPoint.c_str());
  http.addHeader("Content-Type", "application/json");
  String spo2 = "{\"spo2\": ";
  String beat = ", \"beat\": ";
  String mood = ", \"mood\": ";
  String pw = ", \"password\":";
  spo2.concat(msg1);
  beat.concat(msg2);
  mood.concat(msg3);
  pw.concat(pd);
  pw.concat("}");
  spo2.concat(beat.c_str());
  spo2.concat(mood.c_str());
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