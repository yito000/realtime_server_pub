#ifndef SPIN_LOCK_H
#define SPIN_LOCK_H

#include <cstddef>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include <atomic>

template <std::size_t LOOP_COUNT = 50>
class Spinlock {
private:
    typedef enum {Locked = 1, Unlocked = 0} LockState;
    std::atomic<long> atomic_lock;

public:
    Spinlock() : atomic_lock(Unlocked) {}

    inline void lock()
    {
        int cnt = 0;
        auto du = boost::chrono::milliseconds(0);

        while (atomic_lock.exchange(Locked) == Locked) {
            if (cnt >= LOOP_COUNT) {
                boost::this_thread::sleep_for(du);
            }

            cnt++;
        }
    }

    inline void unlock()
    {
        atomic_lock.store(Unlocked);
    }
};

#endif
