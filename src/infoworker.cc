#include "infoworker.h"
#include "util.h"

using namespace v8;


void InfoWorker::Execute() {
    try {
        bossa->info(finfo);
    } catch(const std::exception& exc) {
        SetErrorMessage(exc.what());
    }
}


void InfoWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    Local<Object> obj = Nan::New<Object>();
    obj->Set(L("name"), L(finfo.name));
    obj->Set(L("chipId"), Nan::New(finfo.chipId));
    obj->Set(L("extChipId"), Nan::New(finfo.extChipId));
    obj->Set(L("version"), L(finfo.version));
    obj->Set(L("address"), Nan::New(finfo.address));
    obj->Set(L("numPages"), Nan::New(finfo.numPages));
    obj->Set(L("pageSize"), Nan::New(finfo.pageSize));
    obj->Set(L("totalSize"), Nan::New(finfo.totalSize));
    obj->Set(L("numPlanes"), Nan::New(finfo.numPlanes));

    obj->Set(L("security"), Nan::New(finfo.security));
    obj->Set(L("bootFlash"), Nan::New(finfo.bootFlash));
    obj->Set(L("bod"), Nan::New(finfo.bod));
    obj->Set(L("bor"), Nan::New(finfo.bor));

    obj->Set(L("canBootFlash"), Nan::New(finfo.canBootFlash));
    obj->Set(L("canBod"), Nan::New(finfo.canBod));
    obj->Set(L("canBor"), Nan::New(finfo.canBor));
    obj->Set(L("canChipErase"), Nan::New(finfo.canChipErase));
    obj->Set(L("canWriteBuffer"), Nan::New(finfo.canWriteBuffer));
    obj->Set(L("canChecksumBuffer"), Nan::New(finfo.canChecksumBuffer));

    Local<Array> lockRegions = Nan::New<Array>(finfo.lockRegions.size());
    unsigned index = 0;
    for (auto elem : finfo.lockRegions) {
        Nan::Set(lockRegions, index, Nan::New(elem));
        index++;
    }
    obj->Set(L("lockRegions"), lockRegions);

    Local<Value> argv[] = {
        Nan::Null(),  // err
        obj,  // value
    };

    callback->Call(2, argv, async_resource);
}


void InfoWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    Local<Value> argv[] = {
        Nan::Error(this->ErrorMessage()),  // err
        Nan::Null(),
    };

    callback->Call(2, argv, async_resource);
}