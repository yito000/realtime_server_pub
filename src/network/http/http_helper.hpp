#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include "network/io/socket_buffer.h"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#include "archive/gzip.h"
#include "http_header_parser.h"

#include "log/logger.h"

BEGIN_NS

template <typename Socket>
class HttpHelper
{
public:
    typedef std::map<std::string, std::string> HttpHeaders;
    
    typedef boost::function<void(const HttpResponseHeader&, const ByteBuffer&)> ResponseCallback;
    typedef boost::function<void(const HttpResponseHeader&)> FinishResponseCallback;
    
private:
    void setHeaderParams(HttpHeaders& headers, std::string& request)
    {
        HttpHeaders::iterator it = headers.begin();
        
        for (; it != headers.end(); ++it) {
            request += it->first + ": " + it->second + "\r\n";
        }
    }
    
public:
    HttpHelper()
    {
        read_size = 0;
        header_size  = 0;
    }
    
    HttpResponseHeader::ptr sendGetRequest(Socket& socket, HttpRequestParams::ptr req_params)
    {
        ResponseCallback& response_callback = req_params->response_callback;
        FinishResponseCallback& finish_response_callback = req_params->finish_response_callback;
        HttpHeaders& headers = req_params->user_headers;
        const std::string& hostname = req_params->hostname;
        const std::string& path = req_params->path;
        boost::posix_time::time_duration timeout = boost::posix_time::seconds(req_params->request_timeout);
        
        std::string request;
        
        request = "GET " + path + " HTTP/1.1\r\n";
        request += "Host: " + hostname + "\r\n";
        request += "Accept: */*\r\n";
        
        setHeaderParams(headers, request);
        
        request += "\r\n";
        
#ifndef NDEBUG
        std::cout << "request:\n" << request << std::endl;
#endif
        
        socket.write(request, timeout);
        
        return receiveResponse(response_callback, finish_response_callback, socket, timeout);
    }
    
    HttpResponseHeader::ptr sendPostRequestAsJson(const std::string& json_str, 
        Socket& socket, HttpRequestParams::ptr req_params)
    {
        ResponseCallback& response_callback = req_params->response_callback;
        FinishResponseCallback& finish_response_callback = req_params->finish_response_callback;
        HttpHeaders& headers = req_params->user_headers;
        const std::string& hostname = req_params->hostname;
        const std::string& path = req_params->path;
        boost::posix_time::time_duration timeout = boost::posix_time::seconds(req_params->request_timeout);
        
        std::string request;
        
        request = "POST " + path + " HTTP/1.1\r\n";
        request += "Host: " + hostname + "\r\n";
        request += "Accept: application/json\r\n";
        request += "Content-Type: application/json\r\n";
        request += "Content-Length: " + boost::lexical_cast<std::string>(json_str.size()) + "\r\n";
        
        setHeaderParams(headers, request);
        
        request += "\r\n";
        request += json_str + "\r\n";
        
#ifndef NDEBUG
        std::cout << "request:\n" << request << std::endl;
#endif
        
        socket.write(request, timeout);
        
        return receiveResponse(response_callback, finish_response_callback, socket, timeout);
    }
    
private:
    HttpResponseHeader::ptr receiveResponse(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, boost::posix_time::time_duration timeout)
    {
        ByteBuffer buffer;
        
        bool can_read_buffer = true;
        
        buffer.clear();
        
        HttpResponseHeader::ptr header = new HttpResponseHeader;
        readResponseHeader(socket, buffer, *header, can_read_buffer, timeout);
        
        if (header->status_code != 200) {
            Logger::debug("status code: %d", header->status_code);
            return header;
        }
        
        if (header->encoding_chunked) {
            return responseTransferEncodingChunked(response_callback, finish_response_callback, socket, 
                buffer, timeout, header, can_read_buffer);
        } else {
            return responseTransferEncodingNormal(response_callback, finish_response_callback, socket, 
                buffer, timeout, header, can_read_buffer);
        }
    }
    
