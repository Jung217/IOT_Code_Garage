#camera.py
# import the necessary packages
from signal import default_int_handler
import cv2
import numpy as np

process_this_frame = True

class VideoCamera(object):
    def __init__(self):
       # capturing video
       self.video = cv2.VideoCapture(0)
       self.capture = False
       self.count = 0

       self.img_path = "./static/test.jpg"
       self.pred = 'None'
    
    def __del__(self):
        # releasing camera
        self.video.release()
    def get_frame(self):
        # extracting frames
        ret, frame = self.video.read()

        if self.capture:
            cv2.imwrite(self.img_path, frame)
            print("Save img")

            self.pred = 'processing...%d' % self.count
            self.count += 1
            self.capture = False

        # encode OpenCV raw frame to jpg and displaying it
        ret, jpeg = cv2.imencode('.jpg', frame)
        return jpeg.tobytes()