#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "smart_ptr.hpp"

class FileStream;

class DataBuffer : public SmartPtr<DataBuffer>
{
    friend class FileStream;
    
public:
    typedef boost::intrusive_ptr<DataBuffer> ptr;
    
    static DataBuffer::ptr create(unsigned char* buf, size_t s, bool dup = true)
    {
        if (buf && dup) {
            unsigned char* tmp_b = new unsigned char[s];
            memcpy(tmp_b, buf, s);
            
            buf = tmp_b;
        }
        
        auto ret = new DataBuffer(buf, s);
        
        return ret;
    }
    
    ~DataBuffer()
    {
        if (buf) {
            delete[] buf;
        }
    }
    
    void assign(long start, size_t size, const void* buffer)
    {
        memcpy(buf + start, buffer, size);
    }
    
    unsigned char* getBuffer() const
    {
        return buf;
    }
    
    size_t getSize() const
    {
        return size;
    }
    
    unsigned char* moveBuffer(size_t* s)
    {
        auto ret = buf;
        buf = nullptr;
        
        *s = size;
        size = 0;
        
        return ret;
    }
    
private:
    DataBuffer(unsigned char* b, size_t s)
    {
        if (b) {
            buf = b;
        } else {
            buf = new unsigned char[s];
            memset(buf, 0, s);
        }
        
        size = s;
    }
    
    unsigned char* buf;
    size_t size;
};

#endif
