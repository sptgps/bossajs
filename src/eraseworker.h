#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class EraseWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;

        uint32_t offset;

    public:
        EraseWorker(Nan::Callback* callback,
                    Bossa* bossa,
                    uint32_t offset)
                  : Nan::AsyncWorker(callback),
                    bossa(bossa),
                    offset(offset)
                    {}
        ~EraseWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};

