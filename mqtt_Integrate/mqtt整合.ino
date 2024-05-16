#include <WiFi.h>
#include <PubSubClient.h>
#include "EmbeddedMqttBroker.h"//version:1.0.3

#define IN_X 34        // 可變電阻X（水平搖桿）的輸入腳
#define IN_Y 35        // 可變電阻Y（垂直搖桿）的輸入腳
#define size 5         // 資料陣列元素數量
#define middle size/2  // 資料陣列中間索引
#define PORT 1883
/*const char* ssid = "JUNGDLINK";
const char* password = "jung0217";*/
const char* ssid = "319";
const char* password = "sbHappy319319";
const char* mqtt_server = "";
int counter = 0;
uint16_t mqttPort = 1883;

MqttBroker broker(mqttPort);
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

int valX[size] = {0, 0, 0, 0, 0};  // X軸資料陣列
int valY[size] = {0, 0, 0, 0, 0};  // Y軸資料陣列
int posX, posY;// 暫存類比輸入值的變數
int cmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void setup() {
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);  // 設定類比輸入電壓上限3.6V
  analogSetWidth(10);
  WiFi.mode(WIFI_STA);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(100);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println(mqttPort);
  broker.setMaxNumClients(9);
  broker.startBroker();
  Serial.print("broker started");
  Serial.println("Use this port: ");
  delay(1000);
  mqtt_server =  WiFi.localIP().toString().c_str();
  ////可以在ap上設固定ip取代*/
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length ; i++) {
    messageTemp += (char)message[i];
  }
  //Serial.print(topic);
  Serial.println(messageTemp);
}

void reconnect() {
  while (WiFi.status() != WL_CONNECTED)
  {
    setup_wifi();
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);   //將回傳資料傳入callback

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      client.subscribe("esp32/output/x");//Change Topic
      client.subscribe("esp32/output/y");
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}

void filter() {
  static byte i = 0;  // 陣列索引
         
  valX[i] = analogRead(IN_X);  // 水平（X）搖桿的輸入值
  valY[i] = analogRead(IN_Y);  // 垂直（Y）搖桿的輸入值  

  qsort(valX, size, sizeof(int), cmp);
  qsort(valY, size, sizeof(int), cmp);

  if (++i % size == 0) i = 0;

  posX = posY = 0;
  
  posX = map(valX[middle], 0, 1023, 0, 180);
  posY = map(valY[middle], 0, 1023, 0, 180);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  long now = millis();
  if (now - lastMsg > 500) {
    lastMsg = now;

    filter();

    char PUBx[4] = {0,0,0,0};
    char PUBy[4] = {0,0,0,0};

    itoa(posX, PUBx, 10);
    itoa(posY, PUBy, 10);
    Serial.print(posX);
    Serial.print("\n");
    Serial.print(posY);
    Serial.print("\n");
    Serial.print(PUBx);
    Serial.print("\n");
    Serial.print(PUBy);
    Serial.print("\n");
    //itoa值跑掉

    client.publish("esp32/output/x", PUBx);
    client.publish("esp32/output/y", PUBy);
  }
  delay(500);
}