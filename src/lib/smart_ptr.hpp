#ifndef SMART_PTR_HPP
#define SMART_PTR_HPP

#include <boost/intrusive_ptr.hpp>
#include <boost/atomic.hpp>

template <typename T>
class SmartPtr
{
public:
    SmartPtr() : ref_count(0) {}
    SmartPtr(const SmartPtr& p) : ref_count(p.ref_count.load()) {}
    virtual ~SmartPtr() {}

    friend void intrusive_ptr_add_ref(const T* t)
    {
        t->ref_count.fetch_add(1, boost::memory_order_relaxed);
    }

    friend void intrusive_ptr_release(const T* t)
    {
        if (t->ref_count.fetch_sub(1, boost::memory_order_release) == 1) {
            boost::atomic_thread_fence(boost::memory_order_acquire);
            delete t;
        }
    }

private:
    mutable boost::atomic<int> ref_count;
};

#endif
