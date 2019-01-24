#include <nan.h>

#include "bossajs.h"


NAN_MODULE_INIT(InitModule) {
    Bossa::Init(target);
}


NODE_MODULE(addon, InitModule);