node-gamepad
============

Bindings for Alex Diener's cross-platform gamepad code

The library is tiny and included inside this addon so you don't need any special libraries installed on your system like SDL.

To install as a dependency simple type the following in your project's folder:

```sh
npm install gamepad
```

## Examples

```js
var gamepad = require("gamepad");

// Initialize the library
gamepad.init()

// List the state of all currently attached devices
for (var i = 0, l = gamepad.numDevices(); i < l; i++) {
  console.log(i, gamepad.deviceAtIndex());
}

// Create a game loop and poll for events
setInterval(gamepad.processEvents, 16);
// Scan for new gamepads as a slower rate
setInterval(gamepad.detectDevices, 500);

// Listen for move events on all gamepads
gamepad.on("move", function (id, axis, value) {
  console.log("move", {
    id: id,
    axis: axis,
    value: value,
  });
});

// Listen for button up events on all gamepads
gamepad.on("up", function (id, num) {
  console.log("up", {
    id: id,
    num: num,
  });
});

// Listen for button down events on all gamepads
gamepad.on("down", function (id, num) {
  console.log("down", {
    id: id,
    num: num,
  });
});

```

## Events

There are 5 event types `attach`, `remove`, `down`, `up`, and `move`.  All pass the session id of the gamepad as the first argument.

The `attach` event also passes current state as the second arg to attach.

The `down` and `up` events also pass in the button id and timestamp after device session id.

The `move` event passes axis, value, old value and timestamp after session id.

## Functions

### gamepad.init()

Initializes gamepad library and detects initial devices. Call this before any
other library function. If you want to receive attach events callbacks from
devices detected in `gamepad.init()`, you must listen for `attach` before
calling `gamepad.init()`.

### gamepad.shutdown()

Tears down all data structures created by the gamepad library and releases any
memory that was allocated. It is not necessary to call this function at
application termination, but it's provided in case you want to free memory
associated with gamepads at some earlier time.

### numDevices()

Returns the number of currently attached gamepad devices.

### deviceAtIndex(deviceIndex)

Returns the specified gamepad's current state, or `undefined` if deviceIndex is
out of bounds.

### detectDevices()

Polls for any devices that have been attached since the last call to
`gamepad.detectDevices()` or `gamepad.init()`. If any new devices are found, the
`attach` event will be emitted once per newly detected device.

Note that depending on implementation, you may receive button and axis event
callbacks for devices that have not yet been detected with
`gamepad.detectDevices()`. You can safely ignore these events, but be aware that
your callbacks might receive a device ID that hasn't been emitted as an `attach`
event yet.

### processEvents()

Reads pending input from all attached devices and emits the appropriate events,
if any have been registered.
