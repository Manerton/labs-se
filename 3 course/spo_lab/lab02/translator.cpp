#include "translator.h"
#include <fstream>
#include <string_view>

using VM_types::COP;
using VM_types::address_t;
using ASM_types::Error;
using namespace std;

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
    TableOperations["int"]         = {HandleFunc,COP::nop};
    TableOperations["float"]       = {HandleFunc,COP::nop};
    TableOperations["memint"]      = {HandleFunc,COP::nop};
    TableOperations["memfloat"]    = {HandleFunc,COP::nop};
    // -- остальные директивы -- //
    TableOperations["equ"]         = {HandleFunc,COP::nop, false};
    TableOperations["proc"]        = {HandleFunc,COP::nop};
    TableOperations["endp"]        = {HandleFunc,COP::nop, false};
    TableOperations["org"]         = {HandleFunc,COP::nop, false};
    TableOperations["end"]         = {HandleFunc,COP::nop, false};
    // -- команды пересылки данных -- //
    TableOperations["xchg"]         = {HandleFunc,COP::exchange};
    TableOperations["mov"]          = {HandleFunc,COP::move};
    TableOperations["loadi"]        = {HandleFunc,COP::sign_value_load};
    TableOperations["loadu"]        = {HandleFunc,COP::unsign_value_load};
    TableOperations["load"]         = {HandleFunc,COP::direct_load};
    TableOperations["save"]         = {HandleFunc,COP::save};
    TableOperations["savep"]        = {HandleFunc,COP::save_pop};
    TableOperations["deref"]        = {HandleFunc,COP::dereference_ptr};
    // -- команды целой арифметики -- //
    TableOperations["iadd"]         = {HandleFunc,COP::iAdd};
    TableOperations["isub"]         = {HandleFunc,COP::iSub};
    TableOperations["imul"]         = {HandleFunc,COP::iMul};
    TableOperations["idiv"]         = {HandleFunc,COP::iDiv};
    // -- команды дробной арифметики -- //
    TableOperations["fadd"]         = {HandleFunc,COP::fAdd};
    TableOperations["fsub"]         = {HandleFunc,COP::fSub};
    TableOperations["fmul"]         = {HandleFunc,COP::fMul};
    TableOperations["fdiv"]         = {HandleFunc,COP::fDiv};
    // -- команды ввода и вывода -- //
    // со стеком
    TableOperations["insti"]        = {HandleFunc,COP::st_in_int};
    TableOperations["instf"]        = {HandleFunc,COP::st_in_float};
    TableOperations["outsti"]       = {HandleFunc,COP::st_out_int};
    TableOperations["outstu"]       = {HandleFunc,COP::st_out_uint};
    TableOperations["outstf"]       = {HandleFunc,COP::st_out_float};
    // с переменной в памяти по адресу в команде
    TableOperations["inmemi"]       = {HandleFunc,COP::mem_in_int};
    TableOperations["inmemf"]       = {HandleFunc,COP::mem_in_float};
    TableOperations["outmemi"]      = {HandleFunc,COP::mem_out_int};
    TableOperations["outmemu"]      = {HandleFunc,COP::mem_out_uint};
    TableOperations["outmemf"]      = {HandleFunc,COP::mem_out_float};
    // -- команды перехода -- //
    TableOperations["jmp"]          = {HandleFunc,COP::direct_jmp};
    TableOperations["jmpofp"]       = {HandleFunc,COP::offset_plus_jmp};
    TableOperations["jmpofm"]       = {HandleFunc,COP::offset_minus_jmp};
    TableOperations["jmpind"]       = {HandleFunc,COP::indirect_jmp};
    TableOperations["jzf"]          = {HandleFunc,COP::jzf};
    TableOperations["jnzf"]         = {HandleFunc,COP::jsf};
    TableOperations["jnsf"]         = {HandleFunc,COP::jnsf};
    TableOperations["call"]         = {HandleFunc,COP::call};
    TableOperations["ret"]          = {HandleFunc,COP::ret};
}

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
                t.function(oper);   // вызываем обработчик директивы
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

void Translator::HandleFunc(Operator& oper)
{

}

Translator::Translator(const std::string_view filename)
{
    initTables();
    std::ifstream source(filename.cbegin());
    firstPass(source);
    int a = 5;
}
