#ifndef LOCK_H
#define LOCK_H

#include <atomic>

class Lock
{
    private:
        std::atomic_flag held = ATOMIC_FLAG_INIT;
    public:
        void acquire();
        void release();
};

#endif