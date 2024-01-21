from flask import Flask,render_template, request
import requests

app = Flask(__name__)


@app.route('/')
def home():
    return render_template("home.html")

@app.route('/new_coord')
def move_to_position():
    pixel = request.query_string
    x_pixel,y_pixel= pixel.split(",")
    #calculate the position
    #x_pos = x*(x_pix/imagex)
    #y_pos = y*(y_pix/imagey)

    requests.get("esp ip address")

    return render_template("home.html")



if __name__ =="__main__":
    app.run(debug=True)

#http://127.0.0.1:5000/