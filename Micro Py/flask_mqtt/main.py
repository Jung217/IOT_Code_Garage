# main.py
# import the necessary packages
from xml.etree.ElementPath import prepare_descendant
from flask import Flask, render_template, Response
from camera import VideoCamera

import paho.mqtt.client as mqtt
import time

weed_classes = {'broken': 0, 'ripe': 1, 'stage1': 2, 'stage2': 3, 'stage3': 4, 'stage4': 5, 'stage5': 6, 'stage6': 7, 'None': 9}

mqttClient = mqtt.Client()
mqttClient.connect("192.168.50.248",1883)
mqttClient.loop_start()

vc = VideoCamera()

app = Flask(__name__)

@app.route('/')
def index():
    # rendering webpage
    return render_template('index.html', pred_class = vc.pred)

@app.route('/', methods=['POST'])
def capture_img():
    vc.capture = True
    time.sleep(1)

    pred_data = str(vc.pred)
    pred_num = weed_classes[vc.pred]
    mqttClient.publish('test', pred_num)
    
    return render_template('index.html', pred_class = pred_data)

def gen(camera):
    while True:
        #get camera frame
        frame = camera.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/video_feed')
def video_feed():
    return Response(gen(vc),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    # defining server ip address and port
    app.run(host='0.0.0.0',port='5000', debug=True)