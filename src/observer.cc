#include "observer.h"


void BossaObserver::onStatus(const char* message, ...) {
    va_list ap;
    
    va_start(ap, message);
    vprintf(message, ap);
    va_end(ap);
}

void BossaObserver::onProgress(int num, int div) {
}