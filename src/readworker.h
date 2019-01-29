#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class ReadWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;

        uint32_t offset;
        uint32_t size;

        std::string result;

    public:
        ReadWorker(Nan::Callback* callback,
                   Bossa* bossa,
                   uint32_t offset,
                   uint32_t size)
                 : Nan::AsyncWorker(callback),
                   bossa(bossa),
                   offset(offset),
                   size(size)
                   {}
        ~ReadWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};
