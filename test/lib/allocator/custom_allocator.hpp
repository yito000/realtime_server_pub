#ifndef CUSTOM_ALLOCATOR_H
#define CUSTOM_ALLOCATOR_H

#include <stddef.h>
#include <cstdlib>

#ifdef USE_JEMALLOC
#include <jemalloc/jemalloc.h>

#define MALLOC je_malloc
#define FREE je_free
#else
#define MALLOC malloc
#define FREE free
#endif

#include "log/logger.h"

struct MyTag {};

template <typename T = MyTag>
class CustomAllocator
{
public:
    template <typename U>
    static inline void* alloc()
    {
        return MALLOC(sizeof(U));
    }
    
    template <typename U>
    static inline void* alloc(size_t n)
    {
        return MALLOC(sizeof(U) * n);
    }
    
    static inline void* operator new(size_t size) throw()
    {
        return MALLOC(size);
    }
	
    static inline void* operator new(size_t size, void* buf) throw()
    {
        return buf;
    }
    
    static inline void* operator new[](size_t size) throw()
    {
        return MALLOC(size);
    }
    
    template <typename U>
    static inline void free_mem(void* p)
    {
        FREE(p);
    }
    
    template <typename U>
    static inline void free_mem(void* p, size_t n)
    {
        FREE(p);
    }
    
    static inline void operator delete(void* raw_memory, size_t size) throw()
    {
        FREE(raw_memory);
    }
	
    static inline void operator delete(void* p, void* buf) throw()
    {
        //
    }
    
    static inline void operator delete[](void* raw_memory, size_t size) throw()
    {
        FREE(raw_memory);
    }
};

#endif
