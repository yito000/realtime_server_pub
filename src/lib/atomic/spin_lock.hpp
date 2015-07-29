#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

#include <cstddef>
#include <boost/thread.hpp>

#include "atomic_operator.hpp"

template <std::size_t LOOP_COUNT = 50>
class Spinlock {
private:
    typedef enum {Locked = 1, Unlocked = 0} LockState;
    long atomic_lock;

public:
    Spinlock() : atomic_lock(Unlocked) {}

    inline void lock()
    {
        int cnt = 0;
        auto du = boost::chrono::milliseconds(1);

        while (Atomic::lock_test_and_set(&atomic_lock, Locked) == Locked) {
            if (cnt >= LOOP_COUNT) {
                boost::this_thread::sleep_for(du);
            }

            cnt++;
        }
    }

    inline void unlock()
    {
        Atomic::lock_release(&atomic_lock);
    }
};

#endif
