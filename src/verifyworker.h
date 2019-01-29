#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class VerifyWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;

        std::string content;
        uint32_t offset;

    public:
        VerifyWorker(Nan::Callback* callback,
                    Bossa* bossa,
                    std::string content,
                    uint32_t offset)
                  : Nan::AsyncWorker(callback),
                    bossa(bossa),
                    content(content),
                    offset(offset)
                    {}
        ~VerifyWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};
