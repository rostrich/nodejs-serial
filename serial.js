exports.start = function(port, speed) {
  var ser = require("./build/Release/serial");
  ser.open(port, speed);

  var onRead = function(data) {
    var l = data.length;
    var s = "";
    for (i=0; i<l; i++) {
      s += String.fromCharCode(data[i]);
//    console.log(data[i]);
    }
    console.log("l=" + l + " '" + s + "'");
    ser.read(onRead);
  }

  ser.read(onRead);
  return ser;
}

