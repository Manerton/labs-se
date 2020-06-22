#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <windows.h>

using namespace std;

struct string_with_pos // строка в файле с номером (позицией в файле)
{
    string value = "";
    size_t pos = 0;
};

using text_from_file = vector<string_with_pos>; // текст из файла - вектор строк с номерами

struct finded_operation // найденная операция и где она была найдена впервые
{
    string operation = "";
    size_t pos = 0;
};

using result_vector = vector<finded_operation>; // найденные операции, которые надо вывести в выходной файл

vector<string> all_operations; // глобальный вектор с перечислением всех
// заполнение операций в вектор
void fill_operations()
{
    // арифметические
    all_operations.push_back("+");
    all_operations.push_back("-");
    all_operations.push_back("*");
    all_operations.push_back("/");
    all_operations.push_back("%");
    all_operations.push_back("++");
    all_operations.push_back("--");
    // сравнения
    all_operations.push_back("==");
    all_operations.push_back("!=");
    all_operations.push_back(">");
    all_operations.push_back("<");
    all_operations.push_back(">=");
    all_operations.push_back("<=");
    // логические
    all_operations.push_back("!");
    all_operations.push_back("&&");
    all_operations.push_back("||");
    // побитовые
    all_operations.push_back("~");
    all_operations.push_back("&");
    all_operations.push_back("|");
    all_operations.push_back("^");
    all_operations.push_back("<<");
    all_operations.push_back(">>");
    // присваивание
    all_operations.push_back("=");
    all_operations.push_back("+=");
    all_operations.push_back("-=");
    all_operations.push_back("*=");
    all_operations.push_back("/=");
    all_operations.push_back("%=");
    all_operations.push_back("&=");
    all_operations.push_back("|=");
    all_operations.push_back("^=");
    all_operations.push_back("<<=");
    all_operations.push_back(">>=");
}

// чтение каждой строки в вектор строк text_from_file
text_from_file read_strings_from_file(LPCWSTR filename)
{
    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // открываем на чтение
    text_from_file result;
    if (INVALID_HANDLE_VALUE == hFile)
    {
        cout << "Не удалось открыть файл";
    }
    else
    {
        string_with_pos tmp_s;
        DWORD FileSize = GetFileSize(hFile, NULL);               // количество символов в файле (его размер)
        char buffer;
        DWORD BytesRead = 0, CountOfSymbolsRead = 0;
        size_t pos = 1;
        do                                                       // читаем весь файл посимвольно, пока не прочтем весь
        {
            ReadFile(hFile, &buffer, 1, &BytesRead, 0);          // читаем посимвольно
            ++CountOfSymbolsRead;
            if (buffer != '\n') tmp_s.value += buffer;           // прибавляем прочитанный символ к строке
            else                                                 // если достигнута новая строка
            {
                tmp_s.pos = pos++;                               // записываем номер строки
                result.push_back(tmp_s);
                tmp_s.value = "";
            }
        } while (CountOfSymbolsRead < FileSize);
    }
    return result;
}

// открытие файла в блокноте
inline void open_file()
{
    system("notepad.exe result.txt");
}

// запись в файл результата поиска
void write_in_file(const result_vector& result)
{
    HANDLE hFile = CreateFile(L"result.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // открываем на запись
    if (INVALID_HANDLE_VALUE == hFile)
    {
        cout << "Не удалось создать выходной файл";
    }
    else
    {
        DWORD CountOfSymbolsWritten = 0;
        for (const finded_operation& c : result) // каждую запись проходим
        {
            stringstream info;
            info << c.pos << ' ' << c.operation << ' ' << endl;
            for (char a : info.str())
            {
                WriteFile(hFile, &a, 1, &CountOfSymbolsWritten, NULL); // посимвольная запись в файл
            }
        }
        open_file(); // открытие выходного файла блокнотом
    }
}

// проверяем операторы <>, что это не скобки для #include
bool check_op_is_include(const string &s)
{
    return (s[0] == '#');
}
// проверяем оператор "/", чтобы он не оказался составляющей комментария c++ "//"
bool check_op_is_comment(const string &s, size_t pos)
{
    return (s[++pos] == '/');
}
// проверяем оператор "=", чтобы он не оказался оператором "=="
bool check_op_is_equal(const string &s, size_t pos)
{
    return (s[pos-1] == '=' || s[pos+1] == '=');
}
// проверяем, оператор найден в комментарии или нет
bool check_op_is_in_comment(const string &s, size_t pos)
{
    size_t comment_sign_pos = s.find("//");
    return (comment_sign_pos != string::npos && pos > comment_sign_pos);
}
// проверяем операторы таким образом, чтобы к примеру не принять оператор "+=" за оператор "+"
bool check_similar_ops(const string &s, const string &op, size_t pos)
{
    bool check = false;
    vector<string> chars_ops = {"+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>", ">", "<", "!"};
    if (op == "=")  // проверяем, чтобы это было обычное присваивание, а не составное
    {
        check += (find(chars_ops.begin(), chars_ops.end(),string{s[pos-1]}) != chars_ops.end());
    }
    if (op == "+")  // проверяем чтобы это был унарный плюс, а не инкремент
    {
         check += (s[pos+1] == '+');
    }
    if (op == "-")  // проверяем чтобы это был унарный минус, а не декремент
    {
         check += (s[pos+1] == '-');
    }
    if (op == "<")  // проверяем чтобы это была операция сравнения, а не сдвиг
    {
         check += (s[pos+1] == '<');
    }
    if (op == ">")  // проверяем чтобы это была операция сравнения, а не сдвиг
    {
         check += (s[pos+1] == '>');
    }
    if (op == "&")
    {
         check += (s[pos+1] == '&');
    }
    if (find(chars_ops.begin(), chars_ops.end(), op) != chars_ops.end()) // если это одна из операций из строки chars_ops
    {
        // проверяем, чтобы это была именно эта операция, а не сост. присваивание
        check += (s[pos+1] == '=');
    }
    return check;
}
// ищем операции и запоминаем где они находятся
result_vector find_operations(text_from_file &text)
{
    result_vector result;

    for (const string_with_pos& s : text)                               // в каждой строке текста
    {
        for (const string& op : all_operations)                         // перебираем все операторы
        {
            size_t operation_pos = s.value.find(op);                    // ищем операторы в строке

            if (operation_pos != string::npos)                          // если вообще были найдены такие символы
            {
                bool checks   = (((op == "<" || op == ">") && check_op_is_include(s.value))                 // если операции <> это не скобки для #include
                              || ( op == "/"               && check_op_is_comment(s.value,operation_pos))   // если операция "/" это не комментарий
                              || ( op == "="               && check_op_is_equal(s.value, operation_pos))
                              || check_op_is_in_comment(s.value,operation_pos)
                              || check_similar_ops(s.value,op,operation_pos));  // проверяем, это операция присваивания или сравнения

                if (!checks)
                {
                    result.push_back(finded_operation{op,s.pos});
                }
            }
        }
    }

    return result;
}

int main(int argc, char* argv[])
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №11. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    if (argc > 1)
    {
        wstringstream wss;  // преобразуем имя файла из char* в wchar_t* для открытия файла через WinAPI
        wss << argv[1];
        text_from_file text = read_strings_from_file(wss.str().c_str()); // читаем строки и их позиции из файла
        fill_operations();
        write_in_file(find_operations(text)); // запись найденного в файл
    }
    else cout << "Программа была запущена без аргументов." << endl;

    system("pause");
    return 0;
}
