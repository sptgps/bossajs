#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class VerifyWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;

        Nan::TypedArrayContents<uint8_t> buffer;
        uint32_t offset;

    public:
        VerifyWorker(Nan::Callback* callback,
                    Bossa* bossa,
                    Local<Object> buffer,
                    uint32_t offset)
                  : Nan::AsyncWorker(callback),
                    bossa(bossa),
                    buffer(buffer),
                    offset(offset)
                    {}
        ~VerifyWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};
