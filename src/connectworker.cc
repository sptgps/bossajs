#include "connectworker.h"
#include "util.h"

using namespace v8;


void ConnectWorker::Execute() {
    try {
        bossa->connect(port);
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void ConnectWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Null(),  // err
    };

    callback->Call(1, argv, async_resource);
}


void ConnectWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
    };

    callback->Call(1, argv, async_resource);
}