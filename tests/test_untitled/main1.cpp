#include <iostream>
#include <cinttypes>
#include <sstream>
#pragma pack(1)

using namespace std;

struct command2byte
{
    uint16_t cop : 7;
    uint16_t a1  : 3;
    uint16_t a2  : 3;
    uint16_t a3  : 3;
};
struct command3byte
{
    uint16_t b       : 1;
    uint16_t cop     : 7;
    uint16_t adr     : 16;
};

struct command4byte
{
    uint16_t cop     : 7;
    uint16_t a1      : 3;
    uint16_t a2      : 3;
    uint16_t a3      : 3;
    uint16_t adr    : 16;
};

union allcommands
{
    command2byte cmd2byte;
    command3byte cmd3byte;
    command4byte cmd4byte;
    uint8_t bytes[4];
};

int main()
{
    uint16_t start_address = 150;
    uint16_t ip = start_address;
    uint8_t *mem = new uint8_t[256];
    string s2 = "11 1 2 6";   // -- двухбайтная -- //
    string s3 = "10 1 65510";   // -- трехбайтная команда -- //
    string s4 = "13 1 1 1 65510";   // -- четырехбайтная команда -- //

    // читаю 2-байтную
    stringstream ss(s2);
    allcommands command2b;
    int tmp = 0;
    ss >> tmp;
    command2b.cmd2byte.cop = tmp;
    ss >> tmp;
    command2b.cmd2byte.a1 = tmp;
    ss >> tmp;
    command2b.cmd2byte.a2 = tmp;
    ss >> tmp;
    command2b.cmd2byte.a3 = tmp;
    // гружу 2-байтную
    for (int i = 0; i < 4; ++i)
    {
        mem[start_address+i] = command2b.bytes[i];
        cout << "mem1 " << int(mem[start_address+i]) << endl;
    }
    start_address += 2;
    // читаю 3-байтную
    stringstream sss(s3);
    allcommands command3b;
    tmp = 0;
    sss >> tmp;
    command3b.cmd3byte.cop = tmp;
    sss >> tmp;
    command3b.cmd3byte.b = tmp;
    sss >> tmp;
    command3b.cmd3byte.adr = tmp;
    // гружу 3-байтную
    for (int i = 0; i < 4; ++i)
    {
        mem[start_address+i] = command3b.bytes[i];
        cout << "mem2 " << int(mem[start_address+i]) << endl;
    }
    start_address += 3;
    // читаю 4-байтную
    stringstream ssss(s4);
    allcommands command4b;
    tmp = 0;
    ssss >> tmp;
    command4b.cmd4byte.cop = tmp;
    ssss >> tmp;
    command4b.cmd4byte.a1 = tmp;
    ssss >> tmp;
    command4b.cmd4byte.a2 = tmp;
    ssss >> tmp;
    command4b.cmd4byte.a3 = tmp;
    ssss >> tmp;
    command4b.cmd4byte.adr = tmp;
    // гружу 4-байтную
    for (int i = 0; i < 4; ++i)
    {
        mem[start_address+i] = command4b.bytes[i];
        cout << "mem3 " << int(command4b.bytes[i]) << endl;
    }

    allcommands command_in_cpu;
    for (int i = 0; i < 4; ++i)
    {
        command_in_cpu.bytes[i] = mem[i+ip];
    }
    cout << int(command_in_cpu.cmd2byte.cop) << endl;   // -- читаю число 11, типо, ага, 11 - это двубайтная команда --
    ip += 2; // сразу смещу ip
    cout << "2-byte cmd: " << int(command_in_cpu.cmd2byte.a1) << " " << int(command_in_cpu.cmd2byte.a2) << " " << int(command_in_cpu.cmd2byte.a3) << endl;

    // гружу след команду
    for (int i = 0; i < 4; ++i)
    {
        command_in_cpu.bytes[i] = mem[i+ip];
    }
    cout << int(command_in_cpu.cmd4byte.cop) << endl;   // -- читаю число 12, типо, ага, 12 - это трехбайтная команда --
    ip += 3; // сразу смещу ip
    cout << "3-byte cmd: " << int(command_in_cpu.cmd3byte.b) << " " << int(command_in_cpu.cmd3byte.adr) << endl;

    // гружу след команду
    for (int i = 0; i < 4; ++i)
    {
        command_in_cpu.bytes[i] = mem[i+ip];
    }
    cout << int(command_in_cpu.cmd4byte.cop) << endl;   // -- читаю число 13, типо, ага, 13 - это четырехбайтная команда --
    ip += 4; // сразу смещу ip
     cout << "4-byte cmd: " << int(command_in_cpu.cmd4byte.a1) << " " << int(command_in_cpu.cmd4byte.a2) << " " << int(command_in_cpu.cmd4byte.a3) << " " <<  int(command_in_cpu.cmd4byte.adr) << endl;

}
