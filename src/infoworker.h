#pragma once

#include <nan.h>

#include "bossajs.h"

using namespace v8;


class InfoWorker : public Nan::AsyncWorker {
    private:
        Bossa* bossa;
        FlasherInfo finfo;

    public:
        InfoWorker(Nan::Callback* callback,
                   Bossa* bossa) :
                   Nan::AsyncWorker(callback),
                   bossa(bossa)
                   {}
        ~InfoWorker() {}

        void Execute();
        void HandleOKCallback();
};