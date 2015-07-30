#include "base_exception.h"

#include "log/logger.h"

BaseException::BaseException(const char* _message, 
    const char* _filename, const char* _function, int _line) : 
    message(_message), filename(_filename), 
    function(_function), line(_line)
{
    //
}

BaseException::~BaseException() throw()
{
    //
}

const char* BaseException::getFilename()
{
    return filename;
}

const char* BaseException::getFunction()
{
    return function;
}

int BaseException::getLine()
{
    return line;
}

const char* BaseException::what() const throw()
{
    return message != NULL ? message : "";
}

void BaseException::printAll()
{
    Logger::log("filename: %s", getFilename());
    Logger::log("function: %s", getFunction());
    Logger::log("line:     %d", getLine());
    Logger::log("message:  %s", what());
}
