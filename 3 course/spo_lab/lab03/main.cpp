#include "types.h"
#include "cpu.h"
#include "debugger.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

using VM_types::byte_t;
using VM_types::cmd_t;
using VM_types::data_t;
using VM_types::address_t;
using VM_types::data_length;
using VM_types::cmd_length;

bool TxtUploader(CPU &cpu, const string_view filename)
{
    ifstream file(filename.cbegin());

    if (!file.is_open()) return false;

    address_t address = 0;   // -- адрес памяти, по которому загружаются данные или команды -- //
    char symbol = ' ';      // -- текущий читаемый символ -- //
    while (symbol != 'e')   // -- пока не встретили символ 'e' -- //
    {
        string s;
        getline(file, s);
        istringstream ss(s);
        ss >> symbol;
        switch (symbol)
        {
            case 'a':   // -- установить адрес -- //
            {
                ss >> address;
                break;
            }
            case 'i': case 'u': // -- загрузка данных: целое, беззнаковое -- //
            {
                data_t chislo;
                ss >> chislo.u;
                cpu.ram.push(chislo, address);
                address += data_length;
                break;
            }
            case 'f': // -- загрузка данных: число с плавающей точкой -- //
            {
                data_t chislo;
                ss >> chislo.f; // -- читаем как число с плавающей точкой -- //
                cpu.ram.push(chislo, address);
                address += data_length;
                break;
            }
            case 'c':   // -- загрузка команды -- //
            {
                cmd_t command; // -- пример: c cop address (для 24-битной команды) -- //
                uint16_t temp = 0;
                ss >> temp; // -- читаем cop (код операции) -- //
                command.c.cop = uint8_t(temp);
                ss >> temp; // -- читаем address -- //
                command.c.address = temp;
                cpu.ram.push(command, address);
                address += cmd_length;
                break;
            }
            case 'e':   // -- начинаем работу процессора -- //
            {
                cmd_t command;
                command.c.cop = CPU::COP::stop;
                cpu.ram.push(command,address);
                address_t IP = 0;
                ss >> IP;
                cpu.setIP(IP);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    file.close();
    return true;
}

bool BinUploader(CPU &cpu, const string_view filename)
{
    ifstream file(filename.cbegin(), ios::binary);

    if (!file.is_open()) return false;

    address_t address = 0;   // -- адрес памяти, по которому загружаются данные или команды -- //
    char symbol = ' ';      // -- текущий читаемый символ -- //
    while (symbol != 'e')   // -- пока не встретили символ 'e' -- //
    {
        file >> symbol;
        switch (symbol)
        {
            case 'a':   // -- установить адрес -- //
            {
                file.read(reinterpret_cast<char*>(&address),sizeof(address_t));
                break;
            }
            case 'i': case 'u': case 'f': // -- загрузка данных: целое, беззнаковое, дробное -- //
            {
                data_t chislo = {};
                file.read(reinterpret_cast<char*>(&chislo),sizeof(data_t));
                cpu.ram.push(chislo, address);
                address += data_length;
                break;
            }
            case 'c':   // -- загрузка команды -- //
            {
                cmd_t command; // -- пример: c cop address (для 24-битной команды) -- //
                file.read(reinterpret_cast<char*>(&command),sizeof(cmd_t));
                cpu.ram.push(command, address);
                address += cmd_length;
                break;
            }
            case 'e':   // -- начинаем работу процессора -- //
            {
                cmd_t command;
                command.c.cop = CPU::COP::stop;
                cpu.ram.push(command,address);
                address_t IP = 0;
                file.read(reinterpret_cast<char*>(&IP),sizeof(address_t));
                cpu.setIP(IP);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    file.close();
    return true;
}

bool Uploader(CPU &cpu, const string_view filename)
{
    size_t pointIndex = filename.rfind('.');
    std::string filetype{filename.substr(pointIndex+1)};
    if (filetype == "bin")
    {
        return BinUploader(cpu,filename);
    }
    if (filetype == "txt")
    {
        return TxtUploader(cpu,filename);
    }
    cout << " is without file extension" << endl;
    return false;
}

int main (int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string filename = argv[1];
        //std::string_view filename = R"(C:\GIT\cpp_workspace\_qt_workspace\3 course\spo_lab\lab02\tests\test_perecilki_asm.bin)";
        cout << "File " << filename;
        Debugger dbg(filename);
        if (Uploader(dbg.getCpu(), filename))
        {
            cout << " is uploaded!" << endl;
            dbg.run();
        }
        else cout << "File is not uploaded" << endl;
    } else cout << "Program has been started without arguments." << endl;
    system("pause");

    return 0;
}
