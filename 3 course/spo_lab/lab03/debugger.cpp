#include "debugger.h"
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
using VM_types::address_t;

CPU& Debugger::getCpu()
{
    return cpu;
}

void Debugger::initTable()
{
    // пошаговое выполнение
    tableOperations["step"] = stepByStep;
    // непрерывное выполнение без остановок на контрольных точках
    tableOperations["normal"] = normalRun;
    // установить точку останова: set id mode address
    tableOperations["set"] = setBP;
    // удалить точку останова (если без аргументов, то удалить все)
    // rm id или rm
    tableOperations["rm"] = removeBP;
    // перейти к следующей точке
    tableOperations["next"] = nextBP;
    // показать содержимое стека
    tableOperations["displayst"] = displayStack;
    // показать содержимое по адресу памяти
    tableOperations["displaymem"] = displayMem;
    // записать число по адресу памяти
    tableOperations["setmemi"] = setMem;
    // записать дробное число по адресу
    tableOperations["setmemf"] = setMemFloat;
    // завершить работу отладчика
    tableOperations["exit"] = exitDbg;
}

void Debugger::ltrim(string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

VM_types::AccessMode Debugger::getAccessMode(char ch)
{
    switch (ch)
    {
        case 'e':
            return VM_types::AccessMode::Exec;
        case 'r':
            return VM_types::AccessMode::Read;
        case 'w':
            return VM_types::AccessMode::Write;
        default:
            return VM_types::AccessMode::None;
    }
}

void Debugger::strToLower(string &s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); }
    );
}

void Debugger::run()
{
    std::cout << "Debugger activated" << std::endl;
    startAddress = cpu.PSW.IP;
    std::cout << "Load address = " << startAddress << std::endl;
    string readline;
    string cmd;
    while (cmd != "exit")
    {
        cout << ".dbg > ";
        getline(cin, readline);
        ltrim(readline);
        vector<string> args = split(readline, ' ');
        if (!args.empty())
        {
            cmd = args.front();
            strToLower(cmd);
            if (tableOperations[cmd])
            {
                tableOperations[cmd](args, *this);
            } else cout << ".dbg > not such command" << endl;
        }
    }
}

void Debugger::stepByStep(const Tokens& , Debugger &dbg)
{
    dbg.cpu.PSW.BPF = 0;
    dbg.cpu.PSW.TF = 1;
    dbg.cpu.run();
    if (!dbg.cpu.traceLog.empty() && dbg.cpu.PSW.EF == 0)
    {
        dbg.log.push_back("step by step");
        auto traceStr = dbg.cpu.traceLog.back();
        dbg.log.push_back(traceStr);
        cout << traceStr;
        dbg.cpu.traceLog.clear();
    }
}

void Debugger::displayStack(const Tokens& , Debugger &dbg)
{
    stringstream ss;
    dbg.log.push_back("display stack");
    for (size_t i = 1; i <= dbg.cpu.PSW.SP; ++i)
    {
        ss << "ST[" << i << "] > u: " << dbg.cpu.ST[i].u << ", ";
        ss << "i: " << dbg.cpu.ST[i].i << ", f: " << dbg.cpu.ST[i].f << endl;
    }
    cout << ss.str();
    dbg.log.push_back(ss.str());
}

void Debugger::displayMem(const Debugger::Tokens &args, Debugger &dbg)
{
    if (args.size() > 1)
    {
        stringstream ss;
        dbg.log.push_back("display memory");
        for (size_t i = 1; i < args.size(); ++i)
        {
            auto addr = static_cast<address_t>(stoi(args[i]));
            ss << "mem[" << args[i] << "] > u: " << dbg.cpu.ram.get_data(addr).u << ", ";
            ss << "i: " << dbg.cpu.ram.get_data(addr).i << ", f: " << dbg.cpu.ram.get_data(addr).f << endl;
        }
        cout << ss.str();
        dbg.log.push_back(ss.str());
    }
}

