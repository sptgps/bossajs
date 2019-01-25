#pragma once

#include <iostream>
#include <optional>

#include <nan.h>

#include "Flasher.h"
#include "Samba.h"
#include "PortFactory.h"
#include "Device.h"

#include "observer.h"


class NotConnected : public std::runtime_error {
    public:
        NotConnected() : std::runtime_error("Not connected") {}
};


class Bossa : public Nan::ObjectWrap {
    private:
        Samba samba;
        PortFactory portFactory;
        std::optional<Device> device;
        std::optional<Flasher> flasher;

        BossaObserver observer;

        bool connected = false;
    public:

        Bossa(bool debug) {
            samba.setDebug(debug);
        };

        void connect(std::string port);
        void info(FlasherInfo& info);

        static NAN_MODULE_INIT(Init);

        static NAN_METHOD(New);
        static NAN_METHOD(Connect);
        static NAN_METHOD(Info);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
};