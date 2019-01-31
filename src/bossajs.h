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
        Bossa(bool debug)
            : observer(this)
        {
            samba.setDebug(debug);
        }
        ~Bossa() {}

        // C++ Methods
        void connect(std::string port);
        void close();
        void info(FlasherInfo& info);
        std::vector<char> read(uint32_t offset, uint32_t size);
        void erase(uint32_t offset);
        bool verify(Nan::TypedArrayContents<uint8_t>& buffer, u_int32_t offset);
        void write(Nan::TypedArrayContents<uint8_t>& buffer, uint32_t offset);

        // Node methods
        static NAN_MODULE_INIT(Init);

        static NAN_METHOD(New);
        static NAN_METHOD(Connect);
        static NAN_METHOD(Close);
        static NAN_METHOD(Info);
        static NAN_METHOD(Read);
        static NAN_METHOD(Erase);
        static NAN_METHOD(Verify);
        static NAN_METHOD(Write);

        static Nan::Persistent<v8::FunctionTemplate> constructor;
};