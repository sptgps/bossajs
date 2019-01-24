#include <iostream>
#include <string>

#include "bossajs.h"
#include "util.h"

#include "Flasher.h"
#include "Samba.h"
#include "PortFactory.h"
#include "Device.h"

using namespace v8;


Nan::Persistent<FunctionTemplate> Bossa::constructor;


class BossaObserver : public FlasherObserver {
    public:
        BossaObserver() : _lastTicks(-1) {}
        virtual ~BossaObserver() {}
        
        virtual void onStatus(const char *message, ...);
        virtual void onProgress(int num, int div);
    private:
        int _lastTicks;
};

void BossaObserver::onStatus(const char *message, ...) {
    va_list ap;
    
    va_start(ap, message);
    vprintf(message, ap);
    va_end(ap);
}

void BossaObserver::onProgress(int num, int div) {
    int ticks;
    int bars = 30;

    ticks = num * bars / div;
    
    if (ticks == _lastTicks)
        return;
    
    printf("\r[");
    while (ticks-- > 0)
    {
        putchar('=');
        bars--;
    }
    while (bars-- > 0)
    {
        putchar(' ');
    }
    printf("] %d%% (%d/%d pages)", num * 100 / div, num, div);
    fflush(stdout);
    
    _lastTicks = 0;
}


NAN_MODULE_INIT(Bossa::Init) {
    Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Bossa::New);

    constructor.Reset(ctor);

    ctor->InstanceTemplate()->SetInternalFieldCount(1);
    ctor->SetClassName(L("Bossa"));

    // Link getters and setters

    // Set Methods into Prototype

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

        Samba samba;
        PortFactory portFactory;
        std::string port = portFactory.def();

        value = OBJ_GET(config, "debug");
        if (AS_BOOL(value)) {
            samba.setDebug(true);
        }

        value = OBJ_GET(config, "port");
        if (value->IsNullOrUndefined()) {
            std::cout << "No port\n";
        } else {
            Nan::Utf8String port_(value);
            port = std::string(*port_);
        }

        if (!samba.connect(portFactory.create(port))) {
            return Nan::ThrowError(L("No device found on " + port));
        }

        Device device(samba);
        device.create();

        Device::FlashPtr& flash = device.getFlash();

        BossaObserver observer;
        Flasher flasher(samba, device, observer);

        // FIXME: move this to its own method
        value = OBJ_GET(config, "info");
        if (AS_BOOL(value)) {
            FlasherInfo info;
            flasher.info(info);
            info.print();
        }

        // Initialise and wrap our instance.
        Bossa *bossa = new Bossa();
        bossa->Wrap(info.Holder());

        info.GetReturnValue().Set(info.Holder());
    } catch(...) {
        return Nan::ThrowError(L("Unhandled exception"));
    }
}
