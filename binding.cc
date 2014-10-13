#include <node.h>
#include <v8.h>
#include "gamepad/Gamepad.h"

using namespace v8;

Handle<Value> nGamepad_init(const Arguments& args) {
  HandleScope scope;
  Gamepad_init();
  return Undefined();
}

void init(Handle<Object> target) {
  NODE_SET_METHOD(target, "init", nGamepad_init);
}

NODE_MODULE(gamepad, init)
