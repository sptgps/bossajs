#include <iostream>
#include <string>

#include "bossajs.h"
#include "infoworker.h"
#include "util.h"

using namespace v8;


Nan::Persistent<FunctionTemplate> Bossa::constructor;


Bossa::Bossa(std::string port, bool debug) {
    std::cout << "Called with port " << port << ".";
    samba.setDebug(debug);

    if (port.empty()) {
        port = portFactory.def();
    }

    if (port.empty()) {
        throw std::runtime_error("No serial ports available");
    }

    if (!samba.connect(portFactory.create(port))) {
        throw std::runtime_error("No device found on " + port);
    }

    device.emplace(samba);
    device->create();

    flasher.emplace(samba, *device, observer);

}


NAN_MODULE_INIT(Bossa::Init) {
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);

    constructor.Reset(ctor);

    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(L("Bossa"));

    // Link getters and setters

    // Set Methods into Prototype
    Nan::SetPrototypeMethod(ctor, "info", Info);

    // Export class
    target->Set(L("Bossa"), ctor->GetFunction());
    target->Set(L("default"), ctor->GetFunction());
}


NAN_METHOD(Bossa::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowError(L("Must call with new keyword"));
    }

    if (info.Length() != 1 || !info[0]->IsObject()) {
        return Nan::ThrowTypeError(L("opts must be an object"));
    }

    Local<Object> config = Nan::To<Object>(info[0]).ToLocalChecked();

    try {
        Local<Value> value;

        value = OBJ_GET(config, "debug");
        bool debug = AS_BOOL(value);

        std::string port;
        value = OBJ_GET(config, "port");
        if (!value->IsNullOrUndefined()) {
            Nan::Utf8String port_(value);
            port = std::string(*port_);
        }

        Bossa* self = new Bossa(port, debug);

        // // Device::FlashPtr& flash = bossa->device.getFlash();

        self->Wrap(info.Holder());
        info.GetReturnValue().Set(info.Holder());
    } catch(std::exception& exc) {
        return Nan::ThrowError(L(exc.what()));
    }
}


NAN_METHOD(Bossa::Info) {
    Bossa* self = Nan::ObjectWrap::Unwrap<Bossa>(info.This());

    if (info.Length() != 1 || !info[0]->IsFunction()) {
        return Nan::ThrowTypeError(L("callback must be a function"));
    }

    Nan::Callback* callback =
        new Nan::Callback(Nan::To<Function>(info[0]).ToLocalChecked());
    Nan::AsyncQueueWorker(new InfoWorker(callback, self));

}