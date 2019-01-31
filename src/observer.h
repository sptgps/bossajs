#pragma once

#include <nan.h>

#include "Flasher.h"

using namespace v8;


class BossaEvent {
    public:
        Nan::ObjectWrap* emitter;
        std::string event;
        int value;
        int total;
        
        BossaEvent(Nan::ObjectWrap* emitter,
                   std::string event,
                   int value,
                   int total)
                 : emitter(emitter),
                   event(event),
                   value(value),
                   total(total)
        {}
        ~BossaEvent() {}
};


class BossaObserver : public FlasherObserver {
    private:
        // Target for emitting 
        Nan::ObjectWrap* emitter;

        uv_async_t async;

        void emit(std::string event, int value, int total);
        static void onAsyncHandler(uv_async_t* handle);

    public:
        BossaObserver(Nan::ObjectWrap* emitter)
                    : emitter(emitter)
        {
            uv_async_init(uv_default_loop(), &async, onAsyncHandler);
        }
        ~BossaObserver() {
            uv_close(reinterpret_cast<uv_handle_t*>(&async), NULL);
        }
        
        void onStatus(const char *message, ...);
        void onProgress(int num, int div);
};