from flask import Flask,render_template, request
import requests

app = Flask(__name__)


@app.route('/')
def home():
    return render_template("home.html")

@app.route('/new_coord')
def move_to_position():
    m_per_s = 0.1
    img_x_size = 100
    img_y_size= 100
    field_x_size=5
    field_y_size=5
    pixel = request.query_string
    x_pixel,y_pixel= pixel.split(",")
    x_time = field_x_size*(x_pixel/img_x_size)/m_per_s
    y_time = field_y_size*(y_pixel/img_y_size)/m_per_s
    requests.post("esp ip address",data = (x_time,y_time))

    return render_template("home.html")



if __name__ =="__main__":
    app.run(debug=True)

#http://127.0.0.1:5000/