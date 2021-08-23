from flask import Flask, request
import json

app = Flask(__name__)


@app.route("/", methods=["GET", "POST"])
def hello_world():
    values = extract_received_values(request.data)
    dump_received_values(values)
    return ""


def extract_received_values(data):
    if data:
        try:
            values = json.loads(data)
        except:
            values = []
    return values

def dump_received_values(data):
    for value in data:
        print("*" * int(1 + 78 * value / 1024))
