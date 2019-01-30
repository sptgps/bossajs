#include "readworker.h"
#include "util.h"

using namespace v8;


void ReadWorker::Execute() {
    try {
        result = bossa->read(offset, size);
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void ReadWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> buffer = Nan::CopyBuffer(result.data(), result.size())
        .ToLocalChecked();

    Local<Value> argv[] = {
        Nan::Null(),  // err
        buffer,  // value
    };

    callback->Call(2, argv, async_resource);
}


void ReadWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
        Nan::Null(),
    };

    callback->Call(2, argv, async_resource);
}
