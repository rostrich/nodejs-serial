#!/usr/local/bin/node 

serial = require("serial");
var s = serial.start("/dev/tty.usbserial-A6004kUQ", 115200);

