#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <windows.h>
#include <exception>
#include <sstream>
#include <cstdint>

using namespace std;

// константы длины полей модели компьютера
const uint8_t NAME_LEN = 15;
const uint8_t CPUTYPE_LEN = 25;

struct computer  // структура с информацией о модели компьютера
{
    uint32_t code = 0;                  // код
    char name[NAME_LEN] = "";           // название торговой марки
    char cpu_type[CPUTYPE_LEN] = "";    // тип процессора
    uint16_t cpu_frequency = 0;             // частота процессора в мегагерцах
    uint32_t ram_size = 0;                  // объем памяти озу в мегабайтах
    uint32_t hdd_size = 0;                  // объем жесткого диска
    uint32_t videomemory_size = 0;          // объем памяти видеокарты
    float cost = 0;                     // цена компьютера в у.е.
    uint32_t count = 0;                 // количество экземпляров, имеющихся в наличии
};

using computer_list = vector<computer>; // информация из файла/для файла

// вывод информации о компьютере
string computer_to_string(const computer& comp)
{
    stringstream ss;
    ss << "Код: " << comp.code << endl;
    ss << "Название торговой марки: " << comp.name << endl;
    ss << "Тип процессора: " << comp.cpu_type << endl;
    ss << "Частота процессора: " << comp.cpu_frequency << endl;
    ss << "Объем памяти: " << comp.ram_size << endl;
    ss << "Объем жесткого диска: " << comp.hdd_size << endl;
    ss << "Объем памяти видеокарты: " << comp.videomemory_size << endl;
    ss << "Цена компьютера: " << comp.cost << endl;
    ss << "Количество экземпляров в наличии: " << comp.count << endl << endl;
    return ss.str();
}

bool compare_computers(const computer& comp1, const computer& comp2)
{
    return ( (comp1.code == comp2.code) && (memcmp(comp1.name,comp2.name,NAME_LEN) == 0) && (memcmp(comp1.cpu_type,comp2.cpu_type,NAME_LEN) == 0) && (comp1.cpu_frequency == comp2.cpu_frequency)
             && (comp1.ram_size == comp2.ram_size) && (comp1.hdd_size == comp2.hdd_size) && (comp1.videomemory_size == comp2.videomemory_size)
             && (fabs(comp1.cost - comp2.cost) < 0.001) && (comp1.count == comp2.count) );
}

computer input() // ввод одного значения
{
    computer r;
    cout << "Код: ";
    cin >> r.code;
    cout << "Название торговой марки (15 символов): ";
    cin.sync();
    cin.getline(r.name,NAME_LEN);
    cout << "Тип процессора (25 символов): ";
    cin.sync();
    cin.getline(r.cpu_type,CPUTYPE_LEN);
    cout << "Частота процессора: ";
    cin >> r.cpu_frequency;
    cout << "Объем памяти: ";
    cin >> r.ram_size;
    cout << "Объем жесткого диска: ";
    cin >> r.hdd_size;
    cout << "Объем памяти видеокарты: ";
    cin >> r.videomemory_size;
    cout << "Цена компьютера: ";
    cin >> r.cost;
    cout << "Количество экземпляров в наличии: ";
    cin >> r.count;
    return r;
}

void initial_input(LPCWSTR FileName) // функция, с помощью которой осуществляется первичный ввод информации с клавиатуры и дозапись в файл
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    char key;
    do
    {
        computer tmp = input();
        SetFilePointer(hFile, 0, NULL, FILE_END); // добавление в конец
        WriteFile(hFile, &tmp, sizeof(computer), &dwBytes, nullptr);
        cout << "\nЖелаете продолжить ввод? (введите символ y)" << endl;
        cin >> key;
    }  while (key == 'Y' || key == 'y');
    CloseHandle(hFile);
}

// добавление записи в файл
void add_to_file(LPCWSTR FileName, const computer &comp)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) throw runtime_error("Не удалось открыть файл");
    SetFilePointer(hFile, 0, NULL, FILE_END); // добавление в конец
    DWORD dwBytes;
    WriteFile(hFile, &comp, sizeof(computer), &dwBytes, NULL);
    CloseHandle(hFile);
}

