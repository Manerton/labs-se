#include "translator.h"
#include "mathparser.h"
#include <fstream>
#include <string_view>

using VM_types::COP;
using VM_types::Prefix;
using VM_types::byte_t;
using VM_types::data_t;
using VM_types::address_t;
using ASM_types::Error;
using ASM_types::LabelValue;
using ASM_types::LC_Symbol;

using namespace std;
// заполнение таблиц транслятора
void Translator::initTables() noexcept
{
    // в таблицу имен добавляем счетчик размещения
    TableNames[LC_Symbol] = 0;
    // заполняем таблицу операций
    /*  формат элемента таблицы операций:
        1. функция обработчик,
        2. КОП,
        3. использует общий механизм обработки меток? (по умолчанию true) */

    // -- директивы определения и резервирования данных -- //
    TableOperations["int"]         = {defineIntData,COP::nop};
    TableOperations["float"]       = {defineFloatData,COP::nop};
    TableOperations["memint"]      = {reserveData<int>,COP::nop};
    TableOperations["memfloat"]    = {reserveData<float>,COP::nop};
    // -- остальные директивы -- //
    TableOperations["equ"]         = {equ,COP::nop};
    TableOperations["proc"]        = {proc,COP::nop};
    TableOperations["endp"]        = {endp,COP::nop, false};
    TableOperations["org"]         = {org,COP::nop, false};
    TableOperations["end"]         = {end,COP::nop, false};

    constexpr bool Arg = true;      // команда требует аргумент
    constexpr bool noArg = false;   // команда не требует аргумент
    // -- команды пересылки данных -- //
    TableOperations["xchg"]         = {cmdHandle<Arg>,COP::exchange};
    TableOperations["mov"]          = {cmdHandle<Arg>,COP::move};
    TableOperations["loadi"]        = {cmdHandle<Arg>,COP::sign_value_load};
    TableOperations["loadu"]        = {cmdHandle<Arg>,COP::unsign_value_load};
    TableOperations["load"]         = {cmdHandle<Arg>,COP::direct_load};
    TableOperations["save"]         = {cmdHandle<Arg>,COP::save};
    TableOperations["savep"]        = {cmdHandle<Arg>,COP::save_pop};
    TableOperations["deref"]        = {cmdHandle<Arg>,COP::dereference_ptr};
    // -- команды целой арифметики -- //
    TableOperations["iadd"]         = {cmdHandle<Arg>,COP::iAdd};
    TableOperations["isub"]         = {cmdHandle<Arg>,COP::iSub};
    TableOperations["imul"]         = {cmdHandle<Arg>,COP::iMul};
    TableOperations["idiv"]         = {cmdHandle<Arg>,COP::iDiv};
    // -- команды дробной арифметики -- //
    TableOperations["fadd"]         = {cmdHandle<Arg>,COP::fAdd};
    TableOperations["fsub"]         = {cmdHandle<Arg>,COP::fSub};
    TableOperations["fmul"]         = {cmdHandle<Arg>,COP::fMul};
    TableOperations["fdiv"]         = {cmdHandle<Arg>,COP::fDiv};
    // -- команды ввода и вывода -- //
    // со стеком
    TableOperations["insti"]        = {cmdHandle<noArg>,COP::st_in_int};
    TableOperations["instf"]        = {cmdHandle<noArg>,COP::st_in_float};
    TableOperations["outsti"]       = {cmdHandle<noArg>,COP::st_out_int};
    TableOperations["outstu"]       = {cmdHandle<noArg>,COP::st_out_uint};
    TableOperations["outstf"]       = {cmdHandle<noArg>,COP::st_out_float};
    // с переменной в памяти по адресу в команде
    TableOperations["inmemi"]       = {cmdHandle<Arg>,COP::mem_in_int};
    TableOperations["inmemf"]       = {cmdHandle<Arg>,COP::mem_in_float};
    TableOperations["outmemi"]      = {cmdHandle<Arg>,COP::mem_out_int};
    TableOperations["outmemu"]      = {cmdHandle<Arg>,COP::mem_out_uint};
    TableOperations["outmemf"]      = {cmdHandle<Arg>,COP::mem_out_float};
    // -- команды перехода -- //
    TableOperations["jmp"]          = {cmdHandle<Arg>,COP::direct_jmp};
    TableOperations["jmpofp"]       = {cmdHandle<Arg>,COP::offset_plus_jmp};
    TableOperations["jmpofm"]       = {cmdHandle<Arg>,COP::offset_minus_jmp};
    TableOperations["jmpind"]       = {cmdHandle<Arg>,COP::indirect_jmp};
    TableOperations["jzf"]          = {cmdHandle<Arg>,COP::jzf};
    TableOperations["jnzf"]         = {cmdHandle<Arg>,COP::jsf};
    TableOperations["jnsf"]         = {cmdHandle<Arg>,COP::jnsf};
    TableOperations["call"]         = {cmdHandle<Arg>,COP::call};
    TableOperations["ret"]          = {cmdHandle<noArg>,COP::ret};
}
// стандартная обработка метки из оператора
void Translator::LabelDo(Operator &oper)
{
    const auto label = oper.label;
    if (!label.empty())
    {
        if (TableNames.find(label) == TableNames.end()) // метки в таблице нет
        {
            auto lowerLabel = label;
            OperatorParser::strToLower(lowerLabel);
            // если метка не является зарезервированным словом (операцией)
            if (TableOperations.find(lowerLabel) == TableOperations.end())
            {
                TableNames[label] = TableNames[LC_Symbol];
            }
            else
            {
                oper.nError = Error::illLabel;
                oper.work = false;
            }
        }
        else    // повторное определение
        {
            oper.nError = Error::reuseLabel;
            oper.work = false;
        }
    }
}
// первый проход - вычисление LC, занесение метки в таблицу имен, выполнение директивы
void Translator::firstPass_handleOper(Operator &oper)
{
    oper.LC = static_cast<address_t>(TableNames[LC_Symbol]);
    if (!oper.code.empty()) // если есть операция
    {
        // если такая операция существует
        const auto cmd_it = TableOperations.find(oper.code);
        if (cmd_it != TableOperations.end())
        {
            const Operation t = cmd_it->second;
            if (t.wLabel)   // стандартная обработка метки
            {
                LabelDo(oper);
            }
            // если директива
            if (t.COP == COP::nop)
            {
                oper.work = false;  // не транслируем
                t.function(oper,TableNames);   // вызываем обработчик директивы
            }
            else // если команда
            {
                // увеличиваем LC и ограничиваем его по памяти приводя к address_t
                TableNames[LC_Symbol] = static_cast<address_t>(TableNames[LC_Symbol] + VM_types::cmd_length);
            }
        }
        else  // такой операции не существует
        {
            oper.nError = Error::illOperation;
            oper.work = false;
        }
    }
    else if (!oper.label.empty()) LabelDo(oper); // если операции нет, должна быть метка
}

