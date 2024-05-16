from machine import Pin, SPI
import time,urandom
from umqtt.simple import MQTTClient
def sub_cb(topic, msg):
    global i
    msg=int(msg.decode("utf-8"))
    print((topic, msg,i),type(msg))
    i += 1
    DECODEMODE = const(9)
    INTENSITY = const(10) #0xA
    SCANLIMIT = const(11) #0xB
    SHUTDOWN = const(12)  #0xC
    DISPLAYTEST = const(15) #0xF

    symbol = [[ 0x3E, 0x7F, 0x71, 0x59, 0x4D, 0x7F, 0x3E, 0x00 ], # '0'
              [ 0x40, 0x42, 0x7F, 0x7F, 0x40, 0x40, 0x00, 0x00 ], # '1'
              [ 0x62, 0x73, 0x59, 0x49, 0x6F, 0x66, 0x00, 0x00 ], # '2'
              [ 0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00 ], # '3'
              [ 0x18, 0x1C, 0x16, 0x53, 0x7F, 0x7F, 0x50, 0x00 ], # '4'
              [ 0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00, 0x00 ], # '5'
              [ 0x3C, 0x7E, 0x4B, 0x49, 0x79, 0x30, 0x00, 0x00 ], # '6'
              [ 0x03, 0x03, 0x71, 0x79, 0x0F, 0x07, 0x00, 0x00 ], # '7'
              [ 0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, 0x00 ], # '8'
              [ 0x06, 0x4F, 0x49, 0x69, 0x3F, 0x1E, 0x00, 0x00 ] # '9'
             ]

    cs = Pin(15, Pin.OUT)
    spi = SPI(1,baudrate=100000)

    def max7219(reg, data):
        cs.value(0)
        spi.write(bytes([reg, data]))
        cs.value(1)

    def init():
        for reg, data in (
            (DISPLAYTEST, 0),
            (SCANLIMIT, 7),
            (INTENSITY, 8),
            (DECODEMODE, 0),
            (SHUTDOWN, 1)
        ):
            max7219(reg, data)

    def clear():
        for i in range(8):
            max7219(i + 1, 0)
        time.sleep(1)


    def animate():
        i = msg
        print('msg=',i)
        for j in range(8):
            max7219(j+1, symbol[i][j])
        time.sleep(1)
    init()
    animate()
    clear()


    

#c = MQTTClient("test2", "192.168.50.105",keepalive=10)
c = MQTTClient("test2", "192.168.50.105")
c.set_callback(sub_cb)

c.connect()
i = 0
c.subscribe("test")
while True:
    if True:
            # Blocking wait for message
        c.wait_msg()
    else:
            # Non-blocking wait for message
        c.check_msg()
            # Then need to sleep to avoid 100% CPU usage (in a real
            # app other useful actions would be performed instead)
        #time.sleep(3)  
