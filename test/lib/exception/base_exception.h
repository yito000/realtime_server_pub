#ifndef BASE_EXCEPTION_H
#define BASE_EXCEPTION_H

#include <exception>

class BaseException;

#define BASE_EXCEPTION \
    throw BaseException("", __FILE__, __func__, __LINE__)

#define THROW_EXCEPTION(EXCEPTION_CLASS, message) \
    throw EXCEPTION_CLASS(message, __FILE__, __func__, __LINE__)

class BaseException : public std::exception
{
public:
    BaseException(const char* _message, const char* _filename, 
        const char* _function, int _line);
    virtual ~BaseException() throw();

    const char* getFilename();
    const char* getFunction();
    int getLine();
    virtual const char* what() const throw();

    virtual void printAll();

private:
    const char* message;
    const char* filename;
    const char* function;
    int line;
};

#endif

