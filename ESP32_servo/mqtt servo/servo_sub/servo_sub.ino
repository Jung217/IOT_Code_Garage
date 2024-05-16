#include <ESP32_Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define OUT_X 14       // 伺服馬達X的輸出腳
#define OUT_Y 12       // 伺服馬達Y的輸出腳
#define PORT 1883
/*const char* ssid = "信我者得Wi-Fi";
const char* password = "0909160217";
const char* mqtt_server = "192.168.238.234";*/
const char* ssid = "319";
const char* password = "sbHappy319319";
const char* mqtt_server = "192.168.50.170";
int cposX, cposY;

WiFiClient espClient;
PubSubClient client(espClient);
Servo servoX, servoY;   // 宣告兩個伺服馬達程式物件

void setup() {
  Serial.begin(115200);
  servoX.attach(OUT_X, 500, 2400);    // 設定伺服馬達的接腳
  servoY.attach(OUT_Y, 500, 2400);
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
  
  /*Serial.println(mqttPort);
  broker.setMaxNumClients(9);
  broker.startBroker();
  Serial.print("broker started");
  Serial.println("Use this port: ");
  delay(1000);
  mqtt_server =  WiFi.localIP().toString().c_str();
  //可以在ap上設固定ip取代*/
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;

  for (int i = 0; i < length ; i++) {
    messageTemp += (char)message[i];
  }

  if (String(topic) == "esp32/output/x") {
    cposX = messageTemp.toInt();
  }
  else if(String(topic) == "esp32/output/y"){
    cposY = messageTemp.toInt();
  }
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

void control() {
   servoX.write(cposX);    
   Serial.print(cposX);
   Serial.print("\n");
   servoY.write(cposY);
   Serial.print(cposY);
// 設定伺服馬達的旋轉角度
   delay(15);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  control(); 
}