#include <nan.h>

#define L(a) (Nan::New((a)).ToLocalChecked())

#define OBJ_GET(obj, key) (Nan::Get(config, L(key)).ToLocalChecked())
#define AS_BOOL(value) (Nan::To<bool>(value).FromMaybe(false))