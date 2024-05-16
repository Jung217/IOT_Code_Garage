import paho.mqtt.client as mqtt
import time,random
i = 0
mqttClient = mqtt.Client()
mqttClient.connect("192.168.56.1",1883)
mqttClient.loop_start()
while True:
    # data = "success"
    # data += str(i)
    data = str(random.randint(0,9))
    mqttClient.publish('test', data)
    print(data,i)
    i = i + 1
    time.sleep(3)



