#pragma once

#include <iostream>
#include <optional>

#include <nan.h>

#include "Flasher.h"
#include "Samba.h"
#include "PortFactory.h"
#include "Device.h"

#include "observer.h"


class Bossa : public Nan::ObjectWrap {
    private:
        Samba samba;
        PortFactory portFactory;
        std::optional<Device> device;

        BossaObserver observer;

    public:
        std::optional<Flasher> flasher;

        Bossa(std::string port, bool debug);

        static NAN_MODULE_INIT(Init);

        static NAN_METHOD(New);
        static NAN_METHOD(Info);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
};