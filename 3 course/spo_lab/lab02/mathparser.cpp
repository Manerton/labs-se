#include "mathparser.h"
#include <memory>
#include <stack>

using ASM_types::LC_Symbol;
using namespace std;

MathParser::priority MathParser::getOperPriority(char ch) noexcept
{
    // унарный минус
    if (ch == -'-')                             return priority::VeryHigh;
    if (ch == '*' || ch == '%' || ch == '/')    return priority::High;
    if (ch == '+' || ch == '-')                 return priority::Mid;
    if (ch == '(' || ch == ')')                 return priority::Low;
    return priority::NOP;
}

MathParser::Tokens MathParser::getTokens(const string_view str)
{
    vector<string> tokens;
    auto it = str.begin();
    string token;
    while (it != str.end())
    {
        if (isalpha(*it) || isUnderLine(*it)) // если метка
        {
            while ((it != str.end()) &&
                   (isalpha(*it) || isdigit(*it) || isUnderLine(*it)))
            {
                token += *it;
                ++it;
            }
            tokens.push_back(token);
            token.clear();
        }
        else if (isdigit(*it))   // если число
        {
            while ((it != str.end()) && (isalpha(*it) || isdigit(*it)))
            {
                token += *it;
                ++it;
            }
            tokens.push_back(token);
            token.clear();
        }
        // если операция или символ счетчика размещения
        else if (getOperPriority(*it) != priority::NOP || (*it) == LC_Symbol[0])
        {
            tokens.push_back({*it});
            ++it;
        }
        else // нераспознанный токен
        {
            throw UndefinedToken();
        }
    }
    return tokens;
}

MathParser::Tokens MathParser::buildRPN(const Tokens &tokens)
{
    Tokens res;
    stack<string> operStack;
    bool may_unary = true;
    for (const auto& token : tokens)
    {
        if (token == "(")
        {
            operStack.push(token);
            may_unary = true;
        }
        else if (token == ")")
        {
            while (!operStack.empty() && operStack.top() != "(")
            {
                // пока верхним элементом стека не станет открывающая скобка,
                // выталкиваем элементы из стека
                res.push_back(operStack.top());
                operStack.pop();
            }
            // если дошли до открывающей скобки
            if (!operStack.empty())
            {
                operStack.pop();
                may_unary = false;
            }
            else throw UnpairedBracket(); // стек кончился, а скобку так и не встретили, ошибка
        }
        // если встретили операцию
        else if (token.size() == 1 && getOperPriority(token[0]) != priority::NOP)
        {
            string oper = token;
            // если унарный минус
            if (may_unary && isUnary(oper[0])) oper[0] = -oper[0];
            while (!operStack.empty()
                   && (getOperPriority(operStack.top()[0]) >= getOperPriority(oper[0])))
            {
                res.push_back(operStack.top());
                operStack.pop();
            }
            operStack.push(oper);
            may_unary = true;
        }
        else // если число
        {
            res.push_back(token);
            may_unary = false;
        }
    }

    while (!operStack.empty())
    {
        res.push_back(operStack.top());
        operStack.pop();
    }

    return res;
}

shared_ptr<MathExpression> MathParser::getOperExpression(char ch, shared_ptr<MathExpression> left, shared_ptr<MathExpression> right)
{
    switch (ch)
    {
        case '+':
            return make_shared<AddExpression>(left,right);
        case '-':
            return make_shared<SubExpression>(left,right);
        case '*':
            return make_shared<MulExpression>(left,right);
        case '/':
            return make_shared<DivExpression>(left,right);
        case '%':
            return make_shared<ModExpression>(left,right);
        default:
            return nullptr;
    }
}

shared_ptr<MathExpression> MathParser::buildExpressionTree(const Tokens &rpnTokens)
{
    stack<std::shared_ptr<MathExpression>> st;

    for (const auto& token: rpnTokens)
    {
        // если операция
        if (token.size() == 1 && getOperPriority(token[0]) != priority::NOP)
        {
            // унарная операция
            if (getOperPriority(token[0]) == priority::VeryHigh)
            {
                if (!st.empty())
                {
                    auto operand = st.top(); st.pop();
                    auto expr = make_shared<UnaryExpression>(operand);
                    st.push(expr);
                } else throw WrongBuildExpTree();
            }
            else // бинарная операция
            {
                if (st.size() >= 2)
                {
                    auto right = st.top(); st.pop();
                    auto left = st.top(); st.pop();
                    auto expr = getOperExpression(token[0],left,right);
                    st.push(expr);
                }
                else throw WrongBuildExpTree();
            }
        }
        else if (isalpha(token[0]) || isUnderLine(token[0]) || token == LC_Symbol) // если метка
        {
            // если метка существует
            if (context.find(token) != context.end())
            {
                st.push(make_shared<NumberExpression>(context[token]));
            }
            else throw UndefinedLabel();
        }
        else if (isdigit(token[0])) // если константа
        {
            try {
                Value val = StrToInt(token);
                st.push(make_shared<NumberExpression>(val));
            }
            catch (std::exception&)
            {
                throw illInteger();
            }
        }
    }
    if (st.size() == 1)
    {
        return st.top();
    }
    throw WrongBuildExpTree();
}

MathParser::Value MathParser::StrToInt(const string &str)
{
    int64_t res = 0;
    if(str[0] == '0')
    {
        // двоичная система
        if(str[1] == 'b' || str[1] == 'B')
        {
            res = stoll(str.substr(2), nullptr, 2);
        }
        else res = stoll(str, nullptr, 0);
    }
    else res = stoll(str, nullptr, 10);

    if ((res < numeric_limits<int32_t>::min()) ||
            (res > numeric_limits<uint32_t>::max())) throw illInteger();
    return static_cast<Value>(res);
}

MathParser::RealValue MathParser::StrToFloat(const string &str, Error &err)
{
    float res = 0;
    try {
        res = stof(str);
    }  catch (std::exception&) {
        err = Error::illFloatValue;
    }
    return res;
}

MathParser::MathParser(MathParser::Context &_context, const string_view str, MathParser::Error &e) : context(_context)
{
    try {
        // выделяем лексемы из строки
        auto tokens = getTokens(str);
        auto rpnTokens = buildRPN(tokens);
        auto expTree = buildExpressionTree(rpnTokens);
        result = expTree->eval(context);
    }
    catch (UndefinedLabel&)
    {
        e = Error::undefLabel;
    }
    catch (illInteger&)
    {
        e = Error::illInteger;
    }
    catch (DivExpression::DivideByZero&)
    {
        e = Error::divideByZero;
    }
    catch (std::exception&)
    {
        e = Error::illExpression;
    }
}


MathParser::Value MathParser::eval() const
{
    return result;
}
