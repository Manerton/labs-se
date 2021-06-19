#ifndef COMPILEREXCEPTION_H
#define COMPILEREXCEPTION_H

#include <stdexcept>

namespace rubyCompiler
{
class CompilerException: public std::exception
{
protected:
    std::string msg{};
public:
    explicit CompilerException(size_t line) : msg(std::to_string(line) + ":") {}
    virtual const char* what() const noexcept override final { return msg.c_str(); }
    virtual ~CompilerException() {}
};

class NameError: public CompilerException
{
public:
    NameError(size_t line, const std::string& name) : CompilerException(line)
    {
        msg += "undefined variable '" + name + "' (NameError)";
    }
    virtual ~NameError() {}
};

class TypeError: public CompilerException
{
public:
    TypeError(size_t line, const std::string &currentType, const std::string &expectedType) : CompilerException(line)
    {
        msg += "no implicit conversion of " + currentType + " into " + expectedType + " (TypeError)";
    }
    virtual ~TypeError() {}
};

class NoMethodError: public CompilerException
{
public:
    NoMethodError(size_t line, const std::string &method, const std::string &type) : CompilerException(line)
    {
        msg += "undefined method '" + method + "' for " + type + " (NoMethodError)";
    }
    virtual ~NoMethodError() {}
};

}

#endif // COMPILEREXCEPTION_H
