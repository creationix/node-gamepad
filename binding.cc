#include <node.h>
#include <nan.h>
#include <v8.h>
#include "gamepad/Gamepad.h"

using namespace v8;

Persistent<Object> persistentHandle;

NAN_METHOD(nGamepad_init) {
  NanScope();
  Gamepad_init();
  NanReturnUndefined();
}

NAN_METHOD(nGamepad_shutdown) {
  NanScope();
  Gamepad_shutdown();
  NanReturnUndefined();
}

NAN_METHOD(nGamepad_numDevices) {
  NanScope();
  NanReturnValue(NanNew<Number>(Gamepad_numDevices()));
}

Local<Object> nGamepad_toObject(Gamepad_device* device) {
  Local<Object> obj = NanNew<Object>();
  obj->Set(NanNew("deviceID"), NanNew<Number>(device->deviceID));
  obj->Set(NanNew("description"), NanNew<String>(device->description));
  obj->Set(NanNew("vendorID"), NanNew<Number>(device->vendorID));
  obj->Set(NanNew("productID"), NanNew<Number>(device->productID));
  Handle<Array> axes = NanNew<Array>(device->numAxes);
  for (unsigned int i = 0; i < device->numAxes; i++) {
    axes->Set(i, NanNew<Number>(device->axisStates[i]));
  }
  obj->Set(NanNew("axisStates"), axes);
  Handle<Array> buttons = NanNew<Array>(device->numButtons);
  for (unsigned int i = 0; i < device->numButtons; i++) {
    buttons->Set(i, NanNew<Boolean>(device->buttonStates[i]));
  }
  obj->Set(NanNew("buttonStates"), buttons);
  return obj;
}

NAN_METHOD(nGamepad_deviceAtIndex) {
  NanScope();
  int deviceIndex = args[0]->Int32Value();
  struct Gamepad_device* device = Gamepad_deviceAtIndex(deviceIndex);
  if (!device) NanReturnUndefined();
  NanReturnValue(nGamepad_toObject(device));
}

NAN_METHOD(nGamepad_detectDevices) {
  NanScope();
  Gamepad_detectDevices();
  NanReturnUndefined();
}

NAN_METHOD(nGamepad_processEvents) {
  NanScope();
  Gamepad_processEvents();
  NanReturnUndefined();
}

void nGamepad_deviceAttach_cb(struct Gamepad_device* device, void* context) {
  Local<Value> args[] = {
    NanNew("attach"),
    NanNew<Number>(device->deviceID),
    nGamepad_toObject(device),
  };
  NanMakeCallback(NanNew<Object>(persistentHandle), "on", 3, args);
}

void nGamepad_deviceRemove_cb(struct Gamepad_device* device, void* context) {
  Local<Value> args[] = {
    NanNew("remove"),
    NanNew<Number>(device->deviceID),
  };
  NanMakeCallback(NanNew<Object>(persistentHandle), "on", 2, args);
}

void nGamepad_buttonDown_cb(struct Gamepad_device* device, unsigned int buttonID, double timestamp, void* context) {
  Local<Value> args[] = {
    NanNew("down"),
    NanNew<Number>(device->deviceID),
    NanNew<Number>(buttonID),
    NanNew<Number>(timestamp),
  };
  NanMakeCallback(NanNew<Object>(persistentHandle), "on", 4, args);
}

void nGamepad_buttonUp_cb(struct Gamepad_device* device, unsigned int buttonID, double timestamp, void* context) {
  Local<Value> args[] = {
    NanNew("up"),
    NanNew<Number>(device->deviceID),
    NanNew<Number>(buttonID),
    NanNew<Number>(timestamp),
  };
  NanMakeCallback(NanNew<Object>(persistentHandle), "on", 4, args);
}

void nGamepad_axisMove_cb(struct Gamepad_device* device, unsigned int axisID, float value, float lastValue, double timestamp, void * context) {
  Local<Value> args[] = {
    NanNew("move"),
    NanNew<Number>(device->deviceID),
    NanNew<Number>(axisID),
    NanNew<Number>(value),
    NanNew<Number>(lastValue),
    NanNew<Number>(timestamp),
  };
  NanMakeCallback(NanNew<Object>(persistentHandle), "on", 6, args);
}

void init(Handle<Object> target) {
  Local<Object> handle = NanNew<Object>();
  NanAssignPersistent(persistentHandle, handle);

  target->Set(NanNew<String>("context"), handle);

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
