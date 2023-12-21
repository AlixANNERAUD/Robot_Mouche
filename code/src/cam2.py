import time
import os
from picamera2 import Picamera2, Preview
import matplotlib.pyplot as plt
import numpy as np

def to_line(img):
    layer = 255 - np.mean(np.mean(img[100:100+5], axis=2).T, axis=1).T
    return layer.astype(np.uint8).reshape(640)

picam2 = Picamera2()
#picam2.start_preview(Preview.QTGL)

preview_config = picam2.create_preview_configuration(raw={"size": picam2.sensor_resolution})
picam2.configure(preview_config)

fig = plt.figure()
ax = fig.add_subplot(111)

picam2.start()
#picam2.stream_configuration("main")
im = ax.bar(range(640), height=to_line(picam2.capture_array("main")))

file_path = './line_position.bin'
tmp_file_path = './line_position.bin.tmp'
while True:
    raw = picam2.capture_array("main")
    line = to_line(raw)
    i = 0
    if os.path.isfile(file_path):
        os.remove(file_path)
    with open(tmp_file_path, 'wb') as file:
        for d in line:
            file.write(d)
    os.rename(tmp_file_path, file_path)
