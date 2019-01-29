#include "verifyworker.h"
#include "util.h"

using namespace v8;


void VerifyWorker::Execute() {
    try {
        if (!bossa->verify(content, offset)) {
            throw std::runtime_error("Failed");
        }
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void VerifyWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Null(),  // err
    };

    callback->Call(1, argv, async_resource);
}


void VerifyWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
    };

    callback->Call(1, argv, async_resource);
}
