#pragma once

#include <iostream>
#include <optional>
#include <string>

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
        std::string read(uint32_t offset, uint32_t size);

        static NAN_MODULE_INIT(Init);

        static NAN_METHOD(New);
        static NAN_METHOD(Connect);
        static NAN_METHOD(Info);
        static NAN_METHOD(Read);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
};