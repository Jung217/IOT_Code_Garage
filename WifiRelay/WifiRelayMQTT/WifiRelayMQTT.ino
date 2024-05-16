#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const byte led[4] = {0, 1, 3, 2};
WiFiClient wifi;
PubSubClient client(wifi);
const char* ssid = "319";
const char* passwd = "sbHappy319319";
const char* ip = "";
const char* id = "wrelay";
void setup(){
  for(byte i=0 ; i<4 ; i++){ 
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], LOW);
  }
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
  ip = WiFi.localIP();
  client.setServer(ip, 1883);
  while(!client.connect(id)){
    delay(1000);
  }
  delay(1000);
  client.subscribe("result");
  client.setCallback(callback);
  delay(1000);
}

void loop() {
  client.loop;
}

void callback(char* topic, byte* payload, unsigned int len){
  String stats = "";
  for(byte i=0 ; i<len ; i++) stats += payload[i];
  if(stats == "on"){
    digitalWrite(led[0], HIGH);
    client.publish("msg", "turn on");
  }
  else if (stats == "off"){
    digitalWrite(led[0], LOW);
    client.publish("msg", "turn off");
  }
}
