#!/bin/bash

import serial
import struct

class Communication:
    LED1        = 1;
    LED2        = 2;
    LED_PWM     = 3;

    ACTION_SET  = 's';
    ACTION_GET  = 'g';

    ON          = 1;
    OFF         = 0;

    def __init__(self):
        self.port = serial.Serial();

    def open(self, port):
        self.port.port = port;
        self.port.baudrate = 9600;
        self.port.bytesize = 8;
        self.port.stopbits = 1;
        self.port.parity = "N";
        self.port.open();

    def close(self):
        self.port.close();

    def setLedState(self, led, value):
        msg = self.ACTION_SET;
        msg += struct.pack("BB", led, value);
        self.port.write(msg);

    def getLedState(self, led):
        msg = self.ACTION_GET;
        msg += struct.pack("BB", led, 0);
        self.port.write(msg);
        state = self.port.read();

        if led == self.LED_PWM:
            return state;

        if state == '\x00':
            return False;
        else:
            return True;

    def getLedPwm(self):
        return self.getLedState(self.LED_PWM);

    def setLedPwm(self, pwm):
        self.setLedState(self.LED_PWM, pwm);

    def ledOn(self, led):
        if led.lower() == "led1":
            self.setLedState(self.LED1, self.ON);
        elif led.lower() == "led2":
            self.setLedState(self.LED2, self.ON);

    def ledOff(self, led):
        if led.lower() == "led1":
            self.setLedState(self.LED1, self.OFF);
        elif led.lower() == "led2":
            self.setLedState(self.LED2, self.OFF);

    def ledPwm(self, pwm):
        self.setLedPwm(pwm);

    def ledState(self, led):
        if led.lower() == "led1":
            return self.getLedState(self.LED1);
        elif led.lower() == "led2":
            return self.getLedState(self.LED2);
        elif led.lower() == "led3":
            return self.getLedPwm();

