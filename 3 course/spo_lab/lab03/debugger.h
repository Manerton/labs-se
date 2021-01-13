#ifndef DEBUGGER_H
#define DEBUGGER_H
#include "cpu.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>

class Debugger
{
    using Tokens = std::vector<std::string>;
    using Log = Tokens;
    using Operation = std::function<void(const Tokens&, Debugger&)>;
    using TableOperations_t = std::map<std::string,Operation>;
    CPU cpu;
    VM_types::address_t startAddress = 0;
    Log log;
    TableOperations_t tableOperations;
    std::string filename;

// методы
    void initTable();
    static Tokens split(const std::string& s, char delim) noexcept;
    static inline void ltrim(std::string &s);
    static VM_types::AccessMode getAccessMode(char ch);
    // к нижнему регистру все символы строки
    static inline void strToLower(std::string& s);
    static void stepByStep(const Tokens &args, Debugger &dbg);
    static void normalRun(const Tokens &args, Debugger &dbg);
    static void nextBP(const Tokens &args, Debugger &dbg);
    static void setBP(const Tokens &args, Debugger &dbg);
    static void removeBP(const Tokens &args, Debugger &dbg);
    static void displayStack(const Tokens &args, Debugger &dbg);
    static void displayMem(const Tokens &args, Debugger &dbg);
    static void setMem(const Tokens &args, Debugger &dbg);
    static void setMemFloat(const Tokens &args, Debugger &dbg);
    static void exitDbg(const Tokens &args, Debugger &dbg);

public:
    explicit Debugger(const std::string &file) : cpu(true), filename(file)
    {
        initTable();
    }
    CPU& getCpu();
    void run();
};

#endif // DEBUGGER_H
