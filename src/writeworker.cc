#include "writeworker.h"
#include "util.h"

using namespace v8;


void WriteWorker::Execute() {
    try {
        bossa->write(buffer, offset);
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void WriteWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Null(),  // err
    };

    callback->Call(1, argv, async_resource);
}


void WriteWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
    };

    callback->Call(1, argv, async_resource);
}
