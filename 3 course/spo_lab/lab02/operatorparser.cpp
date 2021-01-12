#include "operatorparser.h"
#include "translator.h"
#include <vector>

using ASM_types::Operator;
using ASM_types::Error;
using std::string_view;
using std::string;

OperatorParser::SymbolType OperatorParser::getSymbolType(char c) const
{
    if (std::isspace(c))    return SymbolType::Blank;
    if (std::isdigit(c))    return SymbolType::Digit;
    if (std::isalpha(c))    return SymbolType::Id;
    if (c == ':')           return SymbolType::Colon;
    if (c == ';')           return SymbolType::Comment;
    if (c == '_')           return SymbolType::Underline;
    return SymbolType::Other;
}

// переход без действия
OperatorParser::State OperatorParser::ToStart(string_view, size_t, Operator &, string &) noexcept
{
    return State::Start;
}
OperatorParser::State OperatorParser::ToWaitOper(string_view, size_t, Operator&, string&) noexcept
{
    return State::WaitOper;
}
OperatorParser::State OperatorParser::BlankToArg(string_view, size_t, Operator&, string&) noexcept
{
    return State::Arg;
}
// ошибки
OperatorParser::State OperatorParser::ErrToEnd(string_view str, size_t i, Operator& oper, string&) noexcept
{
    oper.nError = Error::illSymbol;
    oper.work = false;
    oper.comment = str.substr(i);
    return State::End;
}
OperatorParser::State OperatorParser::NoColon(string_view str, size_t i, Operator& oper, string&) noexcept
{
    oper.nError = ASM_types::Error::noColon;
    oper.work = false;
    oper.comment = str.substr(i);
    return State::End;
}
// переход в конец из-за комментария в самом начале
OperatorParser::State OperatorParser::StartToEnd(string_view str, size_t i, Operator& oper, string&) noexcept
{
    oper.work = false;
    oper.comment = str.substr(i);
    return State::End;
}
// переходы с добавлением символа во временную строку
OperatorParser::State OperatorParser::ToLabelOrOper(string_view str, size_t i, Operator&, string &tempStr) noexcept
{
    tempStr += str[i];
    return State::LabelOrOper;
}

OperatorParser::State OperatorParser::ToLabel(string_view str, size_t i, Operator&, string &tempStr) noexcept
{
    tempStr += str[i];
    return State::Label;
}

OperatorParser::State OperatorParser::ToOper(string_view str, size_t i, Operator&, string &tempStr) noexcept
{
    tempStr += str[i];
    return State::Oper;
}

OperatorParser::State OperatorParser::ToArg(string_view str, size_t i, Operator&, string &tempStr) noexcept
{
    tempStr += str[i];
    return State::Arg;
}
// переходы с сохранением строки в оператор
OperatorParser::State OperatorParser::OperToArg(string_view, size_t, Operator& oper, string &tempStr) noexcept
{
    oper.code = tempStr;
    tempStr.clear();
    return State::Arg;
}

OperatorParser::State OperatorParser::OperToEnd(string_view str, size_t i, Operator& oper, string &tempStr) noexcept
{
    oper.code = tempStr;
    oper.comment = str.substr(i);
    tempStr.clear();
    return State::End;
}

OperatorParser::State OperatorParser::LabelToWaitOper(string_view, size_t, Operator& oper, string &tempStr) noexcept
{
    oper.label = tempStr;
    tempStr.clear();
    return State::WaitOper;
}

OperatorParser::State OperatorParser::LabelToEnd(string_view str, size_t i, Operator& oper, string &tempStr) noexcept
{
    oper.label = tempStr;
    oper.comment = str.substr(i);
    tempStr.clear();
    return State::End;
}

OperatorParser::State OperatorParser::ArgToEnd(string_view str, size_t i, Operator& oper, string &tempStr) noexcept
{
    oper.argument = tempStr;
    oper.comment = str.substr(i);
    tempStr.clear();
    return State::End;
}

OperatorParser::State OperatorParser::ToEnd(string_view str, size_t i, Operator& oper, string &) noexcept
{
    oper.comment = str.substr(i);
    return State::End;
}

Operator OperatorParser::parseOperator(string_view str)
{
    Operator oper = {};     // формируемый оператор
    std::string tempStr;

    State s = State::Start;         // начальное состояние
    size_t i = 0;                   // индекс символа в строке
    size_t strLen = str.length();
    while( (s != State::End)
           && (i < strLen) )
    {
        SymbolType t = getSymbolType(str[i]);	// получили класс символа
        s = statesMatrix[s][t](str,i,oper,tempStr);
        ++i;				// след символ
    }
    if (s != State::End)
    {
        // проверка состояния при выходе (если закончилась строка до End)
        if (s == State::Oper || s == State::LabelOrOper)  oper.code = tempStr;
        else if (s == State::Arg) oper.argument = tempStr;
        else if (s == State::Start)
        {
            // оператор пуст, в нем были только символы табуляции, пробелы и так далее
            oper.nError = Error::emptyStr;
            oper.work = false;
        }
        else if (s == State::Label)
        {
            oper.nError = Error::noColon;
            oper.work = false;  // обрабатывать не требуется
            oper.comment = str;	// вся строка в коммент
        }
    }
    if (!oper.code.empty()) strToLower(oper.code);
    return oper;
}
