from flask import Flask, request

app = Flask(__name__)


@app.route("/", methods=["GET", "POST"])
def hello_world():
    print (request.data)
    return "<p>Hello, World!</p>"



 
