#include "bossajs.h"


Nan::Persistent<v8::FunctionTemplate> Bossa::constructor;


NAN_MODULE_INIT(Bossa::Init) {
    v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(Bossa::New);

    constructor.Reset(ctor);

    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(Nan::New("Bossa").ToLocalChecked());

    // Link getters and setters

    // Set Methods into Prototype

    // Export class
    target->Set(Nan::New("Bossa").ToLocalChecked(), ctor->GetFunction());
    target->Set(Nan::New("default").ToLocalChecked(), ctor->GetFunction());
}


NAN_METHOD(Bossa::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowError(Nan::New("Must call with new keyword").ToLocalChecked());
    }

    if (info.Length() != 1) {
        return Nan::ThrowTypeError(Nan::New("opts expected").ToLocalChecked());
    }

    if (!info[0]->IsObject()) {
        return Nan::ThrowTypeError(Nan::New("opts must be an object").ToLocalChecked());
    }

    // Initialise and wrap our instance.
    Bossa *bossa = new Bossa();
    bossa->Wrap(info.Holder());

    info.GetReturnValue().Set(info.Holder());
}
