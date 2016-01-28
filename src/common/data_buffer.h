#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include "fw_env.h"

BEGIN_NS

class FileStream;

class DataBuffer : public SmartPtr<DataBuffer>
{
    friend class FileStream;
    
public:
    typedef boost::intrusive_ptr<DataBuffer> ptr;
    
    enum class Flag {
        COPY,
        REFERENCE,
        OWNERSHIP
    };
    
    static DataBuffer::ptr create(unsigned char* buf, size_t s, Flag flag = Flag::OWNERSHIP)
    {
        switch (flag) {
            case Flag::COPY: {
                if (buf && s > 0) {
                    unsigned char* tmp_b = new unsigned char[s];
                    memcpy(tmp_b, buf, s);
                    
                    buf = tmp_b;
                }
                break;
            }
            
            default:
                break;
        }
        
        auto ret = new DataBuffer(buf, s);
        if (flag != Flag::REFERENCE) {
            ret->has_buffer = true;
        }
        
        return ret;
    }
    
    ~DataBuffer()
    {
        if (buf && has_buffer) {
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
    DataBuffer(unsigned char* b, size_t s) :
        buf(nullptr),
        size(0),
        has_buffer(false)
    {
        if (b) {
            buf = b;
        } else if (s > 0) {
            buf = new unsigned char[s];
            memset(buf, 0, s);
        }
        
        size = s;
    }
    
    unsigned char* buf;
    size_t size;
    bool has_buffer;
};

END_NS

#endif
