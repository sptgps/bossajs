#pragma once

#include "Flasher.h"


class BossaObserver : public FlasherObserver {
    private:

    public:
        BossaObserver() {}
        virtual ~BossaObserver() {}
        
        virtual void onStatus(const char *message, ...);
        virtual void onProgress(int num, int div);
};


