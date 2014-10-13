var Gamepad = require("./build/Release/gamepad.node");

console.log("Gamepad", Gamepad);

Gamepad.context.on = function () {
  console.log.apply(console, arguments);
};

console.log("init");
Gamepad.init()

var num = Gamepad.numDevices();
console.log("numDevices", num);

setInterval(Gamepad.processEvents, 16);


// console.log("shutdown", Gamepad.shutdown());
