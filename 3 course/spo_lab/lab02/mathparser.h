#ifndef MATHPARSER_H
#define MATHPARSER_H
#include "types.h"
#include "mathexpression.h"
#include <string_view>
#include <vector>
#include <memory>

class MathParser
{
    using Context = MathExpression::Context;
    using Value = MathExpression::Value;
    using RealValue = decltype(VM_types::data_t::f);
    using Error = ASM_types::Error;
    using Tokens = std::vector<std::string>;
    // ссылка на таблицу меток
    Context &context;
    // результат вычислений
    Value result = 0;

    enum priority: uint8_t
    {
        NOP = 0,    // не операция
        Low,
        Mid,
        High,
        VeryHigh
    };

// методы
    inline bool isUnderLine(char ch) { return (ch == '_'); }
    inline bool isUnary(char ch) { return (ch == '-'); }
    priority getOperPriority(char ch) noexcept;
    // выделение лексемы (токена) из выражения
    Tokens getTokens(const std::string_view str);
    // обратная польская запись
    Tokens buildRPN(const Tokens &tokens);
    // строим дерево выражений (используем класс MathExpression)
    std::shared_ptr<MathExpression> buildExpressionTree(const Tokens& rpnTokens);
    std::shared_ptr<MathExpression> getOperExpression(char ch, std::shared_ptr<MathExpression> left, std::shared_ptr<MathExpression> right);
public:
    MathParser(Context &_context, const std::string_view str, Error &e);
    // вычислить
    Value eval() const;
    // парсер строки в число
    static Value StrToInt(const std::string &str);
    static RealValue StrToFloat(const std::string &str, Error &err);
    // исключения
    class UndefinedToken : public std::exception
    {
    public:
        const char *what() const noexcept {
            return "Undefined token";
        }
    };
    class UnpairedBracket : public std::exception
    {
    public:
        const char *what() const noexcept {
            return "Unpaired bracket";
        }
    };
    class UndefinedLabel : public std::exception
    {
    public:
        const char *what() const noexcept {
            return "Undefined label";
        }
    };
    class WrongBuildExpTree : public std::exception
    {
    public:
        const char *what() const noexcept {
            return "Wrong building expression tree";
        }
    };
};

#endif // MATHPARSER_H
