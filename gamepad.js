var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));
var Gamepad = require(binding_path);

var EventEmitter = require('events').EventEmitter;

Gamepad.__proto__ = EventEmitter.prototype;
EventEmitter.call(Gamepad);
Gamepad.context.on = function () {
  Gamepad.emit.apply(Gamepad, arguments);
};

module.exports = Gamepad;