    // chunked
    HttpResponseHeader::ptr responseTransferEncodingChunked(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        HttpResponseHeader::ptr header, bool& can_read_buffer)
    {
        if (header->content_encoding == HttpResponseHeader::TYPE_GZIP) {
            return responseChunkedGzip(response_callback, finish_response_callback, socket, 
                buffer, timeout, header, can_read_buffer);
        } else {
            return responseChunkedNormal(response_callback, finish_response_callback, socket, 
                buffer, timeout, header, can_read_buffer);
        }
    }
    
    HttpResponseHeader::ptr responseChunkedNormal(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        HttpResponseHeader::ptr header, bool& can_read_buffer)
    {
        // read body
        if (buffer.size() - header_size > 0) {
            ByteBuffer tmp_buffer;
            size_t start_pos = header_size;
            tmp_buffer.reserve(buffer.size() - header_size);
            
            for (int i = start_pos; i < buffer.size(); i++) {
                tmp_buffer.push_back(buffer[i]);
            }
            
            buffer.swap(tmp_buffer);
        } else {
            buffer.clear();
        }
        
        do {
            ByteBuffer out_buffer;
            bool b = streamChunkData(socket, timeout, buffer, out_buffer);
            
            if (!response_callback.empty() && out_buffer.size() > 0) {
                response_callback(*header, out_buffer);
            }
            
            if (!b) {
                break;
            }
            
            if (out_buffer.size() > 0) {
                read_size += out_buffer.size();
            }
        } while(1);
        
#ifndef NDEBUG
        std::cout << std::endl;
#endif
        
        Logger::debug("header size: %ld", header_size);
        Logger::debug("content length: %ld", header->content_length);
        Logger::debug("read size: %ld", read_size);
        
        if (!finish_response_callback.empty()) {
            finish_response_callback(*header);
        }
        
        buffer.clear();
        
        return header;
    }
    
    HttpResponseHeader::ptr responseChunkedGzip(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        HttpResponseHeader::ptr header, bool& can_read_buffer)
    {
        ByteBuffer ret_buffer;
        
        // read body
        if (buffer.size() - header_size > 0) {
            ByteBuffer tmp_buffer;
            size_t start_pos = header_size;
            tmp_buffer.reserve(buffer.size() - header_size);
            
            for (int i = start_pos; i < buffer.size(); i++) {
                tmp_buffer.push_back(buffer[i]);
            }
            
            buffer.swap(tmp_buffer);
        } else {
            buffer.clear();
        }
        
        do {
            ByteBuffer out_buffer;
            
            bool b = streamChunkData(socket, timeout, buffer, out_buffer);
            ret_buffer.reserve(ret_buffer.size() + out_buffer.size());
            
            for (int i = 0; i < out_buffer.size(); i++) {
                ret_buffer.push_back(out_buffer[i]);
            }
            
            if (!b) {
                break;
            }
            
            if (out_buffer.size() > 0) {
                read_size += out_buffer.size();
            }
        } while(1);
        
        if (!response_callback.empty()) {
            GzipByteBuffer decompressed_buffer;
            ByteBuffer decompressed_copy_buffer;
            
            Gzip::decompressBufToBuf((const char*)ret_buffer.data(), ret_buffer.size(), decompressed_buffer);
            
            decompressed_copy_buffer.reserve(decompressed_buffer.size());
            
            for (int i = 0; i < decompressed_buffer.size(); i++) {
                decompressed_copy_buffer.push_back(decompressed_buffer[i]);
            }
            
            response_callback(*header, decompressed_copy_buffer);
        }
        
#ifndef NDEBUG
        std::cout << std::endl;
#endif
        
        Logger::debug("header size: %ld", header_size);
        Logger::debug("content length: %ld", header->content_length);
        Logger::debug("read size: %ld", read_size);
        
        if (!finish_response_callback.empty()) {
            finish_response_callback(*header);
        }
        
        buffer.clear();
        
        return header;
    }
    
