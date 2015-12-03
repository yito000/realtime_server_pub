#ifndef SMART_PTR_HPP
#define SMART_PTR_HPP

#include <boost/intrusive_ptr.hpp>
#include <boost/atomic.hpp>

template <typename T>
class SmartPtr
{
public:
    SmartPtr() : ref_count(0) {}
    virtual ~SmartPtr() {}
    
    int getRefCount() const
    {
        return ref_count.load();
    }

private:
    mutable boost::atomic<int> ref_count;

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
};

#endif
