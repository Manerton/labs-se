#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

struct person
{
    string name;
    string adres;
    int age;
};

void Read(person *X, int N, string filename)
{
    ifstream input(filename);
    if (input)
    {
        for(int i=0; i<N; i++)
        {
            person temp;
            input >> temp.name;
            input >> temp.adres;
            input >> temp.age;
            X[i]=temp;
        }
        input.close();
    } else cout << "Не удалось открыть файл " << filename;

}

void Read(vector<person> &X, int N, string filename)
{
    ifstream input(filename);
    if (input)
    {
        for(int i=0; i<N; i++)
        {
            person temp;
            input >> temp.name;
            input >> temp.adres;
            input >> temp.age;
            X.push_back(temp);
        }
        input.close();
    } else cout << "Не удалось открыть файл " << filename;

}

void Display(person *X, int N)
{
    cout << "=================================================\n";
    for (int i=0; i<N; i++)
    {
        cout << i+1 << ". "; // номер записи
        cout << "Имя: " << X[i].name << " | ";
        cout << "Адрес: " << X[i].adres << " | ";
        cout << "Возраст: " << X[i].age << endl;
    }
    cout << "=================================================\n";
}

void Display(const vector<person> &X)
{
    int N = X.size();
    cout << "=================================================\n";
    for (int i=0; i<N; i++)
    {
        cout << i+1 << ". "; // номер записи
        cout << "Имя: " << X[i].name << " | ";
        cout << "Адрес: " << X[i].adres << " | ";
        cout << "Возраст: " << X[i].age << endl;
    }
    cout << "=================================================\n";
}

// далее приведены две функции: для сохранения структуры в бинарный файл
//                              для чтения структуры из бинарного файла
// стоит отметить, что из-за присутствия в структуре переменных типа string возникают следующие моменты
// сохранение структуры в бинарный файл будет работать таким образом,
// что прочесть бинарный файл можно будет только в том же сеансе работы программы, когда был создан бинарный файл
// это происходит из-за того, что переменная типа string не имеет определенного размера
// если создать бинарный файл и в том же сеансе его прочесть, то эта операция пройдет успешно, так как размер string будет определен при создании и будет использоваться при чтении
// если создать бинарный файл, а потом в другом сеансе попробовать его прочесть, то операция пройдет безуспешно, так как программа не знает точного размера string
// это можно разрешить путем сохранения в файле длины строковых данных объекта string перед самим объектом string
// таким образом при чтении бинарного файла программа будет знать размер каждой строки
// однако в данной лабораторной работе этого не будет реализовано (перманентного хранения строк с переменной длиной в бинарном файле для последующего чтения в других сеансах работы программы)

void ToBinFile(person *X, int N, string filename)
{
    ofstream toBin(filename, ios::binary);
    if(toBin)
    {
        for(int i=0; i<N; i++)
        {
            toBin.write((char*)&X[i], sizeof(person));
        }
        toBin.close();
    } else cout << "Не удалось создать файл " << filename;
}

void ToBinFile(const vector<person> &X, string filename)
{
    int N = X.size();
    ofstream toBin(filename, ios::binary);
    if(toBin)
    {
        for(int i=0; i<N; i++)
        {
            toBin.write((char*)&X[i], sizeof(person));
        }
        toBin.close();
    } else cout << "Не удалось создать файл " << filename;
}

void FromBinFile(string filename)
{
    ifstream fromBin(filename, ios::binary);
    if(fromBin)
    {
        cout << "=================================================\n";
        person temp;
        int i = 0;
        while ( fromBin.read((char*)&temp, sizeof(person)))
        {
            cout << i+1 << ". "; // номер записи
            cout << "Имя: " << temp.name << " | ";
            cout << "Адрес: " << temp.adres << " | ";
            cout << "Возраст: " << temp.age << endl;
            i++;
        }
        cout << "=================================================\n";
        fromBin.close();
    } else cout << "Не удалось открыть файл " << filename;
}