void Debugger::setMem(const Debugger::Tokens &args, Debugger &dbg)
{
    if (args.size() == 3)
    {
        dbg.log.push_back("set memory");
        auto addr = static_cast<address_t>(stoi(args[1]));
        auto val = static_cast<uint32_t>(stoll(args[2]));
        VM_types::data_t d{};
        d.u = val;
        dbg.cpu.ram.push(d,addr);
    }
}

void Debugger::setMemFloat(const Debugger::Tokens &args, Debugger &dbg)
{
    if (args.size() == 3)
    {
        dbg.log.push_back("set memory");
        auto addr = static_cast<address_t>(stoi(args[1]));
        auto val = static_cast<float>(stof(args[2]));
        VM_types::data_t d{};
        d.f = val;
        dbg.cpu.ram.push(d,addr);
    }
}

void Debugger::normalRun(const Tokens &, Debugger &dbg)
{
    dbg.cpu.PSW.BPF = 0;
    dbg.cpu.PSW.TF = 0;
    dbg.cpu.run();
    dbg.log.insert(dbg.log.end(),dbg.cpu.traceLog.begin(),dbg.cpu.traceLog.end());
    dbg.cpu.traceLog.clear();
}

void Debugger::setBP(const Debugger::Tokens &args, Debugger &dbg)
{
    if (args.size() == 4)
    {
        // set id mode address
        if (args[2].size() == 1)
        {
            VM_types::AccessMode mode = getAccessMode(args[2].front());
            if (mode != VM_types::AccessMode::None)
            {
                size_t id_BP = stoul(args[1]);
                if (id_BP < VM_types::BPCount)
                {
                    auto address = static_cast<address_t>(stoi(args[3]));
                    dbg.cpu.setBP(id_BP,mode,address);
                    dbg.log.push_back("set breakpoint " + args[1]);
                }
                else cout << ".dbg > invalid breakpoint id" << endl;
            }
            else cout << ".dbg > invalid breakpoint mode" << endl;
        }
        else cout << ".dbg > invalid command format" << endl;
    }
    else cout << ".dbg > invalid count of arguments" << endl;
}

void Debugger::removeBP(const Debugger::Tokens &args, Debugger &dbg)
{
    size_t argsCount = args.size();
    if (argsCount >= 2)
    {
        for (size_t i = 1; i < argsCount; ++i)
        {
            size_t id_BP = stoul(args[i]);
            if (id_BP < VM_types::BPCount)
            {
                dbg.cpu.removeBP(id_BP);
                dbg.log.push_back("remove breakpoint " + args[i]);
            }
            else cout << ".dbg > invalid breakpoint id" << endl;
        }
    }
    else
    {
        for (size_t i = 0; i < VM_types::BPCount; ++i)
        {
            dbg.cpu.removeBP(i);
        }
    }
}

void Debugger::exitDbg(const Debugger::Tokens &args, Debugger &dbg)
{
    ofstream file(dbg.filename + ".dbg");
    if (file)
    {
        for (auto &str : dbg.log)
        {
            file << str << endl;
        }
    }
}

void Debugger::nextBP(const Tokens &, Debugger &dbg)
{
    dbg.cpu.PSW.BPF = 1;
    dbg.cpu.PSW.TF = 0;
    if (dbg.cpu.PSW.EF == 0)
    {
        dbg.cpu.run();
        if (dbg.cpu.PSW.EF == 0)
        {
            auto traceStr = dbg.cpu.trace();
            cout << traceStr;
            dbg.log.push_back("reach breakpoint");
            dbg.log.push_back(traceStr);
        } // иначе мы дошли до конца программы
    }
}

Debugger::Tokens Debugger::split(const string &s, char delim) noexcept
{
    Tokens tokens;
    string token;
    istringstream ss(s);
    while (getline(ss,token,delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}
