#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "cpu.h"

using namespace std;

using VM_types::cmd_t;
using VM_types::data_t;
using VM_types::data_length;
using VM_types::cmd_length;

bool Uploader(CPU &cpu, const string &filename)
{
    ifstream file(filename);

    if (!file.is_open()) return false;

    uint16_t address = 0;   // -- адрес памяти, по которому загружаются данные или команды -- //
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
                uint16_t temp;
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
                uint16_t IP;
                ss >> IP;
                cpu.PSW.IP = IP;
                break;
            }
        }
    }
    file.close();
    return true;
}

int main (int argc, char* argv[])
{
    CPU cpu;
    if (argc > 1)
    {
        cout << "File " << argv[1] << " is ";
        if (Uploader(cpu, argv[1]))
        {
            cout << "uploaded!" << endl;
            cpu.run();
        }
        else cout << "not uploaded" << endl;
    } else cout << "Program has been started without arguments." << endl;
    system("pause");

    return 0;
}

