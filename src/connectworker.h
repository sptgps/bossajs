#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class ConnectWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;
        std::string port;

    public:
        ConnectWorker(Nan::Callback* callback,
                      Bossa* bossa,
                      std::string port)
                    : Nan::AsyncWorker(callback),
                      bossa(bossa),
                      port(port)
                      {}
        ~ConnectWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};
