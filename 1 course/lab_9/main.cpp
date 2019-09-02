#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <random>
#include <ctime>
#include <sstream>

#define NOFILE ""

using namespace std;

void diapazon_rand (double &min, double& max)
{
        cout << "Задайте диапазон генерирования случайных чисел." << endl;
        cout << "Нижняя граница: ";
        cin >> min;
        cout << "Верхняя граница: ";
        cin >> max;
        while (min >= max)
        {
            cout << "error!" << endl;
            cout << "Верхняя граница: ";
            cin >> max;
        }
}

bool create_bin (int N, string bin_name, double min, double max)
{
    mt19937 gen; // создаю генератор случ чисел
    gen.seed(time(0));
    uniform_real_distribution<> urd(min,max); // указываю диапазон генерирования

    ofstream toBin(bin_name + ".bin", ios::binary);
    if (toBin)
    {
        for (int i = 0; i < N; i++)
        {
            double chislo = urd(gen);
            toBin.write((char*)&chislo, sizeof(double));
        }
        toBin.close();
        return true;
    }
    return false;

}

void output_bin (string bin_name, string txt_name)
{
    ifstream fromBin(bin_name + ".bin", ios::binary);
    if (fromBin)
    {
        double a;
        if (txt_name.length() != 0)
        {
            ofstream toTxt(txt_name + ".txt");
            if (toTxt)
            {
                while (fromBin.read((char*)&a, sizeof(double)))
                {
                    toTxt << a << " ";
                    cout << a << " ";
                }
                toTxt.close();
            }
        }
        else
            while (fromBin.read((char*)&a, sizeof(double)))
                cout << a << " ";
        fromBin.close();
    }
    cout << endl;
}

bool negative_chislo (double chislo)
{
    if (chislo < 0) return true;
    return false;
}

double find_fabs_max (string inputname)
{
    ifstream input(inputname + ".bin", ios::binary);
    double max, a;
    input.read((char*)&a, sizeof(double));
    max = fabs(a);
    while (input.read((char*)&a, sizeof(double)))
    {
        if (fabs(a) > max) max = fabs(a);
    }
    input.close();
    return max;
}

double find_fabs_min (string inputname)
{
    ifstream input(inputname + ".bin", ios::binary);
    double min, a;
    input.read((char*)&a, sizeof(double));
    min = fabs(a);
    while (input.read((char*)&a, sizeof(double)))
    {
        if (fabs(a) < min) min = fabs(a);
    }
    return min;
}

double find_sred_arif (double min, double max)
{
    return ( (min + max) / 2 );
}

void ex3 (string inputname, string outputname)
{
    ifstream input(inputname + ".bin", ios::binary);
    if (input)
    {
        double min, max, sred_arif;
        min = find_fabs_min(inputname);
        max = find_fabs_max(inputname);
        sred_arif = find_sred_arif(min,max);
        ofstream output(outputname + ".bin", ios::binary);
        if (output)
        {
            double a;
            while (input.read((char*)&a, sizeof(double)))
            {
                if (negative_chislo(a))
                {
                    a += sred_arif;
                }
                output.write((char*)&a, sizeof(double));
            }
        }
    }

}

void ex5 (string inputname, string outputname)
{
    ifstream output(outputname + ".bin", ios::binary);
    if (output)
    {
        ofstream input(inputname + ".bin", ios::binary | ios::app);
        if (input)
        {
            double a;
            while (output.read((char*)&a, sizeof(double)))
            {
                input.write((char*)&a, sizeof(double));
            }
        }
    }

}

void ex7 (string inputname)
{
    fstream input(inputname + ".bin", ios::binary|ios::in|ios::out);
    if (input)
    {
        double min, max, sred_arif;
        min = find_fabs_min(inputname);
        max = find_fabs_max(inputname);
        sred_arif = find_sred_arif(min,max);
        double a;
        while (input.read((char*)&a, sizeof(double)))
        {
            if (negative_chislo(a))
            {
                a += sred_arif;
                streampos pos = input.tellg();
                pos -= sizeof(double);
                input.seekp(pos);
                input.write((char*)&a, sizeof(double));
            }
        }
    }
}

void output_bin_ex8 (string bin_name, string txt_name)
{
    ifstream fromBin(bin_name + ".bin", ios::binary);
    if (fromBin)
    {
        double a;
        stringstream ss;
        string s;
        while (fromBin.read((char*)&a, sizeof(double)))
        {
            ss << a << "\n";
        }
        ofstream toTxt(txt_name + ".txt");
        if (toTxt)
        {
            toTxt << ss.str();
            toTxt.close();
        }
        cout << ss.str();
        fromBin.close();
    }
    cout << endl;
}

void ex9 (string txt_name, string bin_name)
{
    ifstream fromTxt(txt_name + ".txt");
    if (fromTxt)
    {
        double a;
        string s;
        stringstream ss;
        while (getline(fromTxt,s))
        {
            ss << s << " ";
        }
        ofstream toBin(bin_name + ".bin", ios::binary);
        if (toBin)
        {
            while (ss >> s)
            {
                a = stod(s);
                toBin.write((char*)&a, sizeof(double));
            }
            toBin.close();
        }
        fromTxt.close();
    }
    cout << endl;
}

