from flask import Flask,render_template, request, redirect
import requests
from math import floor
from time import sleep

app = Flask(__name__)

back_track = []
#constants
#STARTING POINT BOTTON RIGHT CORNER OF FIELD
M_PER_S = 0.1
S_PER_CMD = 0.5
IMG_X_SIZE = 600    
IMG_Y_SIZE= 400
FIELD_X_SIZE= 1.8288
FIELD_Y_SIZE=1.2192
ESP_IP = "http://192.168.137.3"


@app.route('/')
def home():
    return render_template("home.html")

@app.route('/return_home')
def return_home():
    n= len(back_track)
    for i in range(n):
        print(back_track)
        cmd = list(back_track.pop())[0]
        requests.post(ESP_IP,data= cmd)
        sleep(0.5)


@app.route('/new_coord', methods=['GET','POST'])
def move_to_position():
    query_string = request.query_string.decode('utf-8')
    x_pixel,y_pixel = [int(x) for x in query_string.split(',') ]
    print(x_pixel,y_pixel)
    x_time = FIELD_X_SIZE*(x_pixel/IMG_X_SIZE)/M_PER_S
    y_time = FIELD_Y_SIZE*(y_pixel/IMG_Y_SIZE)/M_PER_S
    print (x_time,y_time)
    requests.post(ESP_IP,data = f'coord:{x_time},{y_time}')

    x_steps = floor(x_time/S_PER_CMD)
    y_steps = floor(y_time/S_PER_CMD)
    for i in range(x_steps):
        requests.post(ESP_IP,data= "forward")
        sleep(0.5)
        back_track.append({"back":S_PER_CMD})
    request.post(ESP_IP,data="left_t")
    sleep(0.5)
    back_track.append({"right_t":S_PER_CMD})    
    for i in range(y_steps):
        request.post(ESP_IP,data= "forward")
        sleep(0.5)
        back_track.append({"back":S_PER_CMD})

    return redirect("localhost:5000/")

@app.route('/forward')
def forward():
    requests.post(ESP_IP,data= "forward")
    back_track.append({"back":S_PER_CMD})
    
@app.route('/back')
def back():
    requests.post(ESP_IP,data= "back")
    back_track.append({"forward":S_PER_CMD})

@app.route('/left_t')
def left_t():
    requests.post(ESP_IP,data= "left_t")
    back_track.append({"right_t":S_PER_CMD})

@app.route('/right_t')
def right_t():
    requests.post(ESP_IP,data= "right_t")
    back_track.append({"left_t":S_PER_CMD})

@app.route('/cut')
def cut():
    requests.post(ESP_IP,data= "cut")
    

@app.route('/arm_up')
def arm_up():
    requests.post(ESP_IP,data= "arm_up")

@app.route('/arm_down')
def arm_down():
    requests.post(ESP_IP,data= "arm_down")

@app.route('/arm_left')
def arm_left():
    requests.post(ESP_IP,data= "arm_left")

@app.route('/arm_right')
def arm_right():
    requests.post(ESP_IP,data= "arm_right")

@app.route('/grab')
def grab():
    requests.post(ESP_IP,data= "grab")

@app.route('/release')
def release():
    requests.post(ESP_IP,data= "release")


if __name__ =="__main__":
    app.run(debug=True)

#http://127.0.0.1:5000/