#include <iostream>
#include <string>

#include "bossajs.h"
#include "util.h"

using namespace v8;


Nan::Persistent<FunctionTemplate> Bossa::constructor;


void BossaObserver::onStatus(const char* message, ...) {
    va_list ap;
    
    va_start(ap, message);
    vprintf(message, ap);
    va_end(ap);
}

void BossaObserver::onProgress(int num, int div) {
}


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
    Local<Object> obj = Nan::New<Object>();
    FlasherInfo finfo;

    self->flasher->info(finfo);

    obj->Set(L("name"), L(finfo.name));
    obj->Set(L("chipId"), Nan::New(finfo.chipId));
    obj->Set(L("extChipId"), Nan::New(finfo.extChipId));
    obj->Set(L("version"), L(finfo.version));
    obj->Set(L("address"), Nan::New(finfo.address));
    obj->Set(L("numPages"), Nan::New(finfo.numPages));
    obj->Set(L("pageSize"), Nan::New(finfo.pageSize));
    obj->Set(L("totalSize"), Nan::New(finfo.totalSize));
    obj->Set(L("numPlanes"), Nan::New(finfo.numPlanes));

    obj->Set(L("security"), Nan::New(finfo.security));
    obj->Set(L("bootFlash"), Nan::New(finfo.bootFlash));
    obj->Set(L("bod"), Nan::New(finfo.bod));
    obj->Set(L("bor"), Nan::New(finfo.bor));

    obj->Set(L("canBootFlash"), Nan::New(finfo.canBootFlash));
    obj->Set(L("canBod"), Nan::New(finfo.canBod));
    obj->Set(L("canBor"), Nan::New(finfo.canBor));
    obj->Set(L("canChipErase"), Nan::New(finfo.canChipErase));
    obj->Set(L("canWriteBuffer"), Nan::New(finfo.canWriteBuffer));
    obj->Set(L("canChecksumBuffer"), Nan::New(finfo.canChecksumBuffer));

    Local<Array> lockRegions = Nan::New<Array>(finfo.lockRegions.size());
    unsigned index = 0;
    for (auto elem : finfo.lockRegions) {
        Nan::Set(lockRegions, index, Nan::New(elem));
        index++;
    }
    obj->Set(L("lockRegions"), lockRegions);

    info.GetReturnValue().Set(obj);
}