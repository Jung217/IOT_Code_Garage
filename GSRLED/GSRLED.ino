const int LED=2;
#define GSR 36
int sensorValue;

void setup()
{
Serial.begin(9600);
pinMode(LED,OUTPUT);
digitalWrite(LED,LOW);
delay(1000);
}

void loop()
{
int temp;
float conductivevoltage;
sensorValue=analogRead(GSR);
conductivevoltage = sensorValue*(5.0/1023.0);
Serial.print("sensorValue=");
Serial.println(sensorValue);
delay(500);
}