#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class CloseWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;

    public:
        CloseWorker(Nan::Callback* callback,
                    Bossa* bossa)
                  : Nan::AsyncWorker(callback),
                    bossa(bossa)
                    {}
        ~CloseWorker() {}

        void Execute();
        void HandleOKCallback();
        void HandleErrorCallback();
};
