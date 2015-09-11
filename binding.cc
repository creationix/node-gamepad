#include <node.h>
#include <nan.h>
#include <v8.h>
#include "gamepad/Gamepad.h"

using namespace v8;

Nan::Persistent<Object> persistentHandle;

NAN_METHOD(nGamepad_init) {
  Nan::HandleScope scope;
  Gamepad_init();
  return;
}

NAN_METHOD(nGamepad_shutdown) {
  Nan::HandleScope scope;
  Gamepad_shutdown();
  return;
}

NAN_METHOD(nGamepad_numDevices) {
  Nan::HandleScope scope;
  info.GetReturnValue().Set(Nan::New<Number>(Gamepad_numDevices()));
}

Local<Object> nGamepad_toObject(Gamepad_device* device) {
  Local<Object> obj = Nan::New<Object>();
  obj->Set(Nan::New("deviceID").ToLocalChecked(), Nan::New<Number>(device->deviceID));
  obj->Set(Nan::New("description").ToLocalChecked(), Nan::New<String>(device->description).ToLocalChecked());
  obj->Set(Nan::New("vendorID").ToLocalChecked(), Nan::New<Number>(device->vendorID));
  obj->Set(Nan::New("productID").ToLocalChecked(), Nan::New<Number>(device->productID));
  Handle<Array> axes = Nan::New<Array>(device->numAxes);
  for (unsigned int i = 0; i < device->numAxes; i++) {
    axes->Set(i, Nan::New<Number>(device->axisStates[i]));
  }
  obj->Set(Nan::New("axisStates").ToLocalChecked(), axes);
  Handle<Array> buttons = Nan::New<Array>(device->numButtons);
  for (unsigned int i = 0; i < device->numButtons; i++) {
    buttons->Set(i, Nan::New<Boolean>(device->buttonStates[i]));
  }
  obj->Set(Nan::New("buttonStates").ToLocalChecked(), buttons);
  return obj;
}

NAN_METHOD(nGamepad_deviceAtIndex) {
  Nan::HandleScope scope;
  int deviceIndex = info[0]->Int32Value();
  struct Gamepad_device* device = Gamepad_deviceAtIndex(deviceIndex);
  if (!device) return;
  info.GetReturnValue().Set(nGamepad_toObject(device));
}

NAN_METHOD(nGamepad_detectDevices) {
  Nan::HandleScope scope;
  Gamepad_detectDevices();
  return;
}

NAN_METHOD(nGamepad_processEvents) {
  Nan::HandleScope scope;
  Gamepad_processEvents();
  return;
}

void nGamepad_deviceAttach_cb(struct Gamepad_device* device, void* context) {
  Local<Value> info[] = {
    Nan::New("attach").ToLocalChecked(),
    Nan::New<Number>(device->deviceID),
    nGamepad_toObject(device),
  };
  Nan::MakeCallback(Nan::New<Object>(persistentHandle), "on", 3, info);
}

void nGamepad_deviceRemove_cb(struct Gamepad_device* device, void* context) {
  Local<Value> info[] = {
    Nan::New("remove").ToLocalChecked(),
    Nan::New<Number>(device->deviceID),
  };
  Nan::MakeCallback(Nan::New<Object>(persistentHandle), "on", 2, info);
}

void nGamepad_buttonDown_cb(struct Gamepad_device* device, unsigned int buttonID, double timestamp, void* context) {
  Local<Value> info[] = {
    Nan::New("down").ToLocalChecked(),
    Nan::New<Number>(device->deviceID),
    Nan::New<Number>(buttonID),
    Nan::New<Number>(timestamp),
  };
  Nan::MakeCallback(Nan::New<Object>(persistentHandle), "on", 4, info);
}

void nGamepad_buttonUp_cb(struct Gamepad_device* device, unsigned int buttonID, double timestamp, void* context) {
  Local<Value> info[] = {
    Nan::New("up").ToLocalChecked(),
    Nan::New<Number>(device->deviceID),
    Nan::New<Number>(buttonID),
    Nan::New<Number>(timestamp),
  };
  Nan::MakeCallback(Nan::New<Object>(persistentHandle), "on", 4, info);
}

void nGamepad_axisMove_cb(struct Gamepad_device* device, unsigned int axisID, float value, float lastValue, double timestamp, void * context) {
  Local<Value> info[] = {
    Nan::New("move").ToLocalChecked(),
    Nan::New<Number>(device->deviceID),
    Nan::New<Number>(axisID),
    Nan::New<Number>(value),
    Nan::New<Number>(lastValue),
    Nan::New<Number>(timestamp),
  };
  Nan::MakeCallback(Nan::New<Object>(persistentHandle), "on", 6, info);
}

void init(Handle<Object> target) {
  Local<Object> handle = Nan::New<Object>();
  persistentHandle.Reset(handle);

  target->Set(Nan::New<String>("context").ToLocalChecked(), handle);

  Gamepad_deviceAttachFunc(nGamepad_deviceAttach_cb, NULL);
  Gamepad_deviceRemoveFunc(nGamepad_deviceRemove_cb, NULL);
  Gamepad_buttonDownFunc(nGamepad_buttonDown_cb, NULL);
  Gamepad_buttonUpFunc(nGamepad_buttonUp_cb, NULL);
  Gamepad_axisMoveFunc(nGamepad_axisMove_cb, NULL);

  Nan::SetMethod(target, "init", nGamepad_init);
  Nan::SetMethod(target, "shutdown", nGamepad_shutdown);
  Nan::SetMethod(target, "numDevices", nGamepad_numDevices);
  Nan::SetMethod(target, "deviceAtIndex", nGamepad_deviceAtIndex);
  Nan::SetMethod(target, "detectDevices", nGamepad_detectDevices);
  Nan::SetMethod(target, "processEvents", nGamepad_processEvents);
}

NODE_MODULE(gamepad, init)
