var Gamepad = require('bindings')('gamepad.node');

var EventEmitter = require('events').EventEmitter;

Gamepad.__proto__ = EventEmitter.prototype;
EventEmitter.call(Gamepad);
Gamepad.context.on = function () {
  Gamepad.emit.apply(Gamepad, arguments);
};

module.exports = Gamepad;
