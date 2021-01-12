#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "types.h"
#include "operatorparser.h"
#include <vector>
#include <string>
#include <map>

class Translator
{
    using Operator  = ASM_types::Operator;
    using Operation = ASM_types::Operation;
    using TableNames_t = ASM_types::TableNames_t;
    using TableOperations_t = ASM_types::TableOperations_t;
private:
// поля
    // операторы ассемблерной программы: [метка:] операция [аргументы]
    std::vector<Operator> program;
    // таблица имен (меток)
    TableNames_t TableNames;
    // таблица операций (команда ВМ или директива)
    TableOperations_t TableOperations;
    // Конечный автомат - разбор оператора
    OperatorParser OperParser;
// методы
    // заполнение таблиц транслятора
    void initTables() noexcept;
    // стандартная обработка метки из оператора
    void LabelDo(Operator &oper);
    // первый проход - чтение строк и разбор операторов из строк
    void firstPass(std::ifstream &source);
    // первый проход - вычисление LC, занесение метки в таблицу имен, выполнение директивы
    void firstPass_handleOper(Operator &oper);
    // второй проход - трансляция команд
    void secondPass();
    // вычисление выражения (с помощью MathParser)
    static ASM_types::LabelValue compute(Operator &oper, TableNames_t &TNames);
    // -- функции-обработчики операций -- //
    // для директив определения данных
    static void defineIntData(Operator &oper, TableNames_t &TNames);
    static void defineFloatData(Operator &oper, TableNames_t &TNames);
    // для директив резервирования данных
    template <typename T>
    static void reserveData(Operator &oper, TableNames_t &TNames);
    // директива equ
    static void equ(Operator &oper, TableNames_t &TNames);
    // директивы proc и endp
    static void proc(Operator &oper, TableNames_t &TNames);
    static void endp(Operator &oper, TableNames_t &TNames);
    // директива org
    static void org(Operator &oper, TableNames_t &TNames);
    // директива end
    static void end(Operator &oper, TableNames_t &TNames);
    // для команд ВМ
    template <bool hasArg>
    static void cmdHandle(Operator &oper, TableNames_t &TNames);
public:
    explicit Translator(const std::string_view filename);
    // создать бинарный файл
    void createBinFile(const std::string_view filename);
};

#endif // TRANSLATOR_H
