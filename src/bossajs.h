#include <iostream>
#include <optional>

#include <nan.h>

#include "Flasher.h"
#include "Samba.h"
#include "PortFactory.h"
#include "Device.h"


class BossaObserver : public FlasherObserver {
    private:

    public:
        BossaObserver() {}
        virtual ~BossaObserver() {}
        
        virtual void onStatus(const char *message, ...);
        virtual void onProgress(int num, int div);
};


class Bossa : public Nan::ObjectWrap {
    private:
        Samba samba;
        PortFactory portFactory;
        std::optional<Device> device;
        std::optional<Flasher> flasher;

        BossaObserver observer;

    public:
        Bossa(std::string port, bool debug);

        static NAN_MODULE_INIT(Init);

        static NAN_METHOD(New);
        static NAN_METHOD(Info);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
};