// удаление записи из файла
void delete_from_file(LPCWSTR FileName, const computer &comp)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    DWORD Bytes_count = GetFileSize(hFile, nullptr);
    DWORD offset = 0; // для хранения позиции чтения и записи

    // читаем пока не дошли до конца файла
    while(offset < Bytes_count)
    {
        computer tmp;
        SetFilePointer(hFile, offset, 0, FILE_BEGIN); // установим позицию чтения
        ReadFile(hFile, &tmp, sizeof(computer), &dwBytes, nullptr); // читаем одну модель компьютера из файла
        // если компьютер, который необходимо удалить, найден
        if (compare_computers(tmp, comp))
        {
            BYTE* buffer = new BYTE[Bytes_count-(offset+sizeof(computer))]; // создаем буффер
            ReadFile(hFile, buffer, Bytes_count-(offset+sizeof(computer)), &dwBytes, nullptr); // читаем в буффер все, что после удаляемой записи
            SetFilePointer(hFile, offset, 0, FILE_BEGIN); // возвращаемся назад
            WriteFile (hFile, buffer, Bytes_count-(offset+sizeof(computer)), &dwBytes, nullptr); // записываем на место удаляемой записи этот буффер
            SetEndOfFile(hFile);
            Bytes_count -= sizeof(computer);
            offset = 0;
        }
        else
        {
            offset += sizeof(computer); // для перехода к следующей модели компьютера
        }
    }
    CloseHandle(hFile);
}
// замена записи в файле
void replace_in_file(LPCWSTR FileName, const computer &old_comp, const computer &new_comp)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    DWORD Bytes_count = GetFileSize(hFile, nullptr);
    DWORD offset = 0; // для хранения позиции чтения и записи

    // читаем пока не дошли до конца файла
    while(offset < Bytes_count)
    {
        computer tmp;
        SetFilePointer(hFile, offset, 0, FILE_BEGIN); // установим позицию чтения
        ReadFile(hFile, &tmp, sizeof(computer), &dwBytes, nullptr); // читаем одну модель компьютера из файла
        // если компьютер, который необходимо заменить, найден
        if (compare_computers(tmp, old_comp))
        {
            SetFilePointer(hFile, offset, 0, FILE_BEGIN); // возвращаемся назад
            WriteFile (hFile, &new_comp, sizeof(computer), &dwBytes, nullptr); // записываем на место заменяемой записи новый комп
        }
        offset += sizeof(computer); // для перехода к следующей модели компьютера
    }
    CloseHandle(hFile);
}
// поиск по процессору
void find_by_cpu(LPCWSTR FileName, const char* cpu_type)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileTxt = CreateFile(L"search.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileTxt) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    DWORD computer_count = GetFileSize(hFile, nullptr)/sizeof(computer);

    for (DWORD i = 0; i < computer_count; ++i)
    {
        computer comp;
        ReadFile(hFile, &comp, sizeof(computer), &dwBytes, nullptr);
        if (memcmp(comp.cpu_type,cpu_type,NAME_LEN) == 0)
        {
            for(char a : computer_to_string(comp))
            {
                WriteFile(hFileTxt, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
    }
    CloseHandle(hFile);
}

// поиск по объему озу
void find_by_ram(LPCWSTR FileName, uint32_t ram_size)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileTxt = CreateFile(L"search.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileTxt) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    DWORD computer_count = GetFileSize(hFile, nullptr)/sizeof(computer);

    for (DWORD i = 0; i < computer_count; ++i)
    {
        computer comp;
        ReadFile(hFile, &comp, sizeof(computer), &dwBytes, nullptr);
        if (comp.ram_size == ram_size)
        {
            for(char a : computer_to_string(comp))
            {
                WriteFile(hFileTxt, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
    }
    CloseHandle(hFile);
}

// поиск по объему hdd
void find_by_hdd(LPCWSTR FileName, uint32_t hdd_size)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileTxt = CreateFile(L"search.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileTxt) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    DWORD computer_count = GetFileSize(hFile, nullptr)/sizeof(computer);

    for (DWORD i = 0; i < computer_count; ++i)
    {
        computer comp;
        ReadFile(hFile, &comp, sizeof(computer), &dwBytes, nullptr);
        if (comp.hdd_size == hdd_size)
        {
            for(char a : computer_to_string(comp))
            {
                WriteFile(hFileTxt, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
    }
    CloseHandle(hFile);
}

// поиск по объему видеопамяти
void find_by_videomemory(LPCWSTR FileName, uint32_t videomemory_size)
{
    HANDLE hFile = CreateFile(FileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileTxt = CreateFile(L"search.txt", GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile || INVALID_HANDLE_VALUE == hFileTxt) throw runtime_error("Не удалось открыть файл");
    DWORD dwBytes;
    DWORD computer_count = GetFileSize(hFile, nullptr)/sizeof(computer);

    for (DWORD i = 0; i < computer_count; ++i)
    {
        computer comp;
        ReadFile(hFile, &comp, sizeof(computer), &dwBytes, nullptr);
        if (comp.videomemory_size == videomemory_size)
        {
            for(char a : computer_to_string(comp))
            {
                WriteFile(hFileTxt, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
    }
    CloseHandle(hFile);
}

// функция преобразующая данные исходного файла в текстовый файл
void from_bin_to_txt(LPCWSTR BinFileName, LPCWSTR TxtFileName)
{
    HANDLE hFileB = CreateFile(BinFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // открытие на чтение
    HANDLE hFileT = CreateFile(TxtFileName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // открытие на запись
    if (INVALID_HANDLE_VALUE == hFileB || INVALID_HANDLE_VALUE == hFileT)
    {
        throw runtime_error("Не удалось открыть файл");
    }
    else
    {
        DWORD dwBytes;
        DWORD computer_count = GetFileSize(hFileB, nullptr)/sizeof(computer);
        // чтение из бинарного файла и запись в текстовый
        for (DWORD i = 0; i < computer_count; ++i)
        {
            computer comp;
            ReadFile(hFileB, &comp, sizeof(computer), &dwBytes, nullptr);
            for(char a : computer_to_string(comp))
            {
                WriteFile(hFileT, &a, sizeof(char), &dwBytes, nullptr);
            }
        }
        CloseHandle(hFileB);
        CloseHandle(hFileT);
    }
}

const computer prov1 = {37, "Asus", "Intel Core i7-8700K", 4500, 8192, 524288, 2048, 52370.8, 3}; // для проверки тестовые значения
const computer prov2 = {43256, "Acer", "AMD Ryzen 5 3600", 4300, 4096, 327680, 1024, 43999.9, 8};

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №13. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;

    cout << "№1. Первичный ввод значений в файл" << endl;
    initial_input(L"example.bin");
    system("pause");

    cout << "№2. Операция добавления" << endl;
    add_to_file(L"example.bin", prov1);
    add_to_file(L"example.bin", prov2);
    from_bin_to_txt(L"example.bin", L"output.txt");
    system("notepad.exe output.txt");
    system("pause");

    cout << "№3. Поиск по типу процессора" << endl;
    find_by_cpu(L"example.bin", "Intel Core i7-8700K");
    system("notepad.exe search.txt");
    system("pause");

    cout << "№4. Поиск по объему озу" << endl;
    find_by_ram(L"example.bin", 8192);
    system("notepad.exe search.txt");
    system("pause");

    cout << "№5. Поиск по объему жесткого диска" << endl;
    find_by_hdd(L"example.bin", 524288);
    system("notepad.exe search.txt");
    system("pause");

    cout << "№6. Поиск по объему видеопамяти" << endl;
    find_by_videomemory(L"example.bin", 1024);
    system("notepad.exe search.txt");

    cout << "№7. Удаление значений" << endl;
    delete_from_file(L"example.bin", prov1);

    from_bin_to_txt(L"example.bin", L"output.txt");
    system("notepad.exe output.txt");
    system("pause");

    cout << "№8. Замена значений" << endl;
    replace_in_file(L"example.bin", prov2, prov1);

    from_bin_to_txt(L"example.bin", L"output.txt");
    system("notepad.exe output.txt");
    system("pause");
}
