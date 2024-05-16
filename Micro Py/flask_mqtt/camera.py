# keras version==2.2.5
# import tensorflow.compat.v1 as tf
# tf.disable_v2_behavior()

#camera.py
# import the necessary packages
from signal import default_int_handler
import cv2
import numpy as np

from tensorflow.keras.models import load_model
from PIL import Image
import imageio

process_this_frame = True

class VideoCamera(object):
    def __init__(self):
       # capturing video
       self.video = cv2.VideoCapture(0)
       self.capture = False

       # set model parameters
       self.weed_classes = ['broken', 'ripe', 'stage1', 'stage2', 'stage3', 'stage4', 'stage5', 'stage6']
       self.img_path = "static/test.jpg"
       self.img_shape = (128, 128)
       self.model = load_model('model/model.h5')
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

            image = imageio.imread(self.img_path)
            image = np.array(Image.fromarray(image).resize(self.img_shape))
            img = image.reshape(1, 128, 128, 3)
            prediction_possibility = self.model.predict(img)
            prediction_possibility = np.argmax(prediction_possibility, axis=-1)
            pred_class = self.weed_classes[prediction_possibility[0]]
            print(f'Predicted class: {pred_class}')
            self.pred = pred_class
            self.capture = False

        # encode OpenCV raw frame to jpg and displaying it
        ret, jpeg = cv2.imencode('.jpg', frame)
        return jpeg.tobytes()