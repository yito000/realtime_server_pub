#ifndef STL_CUSTOM_ALLOCATOR_H
#define STL_CUSTOM_ALLOCATOR_H

#include <limits>
#include "custom_allocator.hpp"

template < typename T, typename Allocator = CustomAllocator<> >
class STLCustomAllocator : Allocator
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef Allocator GolbalAllocator;
    
    template <typename U>
    struct rebind
    {
        typedef STLCustomAllocator<U, Allocator> other;
    };
    
    STLCustomAllocator() throw() {}
    STLCustomAllocator(const STLCustomAllocator&) throw() {}
    template <typename U> STLCustomAllocator(const STLCustomAllocator<U, Allocator>&) throw() {}
    ~STLCustomAllocator() throw() {}
    
    pointer allocate(size_type num, STLCustomAllocator< T, Allocator >::const_pointer hint = 0)
    {
        if (num <= 1) {
            return (pointer)(GolbalAllocator::operator new(sizeof(T)));
        } else {
            return (pointer)(GolbalAllocator::operator new[](num * sizeof(T)));
        }
	}
    
    void construct(pointer p, const T& value)
    {
        new(p) T(value);
    }
    
    void deallocate(pointer p, size_type num)
    {
        if (p) {
            if (num <= 1) {
                GolbalAllocator::operator delete((void*)p, sizeof(T));
            } else {
                GolbalAllocator::operator delete[]((void*)p, num * sizeof(T));
            }
        }
    }
    
    void destroy(pointer p)
    {
        p->~T();
    }
    
    pointer address(reference value) { return &value; }
    const_pointer address(const_reference value) const { return &value; }
    
    size_type max_size() const throw()
    {
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }
};

template <typename Allocator >
class STLCustomAllocator<void, Allocator> : Allocator
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef void value_type;
	
    template <typename U>
    struct rebind
    {
        typedef STLCustomAllocator<U, Allocator> other;
    };
};

template <class T1, class T2, class Allocator>
bool operator==(const STLCustomAllocator<T1, Allocator>&, const STLCustomAllocator<T2, Allocator>&) throw()
{
    return true;
}

template <class T1, class T2, class Allocator>
bool operator!=(const STLCustomAllocator<T1, Allocator>&, const STLCustomAllocator<T2, Allocator>&) throw()
{
    return false;
}

#endif

