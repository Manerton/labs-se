#ifndef COMPILEREXCEPTION_H
#define COMPILEREXCEPTION_H

#include <stdexcept>

namespace rubyCompiler
{
// ошибки самого компилятора (в реализации компилятора)
class NotImplementedException: public std::logic_error
{
public:
    enum class error : uint8_t
    {
        exprIsNotValue,
        unknownArifOp
    };

    std::string getMsg(error err) const
    {
        std::string str = "[Not implemented] ";
        switch (err)
        {
        case error::exprIsNotValue:
            str += "Type of expr is not Value in ";
            break;
        case error::unknownArifOp:
            str += "Unknown ArifOp in ";
            break;
        }
        return str;
    }
    NotImplementedException(const std::string& method_name, error err)
        : std::logic_error(getMsg(err) + method_name)
    {}

    virtual ~NotImplementedException() {}
};

// ошибки, возникающие при компиляции (ошибки в коде программы пользователя)
class CompilerException: public std::exception
{
protected:
    std::string msg{};
public:
    explicit CompilerException(size_t line) : msg(std::to_string(line) + ":") {}
    virtual const char* what() const noexcept override final { return msg.c_str(); }
    virtual ~CompilerException() {}
};

// неизвестная переменная
class NameError: public CompilerException
{
public:
    NameError(size_t line, const std::string& name) : CompilerException(line)
    {
        msg += "undefined variable '" + name + "' (NameError)";
    }
    virtual ~NameError() {}
};

// ошибка приведения типов
class TypeError: public CompilerException
{
public:
    TypeError(size_t line, const std::string &currentType, const std::string &expectedType) : CompilerException(line)
    {
        msg += "no implicit conversion of " + currentType + " into " + expectedType + " (TypeError)";
    }
    virtual ~TypeError() {}
};

// неизвестная операция или метод для данного типа
class NoMethodError: public CompilerException
{
public:
    NoMethodError(size_t line, const std::string &method, const std::string &type) : CompilerException(line)
    {
        msg += "undefined method '" + method + "' for " + type + " (NoMethodError)";
    }
    virtual ~NoMethodError() {}
};

// переполнение стека ВМ
class StackOverflowError: public CompilerException
{
public:
    StackOverflowError(size_t line) : CompilerException(line)
    {
        msg += " stack overflow (nesting > 32) (StackOverflowError)";
    }
    virtual ~StackOverflowError() {}
};

}

#endif // COMPILEREXCEPTION_H
