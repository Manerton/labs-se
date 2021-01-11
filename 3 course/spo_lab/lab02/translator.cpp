#include "translator.h"

using VM_types::COP;

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

void Translator::HandleFunc(Operator& oper)
{

}

Translator::Translator()
{
    Operator oper = OperParser.parseOperator("org 1000");
    int a = 5;
}
