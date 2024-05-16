#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "JUNGDLINK";
const char* password = "jung0217";
const char* pd = "20030217";
String EndPoint = "https://ap-southeast-1.aws.data.mongodb-api.com/app/application-0-nbjpw/endpoint/esp32post"; //const char*
const char* data_format = "%5.1f";
const unsigned long lastingInterval = 15L * 1000L;
unsigned long lastConnectTime = 0L;
WiFiClient espClient;

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
}

void loop() {
  if ((millis() - lastConnectTime) > lastingInterval) {
    if(WiFi.status()== WL_CONNECTED){
      char msg1[5] ="80";
      char msg2[5] ="99";
      char msg3[5] ="589";
      int AA = random(95, 99);
      int BB = random(100, 130);
      int CC = random(1800, 3600);
      itoa(AA, msg1, 10);  
      itoa(BB, msg2, 10);
      itoa(CC, msg3, 10); //sprintf    
      http_update(msg1, msg2, msg3);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastConnectTime = millis();
  }
}

void http_update(char *msg1, char *msg2, char *msg3) {
  HTTPClient http;
  http.begin(EndPoint.c_str());//EndPoint
  http.addHeader("Content-Type", "application/json");
  String spo2 = "{\"spo2\": ";
  String beat = ", \"beat\": ";
  String mood = ", \"time\": ";
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
