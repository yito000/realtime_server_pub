#ifndef ATOMIC_OPERATOR_H
#define ATOMIC_OPERATOR_H

#include <cstdint>

template <typename T>
class AtomicOperator
{
public:
    static inline T add(T* ptr, T value)
    {
        return __sync_add_and_fetch(ptr, value);
    }

    static inline T sub(T* ptr, T value)
    {
        return __sync_add_and_fetch(ptr, -value);
    }

    static inline T increment(T* ptr)
    {
        return __sync_add_and_fetch(ptr, 1);
    }

    static inline T decrement(T* ptr)
    {
        return __sync_add_and_fetch(ptr, -1);
    }

    static inline bool compare_and_swap(T* ptr, T oldval, T newval)
    {
        return __sync_bool_compare_and_swap(ptr, oldval, newval);
    }

    static inline T lock_test_and_set(T* ptr, T value)
    {
        return __sync_lock_test_and_set(ptr, value);
    }

    static inline void lock_release(T* ptr)
    {
        __sync_lock_release(ptr);
    }
    
    static inline void load()
    {
        __sync_synchronize();
    }
    
    static inline void store()
    {
        __sync_synchronize();
    }
};

#endif
