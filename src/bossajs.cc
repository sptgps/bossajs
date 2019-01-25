#include <string>

#include "bossajs.h"
#include "connectworker.h"
#include "infoworker.h"
#include "util.h"

using namespace v8;


Nan::Persistent<FunctionTemplate> Bossa::constructor;


void Bossa::info(FlasherInfo& finfo) {
    if (!connected) {
        throw NotConnected();
    }

    flasher->info(finfo);
}


void Bossa::connect(std::string port) {
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

    connected = true;
}


NAN_MODULE_INIT(Bossa::Init) {
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(New);

    constructor.Reset(ctor);

    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(L("Bossa"));

    // Link getters and setters

    // Set Methods into Prototype
    Nan::SetPrototypeMethod(ctor, "connect", Connect);
    Nan::SetPrototypeMethod(ctor, "info", Info);

    // Export class
    target->Set(L("Bossa"), ctor->GetFunction());
    target->Set(L("default"), ctor->GetFunction());
}


NAN_METHOD(Bossa::New) {
    bool debug = false;

    if (!info.IsConstructCall()) {
        return Nan::ThrowError(L("Must call with new keyword"));
    }

    if (info.Length() == 0) {
        // Pass
    } else if (info.Length() == 1) {
        if (!info[0]->IsObject()) {
            return Nan::ThrowTypeError(L("opts must be an object"));
        } else {
            // Load config
            Local config = Nan::To<Object>(info[0]).ToLocalChecked();
            debug = AS_BOOL(OBJ_GET(config, "debug"));
        }
    } else {
        return Nan::ThrowTypeError(L("opts must be an object or nothing"));
    }

    try {

        Bossa* self = new Bossa(debug);

        // // Device::FlashPtr& flash = bossa->device.getFlash();

        self->Wrap(info.Holder());
        info.GetReturnValue().Set(info.Holder());
    } catch(const std::exception& exc) {
        return Nan::ThrowError(L(exc.what()));
    }
}


NAN_METHOD(Bossa::Connect) {
    Bossa* self = Nan::ObjectWrap::Unwrap<Bossa>(info.This());
    std::string port;

    if (info.Length() != 2) {
        return Nan::ThrowTypeError(L("Must provide port and callback"));
    }
    
    if (!info[0]->IsString()) {
        return Nan::ThrowTypeError(L("port must be a string"));
    }

    if (!info[1]->IsFunction()) {
        return Nan::ThrowTypeError(L("callback must be a function"));
    }

    Local value = Nan::To<String>(info[0]).ToLocalChecked();

    if (!value->IsNullOrUndefined()) {
        Nan::Utf8String port_(value);
        port = std::string(*port_);
    }

    Nan::Callback* callback =
        new Nan::Callback(Nan::To<Function>(info[1]).ToLocalChecked());

    Nan::AsyncQueueWorker(new ConnectWorker(callback, self, port));
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