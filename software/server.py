#!/usr/bin/python

from flask import Flask, render_template
from flask_socketio import SocketIO
from communication import Communication

app = Flask(__name__);
app.config["SECRET_KEY"] = "RemoteLab";

socket = SocketIO(app);

port = Communication();
port.open("/dev/ttyUSB0");

@app.route("/")
def index():
    return render_template("index.html");

@socket.on("connect")
def clientConnected():
    sendState();

@socket.on("setLedState")
def setLedState(led, state):
    if state.lower() == "on":
        port.ledOn(led);
    else:
        port.ledOff(led);
    sendState();

@socket.on("setLedPwm")
def setLedPwm(pwm):
    port.ledPwm(pwm);

@socket.on("updateState")
def updateState():
    sendState();

def sendState():
    led1 = "On";
    led2 = "On";
    led3 = ord(port.ledState("led3"));

    if port.ledState("led1"):
        led1 = "Off";

    if port.ledState("led2"):
        led2 = "Off";

    socket.emit("updateState", { "led1": led1,
                                 "led2": led2,
                                 "led3": led3});

if __name__ == "__main__":
    socket.run(app);