    bool streamChunkData(Socket& socket, boost::posix_time::time_duration timeout, 
        ByteBuffer& tmp_buffer, ByteBuffer& out_buffer)
    {
        size_t chunk_size = -1;
        int size_end_index = -1;
        
        while (1) {
            if (chunk_size == -1) {
                chunk_size = getChunkSize(tmp_buffer, size_end_index);
                
                Logger::debug("chunk size: %ld, data size: %ld", chunk_size, tmp_buffer.size());
                if (chunk_size == 0) {
                    return false;
                }
            }
            
            if (chunk_size > 0) {
                int data_index = 0;
                if (size_end_index >= 0) {
                    data_index = size_end_index;
                }
                
                int after_index = 0;
                for (int i = data_index; i < tmp_buffer.size(); i++) {
                    const int CRLF_LEN = 2;
                    out_buffer.push_back(tmp_buffer[i]);
                    
                    if (out_buffer.size() >= chunk_size) {
                        after_index = i + (1 + CRLF_LEN);
                        break;
                    }
                }
                
                if (out_buffer.size() >= chunk_size) {
                    ByteBuffer copy_buffer;
                    for (int i = after_index; i < tmp_buffer.size(); i++) {
                        copy_buffer.push_back(tmp_buffer[i]);
                    }
                    
                    tmp_buffer.clear();
                    tmp_buffer.reserve(copy_buffer.size());
                    tmp_buffer.assign(copy_buffer.begin(), copy_buffer.end());
                    Logger::debug("return: %ld, %ld", out_buffer.size(), chunk_size);
                    
                    return true;
                } else {
                    out_buffer.clear();
                }
            }
            
            try {
                ByteBuffer tmp_buffer2;
                socket.read_some(tmp_buffer2, timeout);
                
                for (int i = 0; i < tmp_buffer2.size(); i++) {
                    tmp_buffer.push_back(tmp_buffer2[i]);
                }
            } catch (std::exception& e) {
                throw e;
            }
        }
    }
    
    size_t getChunkSize(ByteBuffer& buffer, int& find_index)
    {
        size_t ret = -1;
        bool found = findNewLine(buffer, find_index);
        
        if (!found) {
            return ret;
        }
        
        int s_index = 0;
        char* str = new char[find_index + 1];
        for (int i = 0; i < find_index; i++) {
            str[s_index] = buffer[i];
            
            s_index++;
        }
        str[s_index] = '\0';
        
        if (s_index == 0) {
            delete[] str;
            return ret;
        }
        
        char* pos = &str[0];
        std::string size_str(pos);
        
        delete[] str;
        
        if (size_str == "") {
            return ret;
        }
        
        std::stringstream in;
        
        in << std::hex << size_str;
        in >> ret;
        
        return ret;
    }
    
    bool findNewLine(ByteBuffer& buffer, int& index)
    {
        for (int i = 0; i < buffer.size(); i++) {
            if (buffer[i] == '\r') {
                if (i > 0 && buffer[i + 1] == '\n') {
                    index = i + 2;
                    return true;
                }
            }
        }
        
        return false;
    }
    
    // normal
    HttpResponseHeader::ptr responseTransferEncodingNormal(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        HttpResponseHeader::ptr header, bool& can_read_buffer)
    {
        if (header->content_encoding == HttpResponseHeader::TYPE_GZIP) {
            return responseGzip(response_callback, finish_response_callback, socket, 
                buffer, timeout, header, can_read_buffer);
        } else {
            return responseNormal(response_callback, finish_response_callback, socket, 
                buffer, timeout, header, can_read_buffer);
        }
    }
    
    HttpResponseHeader::ptr responseNormal(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        HttpResponseHeader::ptr header, bool& can_read_buffer)
    {
        // read body
        if (buffer.size() - header_size > 0) {
            ByteBuffer tmp_buffer;
            size_t start_pos = header_size;
            tmp_buffer.reserve(buffer.size() - header_size);
            
            for (int i = start_pos; i < buffer.size(); i++) {
                tmp_buffer.push_back(buffer[i]);
            }
            
            buffer.swap(tmp_buffer);
            
            if (!response_callback.empty()) {
                response_callback(*header, buffer);
            }
        } else {
            buffer.clear();
        }
        
        while (can_read_buffer) {
            if (header->content_length > 0 && header->content_length <= read_size - header_size) {
                break;
            }
            
            try {
                buffer.clear();
                can_read_buffer = socket.read_some(buffer, timeout);
            } catch (std::exception& e) {
                throw e;
            }
            
            if (buffer.size() > 0) {
                read_size += buffer.size();
                
                if (!response_callback.empty()) {
#ifndef NDEBUG
                    std::string buffer_str;
                    buffer_str.append(buffer.begin(), buffer.end());
                    
                    std::cout << buffer_str;
#endif
                    response_callback(*header, buffer);
                }
            } else {
                can_read_buffer = false;
            }
        }
        
#ifndef NDEBUG
        std::cout << std::endl;
#endif
        
        Logger::debug("header size: %ld", header_size);
        Logger::debug("content length: %ld", header->content_length);
        Logger::debug("read size: %ld", read_size);
        
        if (!finish_response_callback.empty()) {
            finish_response_callback(*header);
        }
        
        buffer.clear();
        
        return header;
    }
    
