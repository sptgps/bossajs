#include "closeworker.h"
#include "util.h"

using namespace v8;


void CloseWorker::Execute() {
    try {
        bossa->close();
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void CloseWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Null(),  // err
    };

    callback->Call(1, argv, async_resource);
}


void CloseWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
    };

    callback->Call(1, argv, async_resource);
}