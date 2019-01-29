#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class WriteWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;

        std::string content;
        uint32_t offset;

    public:
        WriteWorker(Nan::Callback* callback,
                    Bossa* bossa,
                    std::string content,
                    uint32_t offset)
                  : Nan::AsyncWorker(callback),
                    bossa(bossa),
                    content(content),
                    offset(offset)
                    {}
        ~WriteWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};
