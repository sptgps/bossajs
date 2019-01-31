#include <iostream>
#include <mutex>

#include "observer.h"
#include "util.h"


// Queue control between the threads
std::mutex queue_mutex;
std::queue<BossaEvent> queue;


void
BossaObserver::onAsyncHandler(uv_async_t* handle) {
    std::scoped_lock lock(queue_mutex);

    while (!queue.empty()) {
        Nan::HandleScope scope;
        Nan::AsyncResource async_resource("bossa::observer");
        auto elem = queue.front();

        Local<Object> bossa = elem.emitter->handle();
        Local<Value> argv[] = {
            L(elem.event),
            Nan::New(elem.value),
            Nan::New(elem.total),
        };

        async_resource.runInAsyncScope(bossa, "emit", 3, argv);
        queue.pop();
    }
}


void
BossaObserver::emit(std::string event, int value, int total) {
    std::scoped_lock lock(queue_mutex);

    queue.push(BossaEvent(emitter, event, value, total));
    uv_async_send(&async);
}


void
BossaObserver::onStatus(const char* message, ...) {
    // Pass
}


void
BossaObserver::onProgress(int num, int div) {

    emit("progress", num, div);
}