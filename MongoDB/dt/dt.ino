#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#define DHTPIN 17    
#define DHTTYPE DHT11  

const char* ssid = "JUNGDLINK";
const char* password = "jung0217";
String EndPoint = "https://ap-southeast-1.aws.data.mongodb-api.com/app/application-0-nbjpw/endpoint/esp32post";
int readout_len = 5;
const char* data_format = "%5.1f";
const unsigned long lastingInterval = 15L * 1000L;
unsigned long lastConnectTime = 0L;
DHT dht(DHTPIN, DHTTYPE);
void dht_measure(char *, char *);
void http_update(char *, char *);
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
  dht.begin();
}
void loop() {
  if ((millis() - lastConnectTime) > lastingInterval) {
    if(WiFi.status()== WL_CONNECTED){
      char msg1[readout_len]="";
      char msg2[readout_len]="";  
      dht_measure(msg1, msg2);    
      http_update(msg1, msg2);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastConnectTime = millis();
  }
}
void dht_measure(char *msg1, char *msg2) {
  float data_humi = dht.readHumidity();
  float data_temp = dht.readTemperature();
  /*if (isnan(data_humi) || isnan(data_temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }*/
  sprintf(msg1, data_format, data_temp);
  sprintf(msg2, data_format, data_humi);
  return;
}
void http_update(char *msg1, char *msg2) {
  HTTPClient http;
  http.begin(EndPoint.c_str());
  http.addHeader("Content-Type", "application/json");
  String temp = "{\"temp\":";
  String humi = ", \"humi\":";
  String pw = ", \"password\":";
  temp.concat(msg1);
  humi.concat(msg2);
  pw.concat(password);
  pw.concat("}");
  temp.concat(humi.c_str());
  temp.concat(pw.c_str());
  Serial.println(temp);
  int httpResponseCode = http.POST(temp);          
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