    HttpResponseHeader::ptr responseGzip(ResponseCallback& response_callback, 
        FinishResponseCallback& finish_response_callback, 
        Socket& socket, ByteBuffer& buffer, boost::posix_time::time_duration timeout, 
        HttpResponseHeader::ptr header, bool& can_read_buffer)
    {
        // read body
        if (buffer.size() - header_size > 0) {
            ByteBuffer tmp_buffer;
            size_t start_pos = header_size;
            tmp_buffer.reserve(buffer.size() - header_size);
            
            for (int i = start_pos; i < buffer.size(); i++) {
                tmp_buffer.push_back(buffer[i]);
            }
            
            buffer.swap(tmp_buffer);
        } else {
            buffer.clear();
        }
        
        while (can_read_buffer) {
            if (header->content_length > 0 && header->content_length <= read_size - header_size) {
                break;
            }
            
            ByteBuffer tmp_buffer;
            
            try {
                can_read_buffer = socket.read_some(tmp_buffer, timeout);
            } catch (std::exception& e) {
                throw e;
            }
            
            if (buffer.size() > 0) {
                read_size += tmp_buffer.size();
                
                for (int i = 0; i < tmp_buffer.size(); i++) {
                    buffer.push_back(tmp_buffer[i]);
                }
            } else {
                can_read_buffer = false;
            }
        }
        
        if (!response_callback.empty()) {
            GzipByteBuffer decompressed_buffer;
            ByteBuffer decompressed_copy_buffer;
            
            Gzip::decompressBufToBuf((const char*)buffer.data(), buffer.size(), decompressed_buffer);
            
            decompressed_copy_buffer.reserve(decompressed_buffer.size());
            
            for (int i = 0; i < decompressed_buffer.size(); i++) {
                decompressed_copy_buffer.push_back(decompressed_buffer[i]);
            }
            
            response_callback(*header, decompressed_copy_buffer);
        }
        
#ifndef NDEBUG
        std::cout << std::endl;
#endif
        
        Logger::debug("header size: %ld", header_size);
        Logger::debug("content length: %ld", header->content_length);
        Logger::debug("read size: %ld", read_size);
        
        if (!finish_response_callback.empty()) {
            finish_response_callback(*header);
        }
        
        buffer.clear();
        
        return header;
    }
    
    void readResponseHeader(Socket& socket, ByteBuffer& buffer, HttpResponseHeader& header, 
        bool& can_read_buffer, boost::posix_time::time_duration timeout)
    {
        ByteBuffer tmp_buf;
        
        HttpHeaderParser parser;
        char* header_end_pos = NULL;
        
        std::string buffer_str;
        Logger::debug("read header");
        
        while (header_end_pos == NULL) {
            tmp_buf.clear();
            
            can_read_buffer = socket.read_some(tmp_buf, timeout);
            
            if (tmp_buf.size() <= 0) {
                break;
            }
            
            buffer_str.reserve(tmp_buf.size());
            buffer_str.append(tmp_buf.begin(), tmp_buf.end());
            
            for (int i = 0; i < tmp_buf.size(); i++) {
                buffer.push_back(tmp_buf[i]);
            }
            
#ifndef NDEBUG
            std::cout << "buffer string: " << std::endl << buffer_str << std::endl;
#endif
            
            header_end_pos = (char*)strstr(buffer_str.c_str(), "\r\n\r\n");
        }
        
        size_t len = header_end_pos - buffer_str.c_str();
        std::string header_str(buffer_str.c_str(), len);
        
        header_size = header_str.size() + 4;
        read_size = buffer.size();
        
        parser.getHeader(header_str, header);
    }
    
    size_t read_size;
    size_t header_size;
};

END_NS

#endif
