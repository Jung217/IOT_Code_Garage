import time
from umqtt.simple import MQTTClient
def sub_cb(topic, msg):
    global i
    msg=msg.decode("utf-8")
    print((topic, msg,i),type(msg))
    i += 1

#c = MQTTClient("test2", "192.168.50.105",keepalive=10)
c = MQTTClient("esp32/output", "192.168.0.100", keepalive=20)
c.set_callback(sub_cb)

c.connect()
i = 0
c.subscribe("esp32/output")
while True:
    if True:
            # Blocking wait for message
        c.wait_msg()
    else:
            # Non-blocking wait for message
        c.check_msg()
            # Then need to sleep to avoid 100% CPU usage (in a real
            # app other useful actions would be performed instead)
        time.sleep(3)  