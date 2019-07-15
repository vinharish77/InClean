import paho.mqtt.client as mqtt
import json
import base64

APPEUI = 'esp32/test'
APPID  = 'uodfjdmd'
PSW    = 'PbpIBS3TNNHZ'

import matplotlib.pyplot as plt
#import DataPlot and RealtimePlot from the file plot_data.py
from plot_data import DataPlot, RealtimePlot

fig, axes = plt.subplots()
plt.title('Air Quality Data')

data = DataPlot()
dataPlotting= RealtimePlot(axes)

count=0

def on_connect(client, userdata, flags, rc):
    client.subscribe('esp32/test')

def on_message(client, userdata, msg):
    reading = float(msg.payload)

    # print data
    print('---')
    print('ug/m^3: ' + str(reading))

    # plot data
    global count
    count+=1
    why = 0
    data.add(count, reading, why)
    dataPlotting.plot(data)
    plt.pause(0.001)

# set paho.mqtt callback
ttn_client = mqtt.Client()
ttn_client.on_connect = on_connect
ttn_client.on_message = on_message
ttn_client.username_pw_set(APPID, PSW)
ttn_client.connect("postman.cloudmqtt.com", 11711, 60) #MQTT port over TLS

try:
    ttn_client.loop_forever()
except KeyboardInterrupt:
    print('disconnect')
    ttn_client.disconnect()