void ex5(string filename, char bukva, int age)
{
    ifstream fromBin(filename, ios::binary);
    ofstream toTxt("Search.txt");
    int count = 0;
    if(fromBin && toTxt)
    {
        person temp;
        while (fromBin.read((char*)&temp, sizeof(person)))
        {
            if ((temp.name[0] == bukva) && temp.age >= age)
            {
                toTxt << temp.name << " " << temp.adres << " " << temp.age << endl;
                count++;
            }
        }
        fromBin.close();
        toTxt.close();
    } else cout << "error!";
    if (count == 0) cout << "В бинарном файле требуемые элементы не найдены." << endl;
}

void ex6(int N, char bukva, int age)
{
    ifstream fromTxt("In.txt");
    ofstream toBin("Search.bin", ios::binary);
    int count = 0;
    if(fromTxt && toBin)
    {
        person temp;
        for (int i = 0; i < N; i++)
        {
            fromTxt >> temp.name;
            fromTxt >> temp.adres;
            fromTxt >> temp.age;
            if ((temp.name[0] == bukva) && temp.age >= age)
            {
                toBin.write((char*)&temp, sizeof(person));
                count++;
            }
        }
        fromTxt.close();
        toBin.close();
    } else cout << "error!";
    if (count == 0) cout << "В текстовом файле требуемые элементы не найдены." << endl;
}

void sort_arr (person *X, int N)
{
    for (int i = 0; i != N-1; i++)
    {
        for (int j = i+1; j != N; j++)
        {
           if (X[i].adres[0] > X[j].adres[0]) swap(X[i],X[j]);  // сортирую
        }
    }
}

void sort_arr (vector<person> &X)
{
    int N = X.size();
    for (int i = 0; i != N-1; i++)
    {
        for (int j = i+1; j != N; j++)
        {
           if (X[i].adres[0] > X[j].adres[0]) swap(X[i],X[j]);  // сортирую
        }
    }
}

void ex7(person *X, int N)
{
    ofstream toTxt("Out.txt");
    if(toTxt)
    {
        sort_arr(X,N);
        for(int i=0; i<N; i++)
        {
            toTxt << X[i].name << " " << X[i].adres << " " << X[i].age << endl;
        }
        toTxt.close();
    } else cout << "Не удалось создать файл Out.txt" << endl;
}

void ex7(vector<person> &X)
{
    int N = X.size();
    ofstream toTxt("Out.txt");
    if(toTxt)
    {
        sort_arr(X);
        for(int i=0; i<N; i++)
        {
            toTxt << X[i].name << " " << X[i].adres << " " << X[i].age << endl;
        }
        toTxt.close();
    } else cout << "Не удалось создать файл Out.txt" << endl;
}

