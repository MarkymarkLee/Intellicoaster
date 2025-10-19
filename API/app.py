from flask import Flask
from flask import request,abort
import os
import json
from datetime import date

do_not_disturb = False

def get_date():
    return f"{date.today()}"

admin_token = "admin_token"
with open("admin_token.json", "w") as f:
    data = {
        "name": "admin",
        "do_not_disturb": False,
        "should_remind": False,
        "device": 0,
        "water": {
            "2023-04-30": 1200,
        }
    }
    json.dump(data,f)

app = Flask(__name__)


@app.route('/')
def hello():
    return 'This is API for makeNTU'

@app.route('/send_water/<token>/<water>',methods=["GET","POST"])
def send_water(token,water):
    
    userpath = f"./{token}.json"
    water = float(water)
    if not os.path.isfile(userpath):
        abort(400)
    if water < 0:
        abort(400)
    
    with open(userpath,"r") as datafile:
        userdata = json.load(datafile)

    userdata["water"][get_date()] = water

    with open(userpath,"w") as datafile:
        json.dump(userdata,datafile)

    return "ok"

@app.route('/remind/<token>',methods=["GET"])
def remind(token):
    if not token:
        abort(400)
    userpath = f"./{token}.json"
    if not os.path.isfile(userpath):
        abort(400)

    with open(userpath,"r") as datafile:
        userdata = json.load(datafile)
    remind = not userdata["do_not_disturb"]

    if not userdata["do_not_disturb"] and userdata["device"]==1:
        remind = False
        userdata["should_remind"] = True
        with open(userpath,"w") as datafile:
            json.dump(userdata,datafile)


    return {
        "remind": remind
    }

@app.route('/get_water/<token>/<date>',methods=["GET"])
def get_water(token,date):
    if not token or not date:
        abort(400)
    userpath = f"./{token}.json"
    if not os.path.isfile(userpath):
        abort(400)
    
    with open(userpath,"r") as datafile:
        userdata = json.load(datafile)
    if date in userdata["water"]:
        water = userdata["water"][date]
    else:
        abort(400)
    
    remind = False
    if userdata["should_remind"] and not userdata["do_not_disturb"]:
        remind = True

    return {
        "water": water,
        "remind": remind
    }

@app.route('/do_not_disturb/<token>/<no_disturb>',methods=["GET","POST"])
def do_not_disturb(token,no_disturb):
    if not token:
        abort(400)
    userpath = f"./{token}.json"
    if not os.path.isfile(userpath):
        abort(400)

    with open(userpath,"r") as datafile:
        userdata = json.load(datafile)
    if no_disturb == "true":
        userdata["do_not_disturb"] = True
    elif no_disturb == "false":
        userdata["do_not_disturb"] = False
    else:
        abort(400)
    userdata["do_not_disturb"] = no_disturb
    with open(userpath,"w") as datafile:
        json.dump(userdata,datafile)
    return "ok"
    
@app.route('/change_device/<token>/<device>',methods=["GET","POST"])
def change_device(token,device):
    if not token or not device:
        abort(400)
    userpath = f"./{token}.json"
    if not os.path.isfile(userpath):
        abort(400)

    with open(userpath,"r") as datafile:
        userdata = json.load(datafile)

    if device == "computer":
        userdata["device"] = 1
    elif device == "coaster":
        userdata["device"] = 0
    else:
        abort(400)
    with open(userpath,"w") as datafile:
        json.dump(userdata,datafile)
    return "ok"
