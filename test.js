var Gamepad = require("./.");

console.log("Gamepad", Gamepad);

// Create raw dispatcher to see all events.
Gamepad.context.on = console.log;

// Gamepad.on("down", function () {
//   console.log("DOWN", arguments);
// });
// Gamepad.on("up", function () {
//   console.log("UP", arguments);
// });
// Gamepad.on("move", function () {
//   console.log("MOVE", arguments);
// });
// Gamepad.on("attach", function () {
//   console.log("ATTACH", arguments);
// });
// Gamepad.on("remove", function () {
//   console.log("REMOVE", arguments);
// });

console.log("init");
Gamepad.init()

var num = Gamepad.numDevices();
console.log("numDevices", num);

setInterval(Gamepad.processEvents, 16);
setInterval(Gamepad.detectDevices, 500);

// console.log("shutdown", Gamepad.shutdown());