int check_future_size (string filename)
{
    int N = 0;
    ifstream input(filename);
    if(input)
    {
        string temp;
        while (getline(input,temp))
        {
            N++;
        }
        input.close();
    } else cout << "Не удалось создать файл Out.txt" << endl;
    return N;
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №10 Вариант 1 (13).\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    //------------------Задание 1------------------//
    cout << "1. Сформировать динамический массив из элементов структурного типа. Структурный тип определен в варианте 1. Данные получить из заранее подготовленного файла In.txt, содержащего не менее 15 записей в соответствующем формате." << endl;
    int N;
    cout<<"Введите количество записей: ";
    cin>>N;
    while (N < 1 || N > 15)
    {
        cout << "error!" << endl;
        cout<<"Введите количество записей: ";
        cin>>N;
    }
    person *M;
    M = new person[N];
    Read(M,N, "In.txt");
    //------------------Задание 2------------------//
    cout << "2. Вывести в файл Out.bin элементы полученного массива." << endl;
    cout<<"Введите название бинарного файла (вместе с расширением .bin): ";
    string filename;
    cin >> filename;
    ToBinFile(M,N,filename);
    system("pause");
    system("cls");
    //------------------Задание 3------------------//
    cout << "3. Прочитать из бинарного файла элементы структурного типа в соответствии с вариантом 1. Вывести на экран содержимое файла Out.bin." << endl;
    FromBinFile(filename);
    system("pause");
    system("cls");
    //------------------Задание 4------------------//
    cout << "4. Вывести массив, полученный в результате выполнения задания 1 на экран. Функция вывода должна нумеровать записи, начиная с 1." << endl;
    Display(M,N);
    system("pause");
    system("cls");
    //------------------Задание 5------------------//
    cout << "5. Поиск записей в бинарном файле, удовлетворяющих заданным условиям. Найденные записи вывести в текстовый файл Search.txt." << endl;
    cout << "Условия: Имена начинаются на заданную букву и возраст не менее заданного" << endl;
    char bukva;
    int age;
    cout << "Введите заданную букву: ";
    cin >> bukva;
    cout << "Введите возраст: ";
    cin >> age;
    ex5("out.bin", bukva, age);
    system("pause");
    system("cls");
    //------------------Задание 6------------------//
    cout << "6. Поиска записей в текстовом файле, удовлетворяющих заданным условиям. Найденные записи вывести в бинарный файл Search.bin." << endl;
    cout << "Условия: Имена начинаются на заданную букву и возраст не менее заданного" << endl;
    cout << "Введите заданную букву: ";
    cin >> bukva;
    cout << "Введите возраст: ";
    cin >> age;
    ex6(N,bukva,age);
    system("pause");
    system("cls");
    //------------------Задание 7------------------//
    cout << "7. Обработка массива в соответствии с заданием своего варианта. Преобразовать массив и результаты вывести в файл Out.txt. Удалить массив." << endl;
    cout << "Задание: Отсортировать по возрастанию поля adres" << endl;
    ex7(M,N);
    delete[] M;
    system("pause");
    system("cls");
    //------------------Задание 8------------------//
    cout << "8. Заполнить динамический массив элементами файла Search.txt. Преобразовать массив в соответствии с заданием своего варианта и результаты вывести на экран. Удалить массив." << endl;
    cout << "Задание: Отсортировать по возрастанию поля adres" << endl;
    int N_ex8 = check_future_size("Search.txt");
    M = new person[N_ex8];
    Read(M,N_ex8,"Search.txt");
    sort_arr(M,N_ex8);
    Display(M,N_ex8);
    delete[] M;
    system("pause");
    system("cls");
    //------------------Задание 9------------------//
    cout << "9. Реализовать задания 1, 2, 4, 7, 8 с использованием вектора (а не динамического массива)." << endl;
    //-----------------Задание 9_1-----------------//
    cout << "1. Сформировать вектор из элементов структурного типа. Структурный тип определен в варианте 1. Данные получить из заранее подготовленного файла In.txt, содержащего не менее 15 записей в соответствующем формате." << endl;
    vector<person> V;
    Read(V,N,"In.txt");
    //-----------------Задание 9_2-----------------//
    cout << "2. Вывести в файл Out.bin элементы полученного вектора." << endl;
    ToBinFile(V,filename);
    system("pause");
    system("cls");
    //-----------------Задание 9_4-----------------//
    cout << "4. Вывести вектор, полученный в результате выполнения задания 1 на экран. Функция вывода должна нумеровать записи, начиная с 1." << endl;
    Display(V);
    system("pause");
    system("cls");
    //-----------------Задание 9_7-----------------//
    cout << "7. Обработка вектора в соответствии с заданием своего варианта. Преобразовать вектор и результаты вывести в файл Out.txt. Удалить вектор." << endl;
    cout << "Задание: Отсортировать по возрастанию поля adres" << endl;
    ex7(V);
    V.clear();
    system("pause");
    system("cls");
    //-----------------Задание 9_8-----------------//
    cout << "8. Заполнить вектор элементами файла Search.txt. Преобразовать вектор в соответствии с заданием своего варианта и результаты вывести на экран. Удалить вектор." << endl;
    cout << "Задание: Отсортировать по возрастанию поля adres" << endl;
    N_ex8 = check_future_size("Search.txt");
    Read(V,N_ex8,"Search.txt");
    sort_arr(V);
    Display(V);
    V.clear();
    system("pause");
    system("cls");
    return 0;
}