// первый проход - чтение строк и разбор операторов из строк
void Translator::firstPass(std::ifstream &source)
{
    string str;     // читаемая строка программы
    Operator oper;  // разобранный оператор
    uint32_t n = 0; // номер оператора

    // читаем пока есть строки в файле до директивы end
    while (getline(source, str) && oper.code != "end")
    {
        if (!str.empty())   // если строка не пустая
        {
            oper = OperParser.parseOperator(str);
            oper.number = ++n;
            firstPass_handleOper(oper);
            program.push_back(oper);
        }
    }

    // файл прочитан полностью, а директиву end не встретили
    if (oper.code != "end")
    {
        oper = {};  // очищаем оператор
        oper.work = false;
        oper.nError = Error::noEnd;
        oper.number = ++n;
        program.push_back(oper);
    }
}

void Translator::secondPass()
{
    for (auto &oper: program)
    {
        // если коммент, директива или была ошибка при первом проходе, то не обрабатываем
        if (oper.work)
        {
            // при первом проходе убедились, что такая команда существует
            // поэтому сразу берем из таблицы
            Operation cmd = TableOperations[oper.code];
            oper.binary.push_back(Prefix::cmd);
            oper.binary.push_back(cmd.COP);
            // вызываем транслирующую функцию
            cmd.function(oper, TableNames);
            oper.work = false;
        }
    }
}

