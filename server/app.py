from flask import Flask,render_template, request, redirect
import requests

app = Flask(__name__)

#constants
M_PER_S = 0.1
IMG_X_SIZE = 100
IMG_Y_SIZE= 100
FIELD_X_SIZE=5
FIELD_Y_SIZE=5
ESP_IP = ""

@app.route('/')
def home():
    return render_template("home.html")

@app.route('/forward')
def forward():
    requests.post(ESP_IP,data= "forward")
    return 0

@app.route('/back')
def back():
    requests.post(ESP_IP,data= "back")
    return 0

@app.route('/left_t')
def left_t():
    requests.post(ESP_IP,data= "left_t")
    return 0

@app.route('/right_t')
def right_t():
    requests.post(ESP_IP,data= "right_t")
    return 0

@app.route('/new_coord')
def move_to_position():
    pixel = request.query_string
    x_pixel,y_pixel= pixel.split(",")
    x_time = FIELD_X_SIZE*(x_pixel/IMG_X_SIZE)/M_PER_S
    y_time = FIELD_Y_SIZE*(y_pixel/IMG_Y_SIZE)/M_PER_S
    requests.post(ESP_IP,data = (x_time,y_time))
    return redirect("localhost:5000/")



if __name__ =="__main__":
    app.run(debug=True)

#http://127.0.0.1:5000/