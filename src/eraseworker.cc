#include "eraseworker.h"
#include "util.h"

using namespace v8;


void EraseWorker::Execute() {
    try {
        bossa->erase(offset);
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void EraseWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Null(),  // err
    };

    callback->Call(1, argv, async_resource);
}


void EraseWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
    };

    callback->Call(1, argv, async_resource);
}

