#include <nan.h>


class Bossa : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init);

        static NAN_METHOD(New);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
};