bool filecompare(ifstream &first, ifstream &second)
{
    char ch1 = 0, ch2;
    while (ch1 != EOF)
    {
        ch1 = first.get();
        ch2 = second.get();
        if (ch1!=ch2)
            return false;
    }
    return true;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №9 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;

    //------------------Задание 1------------------//

    cout << "Задание №1. Создать бинарный файл, который будет считаться исходным (input.bin)." << endl;

    string bin_name;
    cout << "Введите название бинарного файла (без расширения .bin): ";
    cin >> bin_name;

    int N;
    cout << "Введите количество чисел N: ";
    cin >> N;
    while (N < 1 || N > 1000000)
    {
        cout << "error!" << endl;
        cout << "Введите количество чисел N: ";
        cin >> N;
    }

    double min, max;
    diapazon_rand(min,max);

    if (create_bin(N,bin_name,min,max)) cout << "Был создан бинарный файл input.bin" << endl;
    else cout << "Не удалось создать файл input.bin" << endl;
    system("pause");
    //------------------Задание 2------------------//
    cout << "Задание №2. Вывести содержимое исходного бинарного файла на экран и в текстовый файл (text1.txt)." << endl;
    string txt_name;
    cout << "Введите название текстового файла (без расширения .txt): ";
    cin >> txt_name;
    output_bin(bin_name, txt_name);
    system("pause");
    //------------------Задание 3------------------//
    cout << "Задание №3. Записать выходной бинарный файл (output.bin), добавить к каждому отрицательному числу среднее арифметическое наименьшего по абсолютной величине и наибольшего из чисел файла." << endl;
    cout << "Введите название выходного бинарного файла (без расширения .bin): ";
    string ex3_bin_name;
    cin >> ex3_bin_name;
    ex3(bin_name, ex3_bin_name);
    system("pause");
    //------------------Задание 4------------------//
    cout << "Задание №4. Вывести содержимое выходного бинарного файла на экран и в файл (text2.txt)." << endl;
    cout << "Введите название текстового файла (без расширения .txt): ";
    cin >> txt_name;
    output_bin(ex3_bin_name, txt_name);
    system("pause");
    //------------------Задание 5------------------//
    cout << "Задание №5. Дописать к содержимому бинарного файла input.bin содержимое бинарного файла output.bin." << endl;
    ex5(bin_name, ex3_bin_name);
    system("pause");
    //------------------Задание 6------------------//
    cout << "Задание №6. Вывести содержимое полученного бинарного файла на экран и в файл. (text3.txt)" << endl;
    cout << "Введите название текстового файла (без расширения .txt): ";
    cin >> txt_name;
    output_bin(bin_name, txt_name);
    system("pause");
    //------------------Задание 7------------------//
    cout << "Задание №7. Выполнить перезапись по месту бинарного файла, полученного в результате выполнения задания №5 (input.bin) в соответствии с заданием своего варианта,";
    cout << " (перезапись по месту означает, что из файла читается очередное число; если оно не подлежит замене, то процесс чтения продолжается; если число подлежит замене,";
    cout << " то необходимо записать на его место требуемое число; дополнительные файлы не использовать)." << endl;
    ex7(bin_name);
    system("pause");
    //------------------Задание 8------------------//
    cout << "Задание №8. Вывести содержимое полученного бинарного файла (input.bin) на экран и в выходной текстовый файл (text4.txt). Выходное число преобразуется в строку с использованием строкового потока." << endl;
    cout << "Введите название текстового файла (без расширения .txt): ";
    cin >> txt_name;
    output_bin_ex8("input", "text4");
    system("pause");
    //------------------Задание 9------------------//
    cout << "Задание №9. Содержимое текстового файла (text4.txt), полученного в результате выполнения задания № 8 переписать в бинарный файл (out2.bin). Входное число считывается как строка и преобразуется во внутренний формат с помощью входного строкового потока." << endl;
    cout << "Введите название выходного бинарного файла (без расширения .bin): ";
    string ex9_bin_name;
    cin >> ex9_bin_name;
    ex9(txt_name, ex9_bin_name);
    system("pause");
    //-----------------Задание 10------------------//
    cout << "Задание №10. Вывести содержимое полученного бинарного файла (out2.bin) на экран и в файл." << endl;
    cout << "Введите название текстового файла (без расширения .txt): ";
    cin >> txt_name;
    output_bin(ex9_bin_name, txt_name);
    system("pause");
    //-----------------Задание 11------------------//
    cout << "Задание №11. Написать функцию для сравнения файлов и проверить, совпал ли файл, полученный в результате выполнения задания 9 (out2.bin) с файлом input.bin." << endl;
    ifstream first ("out2.bin", ios::binary);
    ifstream second("input.bin", ios::binary);
    if (filecompare(first, second)) cout<<"Files are equals!"<<endl;
    else cout << "Files are not equals!" << endl;
    return 0;
}
