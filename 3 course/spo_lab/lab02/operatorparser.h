#ifndef OPERATORPARSER_H
#define OPERATORPARSER_H
#include "types.h"
#include <cstdint>
#include <array>
#include <string_view>
#include <functional>

// Конечный автомат для разбора оператора
class OperatorParser
{
    // состояния КА
    enum State: uint8_t
    {
        Start,          // стартовое состояние
        LabelOrOper,    // пока не знаем, метка или операция
        Label,          // ввод метки
        WaitOper,       // ожидание кода операции
        Oper,           // ввод кода операции
        Arg,            // ввод аргумента
        End,            // конечное состояние

        StateELEMCOUNT           // количество состояний
    };
    // классы символов
    enum SymbolType: uint8_t
    {
        Blank,          // пробел, символ табуляции
        Colon,          // двоеточие
        Comment,        // комментарий
        Digit,          // цифра
        Id,             // идентификатор (английский символ)
        Underline,      // подчеркивание
        Other,          // любой символ

        SymbolTypeELEMCOUNT   // количество классов символов
    };
// поля
    // матрица переходов: строки определяют состояния, столбцы - классы символов
    static constexpr auto statesCount       = State::StateELEMCOUNT-1; // (без End)
    static constexpr auto symbolTypeCount   = SymbolType::SymbolTypeELEMCOUNT;
    using string_view = std::string_view;
    using string = std::string;
    using Operator = ASM_types::Operator;
    using Handler = std::function<State(const string_view, const size_t, Operator&, string&)>;
    using matrixRow = std::array<Handler,symbolTypeCount>;
    using Matrix = std::array<matrixRow,statesCount>;

    Matrix statesMatrix = {};

// методы

    SymbolType getSymbolType(char c) const;
    // действия при переходах
    // переход без действия
    static State ToStart(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State ToWaitOper(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State BlankToArg(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    // ошибки
    static State ErrToEnd(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State NoColon(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    // переход в конец из-за комментария в самом начале
    static State StartToEnd(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    // переходы с добавлением символа во временную строку
    static State ToLabelOrOper(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State ToLabel(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State ToOper(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State ToArg(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    // переходы с сохранением строки в оператор
    static State OperToArg(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State OperToEnd(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State LabelToWaitOper(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State LabelToEnd(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State ArgToEnd(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    static State ToEnd(string_view str, size_t i, Operator &oper, string &tempStr) noexcept;
    void initStatesMatrix() {
        statesMatrix =
        {{
             /*                 Пробел      Двоеточие        Коммент     Цифра      Идентификатор   Подчеркивание   Любой     */
             /* Start */       {ToStart,    ErrToEnd,        StartToEnd, ErrToEnd,  ToLabelOrOper,  ToLabel,        ErrToEnd},
             /* LabelOrOper */ {OperToArg,  LabelToWaitOper, OperToEnd,  ToLabel,   ToLabelOrOper,  ToLabel,        ErrToEnd},
             /* Label */       {NoColon,    LabelToWaitOper, NoColon,    ToLabel,   ToLabel,        ToLabel,        ErrToEnd},
             /* WaitOper */    {ToWaitOper, ErrToEnd,        ToEnd,      ErrToEnd,  ToOper,         ErrToEnd,       ErrToEnd},
             /* Oper */        {OperToArg,  ErrToEnd,        OperToEnd,  ErrToEnd,  ToOper,         ErrToEnd,       ErrToEnd},
             /* Arg */         {BlankToArg, ErrToEnd,        ArgToEnd,   ToArg,     ToArg,          ToArg,          ToArg}
         }};
    }
public:
    OperatorParser()
    {
        initStatesMatrix();
    }

    // к нижнему регистру все символы строки
    static inline void strToLower(std::string& s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return std::tolower(c); }
        );
    }

    ASM_types::Operator parseOperator(string_view str);
};

#endif // OPERATORPARSER_H
