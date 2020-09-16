#include <iostream>
#include <fstream>
#include <conio.h> // для getch()
#include <sstream> // строчный текстовый поток
#include "color.h" // цветной текст
#include "teo.h"

//Чтение теории из файла

void ShowTeoHelp()
{
    SetColor(Blue,White);
    cout << "Вы выбрали режим чтения теории: \n" << endl;
    SetColor(Black,White);
    cout << "Нажмите '->', для перехода к следующей странице" << endl;
    cout << "Нажмите '<-', для перехода к предыдущей странице" << endl;
    cout << "Нажмите '\\/', если вы хотите закончить чтение теории и вернуться в меню\n" << endl;
    system("pause");
    system("cls");
}

void creation_text (text &text){
    ifstream input("files\\teo.dat");
    if(input)
    {
        string temp_string; // текущая строка
        string all_string; // весь текст в одной строке
        page temp_page;
        while (getline(input, temp_string)) // расшифровываю текст из зашифрованного файла и записываю его в общую строку
        {
            if (!input.eof()) temp_string += "\n"; // вставляю \n так как Getline его не захватил
            for (char& i : temp_string) i ^= 83; // расшифровываю
            all_string += temp_string;
        }

        stringstream ss(all_string); // создаю текстовый поток из all_string
        while (getline(ss, temp_string)) // делю общую строку на строки (так как в общей строке были символы \n)
        {
            if (temp_string.find("[next_page]") != string::npos)
            {
                text.push_back(temp_page);
                temp_page.clear();
                //continue;
            } else temp_page.push_back(temp_string);
        }
    } else cout <<"Файл не обнаружен!" << endl;

}

// Вывод одной страницы вместе с выделением цветом слов
void one_page_display (const page &onepage)
{
    string blank(26, ' ');
    size_t N = onepage.size();
    cout << endl;
    for (size_t i = 0; i < N; i++)
    {
        cout << blank;

        size_t pos = onepage[i].find("[color]"); // ищу позицию следующего тега

        if (pos != string::npos) // Если найден тег [color]...[/color] начинаю выводить строку посимвольно
        {
            size_t strlen = onepage[i].length();
            for (size_t j = 0; j < strlen; j++)
            {
                if (j == pos)
                {
                    j += 7; // перепрыгиваю тег, чтобы его не вывести
                    pos = onepage[i].find("[/color]", pos); // ищу позицию закрывающего тега
                    SetColor(Green, White); // включаю синий цвет
                    while (j < pos && j < strlen) // вывожу выделенное цветом слово
                    {
                        cout << onepage[i][j];
                        j++;
                    }
                    SetColor(Black, White); // включаю стандартный цвет
                    j += 8; // перепрыгиваю тег, чтобы его не вывести
                    pos = onepage[i].find("[color]", pos); // ищу позицию следующего открывающего тега в строке
                }
                cout << onepage[i][j];
            }
        } else cout << onepage[i]; // Иначе просто вывод строки

        cout << endl;
    }
    cout << endl;
}

// Вывод страниц
void teo_display (text TEXT){
    string blank(26, ' ');
    string blank2(54, ' ');
    string lower(59, ' ');
    ShowTeoHelp();
    int page_num = 0, ch = 0;
    int size = TEXT.size();
    while (page_num != size && ch == 0)
    {
        cout << blank;
        SetColor(White,Green);
        cout << blank2 <<"["<< page_num+1 << "/" << size << "]" << endl;
        SetColor(Black,White);
        one_page_display(TEXT[page_num]);
        cout << blank;
        SetColor(White,Green);
        if (page_num != 0) cout << "<-";
        else cout << "  ";
        cout << blank << " \\/" << blank;
        if (page_num != size-1) cout << "->";
        else cout << "  ";
        SetColor(Black,White);

        switch(getch())
        {
        case 77:
            if (page_num != size-1) page_num++;
            break;
        case 75:
            if (page_num != 0) page_num--;
            break;
        case 80:
            ch = 1;
            break;
        }
        system("cls");
    }
}

// Шифрование
void crypt (string filename)
{
    ifstream in("files\\" + filename + ".txt");
    if(in)
    {
        cout << "Шифрую файл " << filename + ".txt..." << endl;
        ofstream out("files\\" + filename + ".dat", ios::binary);
        string temp;
        while (getline(in, temp)) // читаем по строке в отформатированном документе
        {
            if (!in.eof()) temp += "\r\n"; // записываем в строку символы форматирования
            for (char &i : temp) i ^= 83; // а потом уже зашифровываем строку
            out.write(temp.data(), temp.size());
        }
        cout << "Файл был успешно зашифрован и сохранен под именем " << filename + ".dat" << endl;
    } else cout << "Файл не обнаружен!" << endl;
}

// Расшифровка
void decrypt (string filename)
{
    ifstream in("files\\" + filename + ".dat");

    if(in)
    {
        cout << "Расшифровываю файл " << filename + ".dat..." << endl;
        ofstream out("files\\" + filename + ".txt", ios::binary);
        string temp;

        while (getline(in, temp)) // читаем строку (она может быть произвольной длины)
        {
            if (!in.eof()) temp += "\n"; // добавляем обратно символ \n в шифр, так как getline копирует строку игнорируя \n
            for (char &i : temp) i ^= 83; // расшифровываем
            out.write(temp.data(), temp.size());
        }
        cout << "Файл был успешно расшифрован и сохранен под именем " << filename + ".txt" << endl;
    } else cout << "Файл не обнаружен!" << endl;
}
