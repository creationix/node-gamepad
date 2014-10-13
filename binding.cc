#include <node.h>
#include <v8.h>
#include "gamepad/Gamepad.h"

using namespace v8;

Persistent<Object> context_obj = Persistent<Object>::New(Object::New());

Handle<Value> nGamepad_init(const Arguments& args) {
  HandleScope scope;
  Gamepad_init();
  return Undefined();
}

Handle<Value> nGamepad_shutdown(const Arguments& args) {
  HandleScope scope;
  Gamepad_shutdown();
  return Undefined();
}

Handle<Value> nGamepad_numDevices(const Arguments& args) {
  HandleScope scope;
  return Number::New(Gamepad_numDevices());
}

Local<Object> nGamepad_toObject(Gamepad_device* device) {
  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("deviceID"), Number::New(device->deviceID));
  obj->Set(String::NewSymbol("description"), String::New(device->description));
  obj->Set(String::NewSymbol("vendorID"), Number::New(device->vendorID));
  obj->Set(String::NewSymbol("productID"), Number::New(device->productID));
  Handle<Array> axes = Array::New(device->numAxes);
  for (unsigned int i = 0; i < device->numAxes; i++) {
    axes->Set(i, Number::New(device->axisStates[i]));
  }
  obj->Set(String::NewSymbol("axisStates"), axes);
  Handle<Array> buttons = Array::New(device->numButtons);
  for (unsigned int i = 0; i < device->numButtons; i++) {
    buttons->Set(i, Boolean::New(device->buttonStates[i]));
  }
  obj->Set(String::NewSymbol("buttonStates"), buttons);
  return obj;
}

Handle<Value> nGamepad_deviceAtIndex(const Arguments& args) {
  HandleScope scope;
  int deviceIndex = args[0]->Int32Value();
  struct Gamepad_device* device = Gamepad_deviceAtIndex(deviceIndex);
  if (!device) return Undefined();
  return scope.Close(nGamepad_toObject(device));
}

Handle<Value> nGamepad_detectDevices(const Arguments& args) {
  HandleScope scope;
  Gamepad_detectDevices();
  return Undefined();
}

Handle<Value> nGamepad_processEvents(const Arguments& args) {
  HandleScope scope;
  Gamepad_processEvents();
  return Undefined();
}

void nGamepad_deviceAttach_cb(struct Gamepad_device* device, void* context) {
  Local<Value> args[] = {
    String::NewSymbol("attach"),
    Number::New(device->deviceID),
    nGamepad_toObject(device),
  };
  node::MakeCallback(context_obj, "on", 3, args);
}

void nGamepad_deviceRemove_cb(struct Gamepad_device* device, void* context) {
  Local<Value> args[] = {
    String::NewSymbol("remove"),
    Number::New(device->deviceID),
  };
  node::MakeCallback(context_obj, "on", 2, args);
}

void nGamepad_buttonDown_cb(struct Gamepad_device* device, unsigned int buttonID, double timestamp, void* context) {
  Local<Value> args[] = {
    String::NewSymbol("down"),
    Number::New(device->deviceID),
    Number::New(buttonID),
    Number::New(timestamp),
  };
  node::MakeCallback(context_obj, "on", 4, args);
}

void nGamepad_buttonUp_cb(struct Gamepad_device* device, unsigned int buttonID, double timestamp, void* context) {
  Local<Value> args[] = {
    String::NewSymbol("up"),
    Number::New(device->deviceID),
    Number::New(buttonID),
    Number::New(timestamp),
  };
  node::MakeCallback(context_obj, "on", 4, args);
}

void nGamepad_axisMove_cb(struct Gamepad_device* device, unsigned int axisID, float value, float lastValue, double timestamp, void * context) {
  Local<Value> args[] = {
    String::NewSymbol("move"),
    Number::New(device->deviceID),
    Number::New(axisID),
    Number::New(value),
    Number::New(lastValue),
    Number::New(timestamp),
  };
  node::MakeCallback(context_obj, "on", 6, args);
}

void init(Handle<Object> target) {
  target->Set(String::New("context"), context_obj, DontEnum);

  Gamepad_deviceAttachFunc(nGamepad_deviceAttach_cb, NULL);
  Gamepad_deviceRemoveFunc(nGamepad_deviceRemove_cb, NULL);
  Gamepad_buttonDownFunc(nGamepad_buttonDown_cb, NULL);
  Gamepad_buttonUpFunc(nGamepad_buttonUp_cb, NULL);
  Gamepad_axisMoveFunc(nGamepad_axisMove_cb, NULL);

  NODE_SET_METHOD(target, "init", nGamepad_init);
  NODE_SET_METHOD(target, "shutdown", nGamepad_shutdown);
  NODE_SET_METHOD(target, "numDevices", nGamepad_numDevices);
  NODE_SET_METHOD(target, "deviceAtIndex", nGamepad_deviceAtIndex);
  NODE_SET_METHOD(target, "detectDevices", nGamepad_detectDevices);
  NODE_SET_METHOD(target, "processEvents", nGamepad_processEvents);
}

NODE_MODULE(gamepad, init)
