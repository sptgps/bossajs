#include <nan.h>

#define L(a) (Nan::New((a)).ToLocalChecked())

#define OBJ_GET(obj, key) (Nan::Get(config, L(key)).ToLocalChecked())