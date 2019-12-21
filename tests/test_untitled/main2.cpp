#include <iostream>
#include <cinttypes>
#include <sstream>
#pragma pack(1)

using namespace std;
union command2byte
{
    struct cmd_struct
    {
        uint16_t cop : 7;
        uint16_t a1  : 3;
        uint16_t a2  : 3;
        uint16_t a3  : 3;
    };
    cmd_struct cmd;
    uint8_t bytes[2];
};

union command3byte
{
    struct cmd_struct
    {
        uint8_t cop     : 7;
        uint8_t b       : 1;
        uint16_t adr     : 16;
    };
    cmd_struct cmd;
    uint8_t bytes[3];
};

union command4byte
{
    struct cmd_struct
    {
        uint16_t cop     : 7;
        uint16_t a1      : 3;
        uint16_t a2      : 3;
        uint16_t a3      : 3;
        uint16_t adr    : 16;
    };
    cmd_struct cmd;
    uint8_t bytes[4];
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
    string s3 = "12 1 65510";   // -- трехбайтная команда -- //
    string s4 = "13 1 1 1 65510";   // -- четырехбайтная команда -- //

    // читаю 2-байтную
    stringstream ss(s2);
    command2byte command2b;
    int tmp = 0;
    ss >> tmp;
    command2b.cmd.cop = tmp;
    ss >> tmp;
    command2b.cmd.a1 = tmp;
    ss >> tmp;
    command2b.cmd.a2 = tmp;
    ss >> tmp;
    command2b.cmd.a3 = tmp;
    // гружу 2-байтную
    for (int i = 0; i < 2; ++i)
    {
        mem[start_address+i] = command2b.bytes[i];
        //cout << "mem1 " << int(command2b.bytes[i]) << endl;
    }
    start_address += 2;
    // читаю 3-байтную
    stringstream sss(s3);
    command3byte command3b;
    tmp = 0;
    sss >> tmp;
    command3b.cmd.cop = tmp;
    sss >> tmp;
    command3b.cmd.b = tmp;
    sss >> tmp;
    command3b.cmd.adr = tmp;
    // гружу 3-байтную
    for (int i = 0; i < 3; ++i)
    {
        mem[start_address+i] = command3b.bytes[i];
        //cout << "mem2 " << int(command3b.bytes[i]) << endl;
    }
    start_address += 3;
    // читаю 4-байтную
    stringstream ssss(s4);
    command4byte command4b;
    tmp = 0;
    ssss >> tmp;
    command4b.cmd.cop = tmp;
    ssss >> tmp;
    command4b.cmd.a1 = tmp;
    ssss >> tmp;
    command4b.cmd.a2 = tmp;
    ssss >> tmp;
    command4b.cmd.a3 = tmp;
    ssss >> tmp;
    command4b.cmd.adr = tmp;
    // гружу 4-байтную
    for (int i = 0; i < 4; ++i)
    {
        mem[start_address+i] = command4b.bytes[i];
        //cout << "mem3 " << int(command4b.bytes[i]) << endl;
    }

    allcommands command_in_cpu;
    for (int i = 0; i < 4; ++i)
    {
        command_in_cpu.bytes[i] = mem[i+ip];
    }
    cout << int(command_in_cpu.cmd2byte.cmd.cop) << endl;   // -- читаю число 11, типо, ага, 11 - это двубайтная команда --
    ip += 2; // сразу смещу ip
    cout << "2-byte cmd: " << int(command_in_cpu.cmd2byte.cmd.a1) << " " << int(command_in_cpu.cmd2byte.cmd.a2) << " " << int(command_in_cpu.cmd2byte.cmd.a3) << endl;

    // гружу след команду
    for (int i = 0; i < 4; ++i)
    {
        command_in_cpu.bytes[i] = mem[i+ip];
    }
    cout << int(command_in_cpu.cmd4byte.cmd.cop) << endl;   // -- читаю число 12, типо, ага, 12 - это трехбайтная команда --
    ip += 3; // сразу смещу ip
    cout << "3-byte cmd: " << int(command_in_cpu.cmd3byte.cmd.b) << " " << int(command_in_cpu.cmd3byte.cmd.adr) << endl;

    // гружу след команду
    for (int i = 0; i < 4; ++i)
    {
        command_in_cpu.bytes[i] = mem[i+ip];
    }
    cout << int(command_in_cpu.cmd4byte.cmd.cop) << endl;   // -- читаю число 13, типо, ага, 13 - это четырехбайтная команда --
    ip += 4; // сразу смещу ip
     cout << "4-byte cmd: " << int(command_in_cpu.cmd4byte.cmd.a1) << " " << int(command_in_cpu.cmd4byte.cmd.a2) << " " << int(command_in_cpu.cmd4byte.cmd.a3) << " " <<  int(command_in_cpu.cmd4byte.cmd.adr) << endl;

}