// вычисление выражения (с помощью MathParser) (в случае ошибки, она занесется в oper)
LabelValue Translator::compute(Operator &oper, TableNames_t &TNames)
{
    Error err = Error::noError;
    // вычисляем число из аргумента операции с помощью MathParser
    MathParser MParser(TNames,oper.argument,err);
    const LabelValue res = MParser.eval();
    // если при вычислении произошла ошибка
    if (err != Error::noError)
    {
        oper.nError = err;
        oper.work = false;
        return 0;
    }
    return res;
}
// -- функции-обработчики операций -- //
 // для директив определения данных
void Translator::defineIntData(Operator &oper, TableNames_t &TNames)
{
    if (!oper.argument.empty()) // если есть аргумент
    {
        // вычисляем число из аргумента операции
        const LabelValue res = compute(oper, TNames);
        // если не было ошибки при вычислении
        if (oper.nError == Error::noError)
        {
            data_t d;
            d.u = res;
            oper.binary.push_back(Prefix::integer);
            oper.binary.insert(oper.binary.end(),d.b.begin(),d.b.end());
            TNames[LC_Symbol] = static_cast<address_t>(TNames[LC_Symbol] + VM_types::data_length);
        }
    } else oper.nError = Error::illCountArgument;
}
void Translator::defineFloatData(Operator &oper, TableNames_t &TNames)
{
    if (!oper.argument.empty()) // если есть аргумент
    {
        Error err = Error::noError;
        // переводим в дробное число с помощью MathParser
        const float res = MathParser::StrToFloat(oper.argument,err);
        // если не было ошибки при вычислении
        if (err == Error::noError)
        {
            data_t d;
            d.f = res;
            oper.binary.push_back(Prefix::real);
            oper.binary.insert(oper.binary.end(),d.b.begin(),d.b.end());
            TNames[LC_Symbol] = static_cast<address_t>(TNames[LC_Symbol] + VM_types::data_length);
        }
        else oper.nError = err;
    } else oper.nError = Error::illCountArgument;
}
// для резервирования данных
template <typename T>
void Translator::reserveData(Operator &oper, TableNames_t &TNames)
{
    if (!oper.argument.empty()) // если есть аргумент
    {
        // вычисляем число - сколько чисел в памяти нужно зарезервировать
        // результат вычисления ограничим: размером памяти ВМ / размер данных
        const address_t limiter = (numeric_limits<address_t>::max() / VM_types::data_length);
        const address_t count = compute(oper, TNames) % (limiter);
        // если не было ошибки при вычислении
        if (oper.nError == Error::noError)
        {
            byte_t prefix = Prefix::integer;
            if (std::is_same_v<T,float>) prefix = Prefix::real;
            data_t d;
            d.u = 0;    // согласно IEEE 754 для float представление 0 такой же как и для uint
            for (address_t i = 0; i < count; ++i)
            {
                oper.binary.push_back(prefix);
                oper.binary.insert(oper.binary.end(),d.b.begin(),d.b.end());
            }
            TNames[LC_Symbol] = static_cast<address_t>(TNames[LC_Symbol] + (VM_types::data_length * count));
        }
    } else oper.nError = Error::illCountArgument;
}
// директива equ
void Translator::equ(Operator &oper, TableNames_t &TNames)
{
    if (!oper.label.empty()) // должна быть метка
    {
        if (!oper.argument.empty()) // должен быть аргумент
        {
            // вычисляем число из аргумента операции
            const LabelValue res = compute(oper, TNames);
            // если не было ошибки при вычислении
            if (oper.nError == Error::noError)
            {
                TNames[oper.label] = res;
            }
        } else oper.nError = Error::illCountArgument;
    } else oper.nError = Error::noLabel;

}
// директива proc
void Translator::proc(Operator &oper, TableNames_t&)
{
    if (oper.label.empty()) // если метки нет, то ошибка
    {
        oper.nError = Error::noLabel;
    }
    else if (!oper.argument.empty()) // если аргументы есть, то ошибка
    {
        oper.nError = Error::illCountArgument;
    }
}
// директива endp
void Translator::endp(Operator &oper, TableNames_t& TNames)
{
    if (!oper.argument.empty()) // если аргумент есть
    {
        if (TNames.find(oper.argument) == TNames.end()) // если такой метки в таблицы нет
        {
            oper.nError = Error::undefLabel;
        }
    }
}
// директива org
void Translator::org(Operator &oper, TableNames_t& TNames)
{
    if (!oper.argument.empty()) // должен быть аргумент
    {
        // вычисляем число из аргумента операции, число ограничиваем размером памяти ВМ
        // так как org устанавливает LC
        const auto res = static_cast<address_t>(compute(oper, TNames));
        // если не было ошибки при вычислении
        if (oper.nError == Error::noError)
        {
            data_t d;
            d.u = res;
            oper.binary.push_back(Prefix::loadAddress);
            // транслируем адрес
            oper.binary.insert(oper.binary.end(),d.b.begin(),d.b.begin() + sizeof(address_t));
            // меняем счетик размещения
            TNames[LC_Symbol] = res;
        }
    } else oper.nError = Error::illCountArgument;
}
// директива end
void Translator::end(Operator &oper, TableNames_t& TNames)
{
    oper.binary.push_back(Prefix::execute);
    data_t d;
    d.u = 0; // если аргумента не будет, запишем нулевой адрес
    if (!oper.argument.empty()) // если есть аргумент (адрес IP)
    {
        const auto res = static_cast<address_t>(compute(oper, TNames));
        // если не было ошибки при вычислении
        if (oper.nError == Error::noError) d.u = res;
    }
    // транслируем адрес
    oper.binary.insert(oper.binary.end(),d.b.begin(),d.b.begin() + sizeof(address_t));
}
// обработчик команд ВМ
template <bool hasArg>
void Translator::cmdHandle(Operator& oper, TableNames_t& TNames)
{
    // если должен быть аргумент и он есть
    // или аргумента не должно быть и его нет
    if (hasArg == !oper.argument.empty())
    {
        data_t d;
        d.u = 0;
        if (hasArg) // если команда требует аргумента
        {
            const auto res = static_cast<address_t>(compute(oper, TNames));
            // если не было ошибки при вычислении
            if (oper.nError == Error::noError) d.u = res;
        }
        oper.binary.insert(oper.binary.end(),d.b.begin(),d.b.begin() + sizeof(address_t));
    }
    else
    {
        oper.nError = Error::illCountArgument;
        oper.work = false;
    }
}

Translator::Translator(const std::string_view filename)
{
    initTables();
    std::ifstream source(filename.cbegin());
    if (source)
    {
        firstPass(source);
        source.close();
        secondPass();
    }

    //auto pr = program;
    //int a = 0;
}

void Translator::createBinFile(const string_view filename)
{
    ofstream output(filename.cbegin(), ios::binary);
    if (output) {
        for (const auto &oper: program)
        {
            if (!oper.binary.empty())
            {
                for (const byte_t byte: oper.binary)
                {
                    output << byte;
                    //execute.write((char *) &byte, sizeof(char));
                }
            }
        }
    }
    output.close();
}
