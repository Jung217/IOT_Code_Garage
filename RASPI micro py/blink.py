from machine import Pin
p2 = Pin(2, Pin.OUT)
while 1:
    p2.value(1)
    time.sleep(1)
    p2.value(0)
    time.sleep(1)
