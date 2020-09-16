#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "color.h"
#include "demo.h"

using namespace std;

typedef vector<int> row;
typedef vector<row> matrix;

int N; // глобальный размер массива

vector<int> mas; // исходный массив
int statesCount = 0; // счетчик кол-ва состояний, количество шагов сортировки

matrix states; // все состояния массива при сортировке; (Кол-во строк - сколько получится, кол-во столбцов N)
matrix counterStates; // элементы I и J каждого состояния, значение d и цвет выделения

int state; //шаги состояния (исп. в ф-ях NextStep и PreviousStep)

void GenerateMas() // генерируем массив;
{
    srand(time(NULL));
    mas.resize(N);
    for(int i=0; i < N; i++)
    {
        mas[i] = rand() % 101 - 50; //квазислучайные числа от -50 до 50;
    }
}

void GenerateMasFromKeyboard() // генерируем массив;
{
    cout << "Задайте значения элементов вектора: " << endl;
    mas.resize(N);
    for (int i = 0; i < N; i++)
    {
        cout << "Для " << i+1 << "-го: ";
        cin >> mas[i];
        while (mas[i] > 50 || mas[i] < -50)
        {
            cout << "Вы ввели число выходящее за диапазон [-50;50]" << endl;
            cout << "Для " << i+1 << "-го: ";
            cin >> mas[i];
        }

    }
}

bool GenerateMasFromFile(string filename) // генерируем массив;
{
    ifstream input("files\\" + filename + ".txt");
    if (input) // проверка открытия
    {
        mas.resize(N);
        for (int i = 0; i < N; i++)
        {
            input >> mas[i];
        }
        input.close();
        return true;
    } else return false;
}

void sposob_zapolneniya ()
{
    int choice;
    cout << "*** Выбор способа заполнения ***" << endl;
    cout << "1. Заполнение с клавиатуры." << endl;
    cout << "2. Заполнение случайными числами." << endl;
    cout << "3. Заполнение из файла." << endl;
    cout << "Выберите способ заполнения: ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "Ошибка!" << endl;
        cout << "Выберите способ заполнения: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        GenerateMasFromKeyboard();
        break;
    }
    case 2:
    {
        GenerateMas();
        break;
    }
    case 3:

        string filename;
        cout << "Введите название файла без расширения txt: ";
        cin >> filename;
        while (!GenerateMasFromFile(filename))
        {
            cout << endl << "Ошибка! Не удалось открыть файл " << filename << endl;
            cout << "Введите название файла без расширения txt: ";
            cin >> filename;
            GenerateMasFromFile(filename);
        }
        break;
    }
    system("cls");
}

void Init(int input_N) //инициализация - генерация массива и сортировка;
{
    N = input_N;
    sposob_zapolneniya();
    Sort(); // сортируем
}

void SaveState(int d, int I, int J, int color) //функция, сохраняющая состояние массива;
{

    states.push_back(mas);
    row temp (4,0);
    temp[0] = d; // записываем подсвечиваемый элемент I этого состояния
    temp[1] = I; // записываем подсвечиваемый элемент J этого состояния
    temp[2] = J; // цвет
    temp[3] = color; // цвет
    counterStates.push_back(temp);

    statesCount++;  //Увеличение счетчика переменной счетчика состояния
}

void Sort() // сортировка и сохранение состояний массива
{
    int d = N/2;
    SaveState (d, 0, 0, LightGray);
    while (d > 0)
    {
        for(int i=0; i<N-d; i++)
        {
            int j = i;

            SaveState (d,j,j+d,Green);

            while(j >= 0 && mas[j] > mas[j+d])
            {
                int temp = mas[j];
                mas[j] = mas[j+d];
                mas[j+d] = temp;

                SaveState (d,j,j+d,Yellow);

                j--;

                if (d == 1)
                {
                   SaveState (d,j,j+d,Green);
                }
            }
        }
        d /= 2;
    }
}

void PreviousStep() //Наш массив на предыдущем шаге сортировки;
{
    if(state > 0)
        state--;
}

void NextStep()    //Наш массив на следующем шаге сортировки;
{
    if(state < statesCount-1)
        state++;
}


void ShowDefaultState() // показывает массив на текущем шаге;
{

    int d = counterStates[0][0];

    SetColor(LightGreen,Black);
    cout << "Исходный массив: " << endl;
    SetColor(LightGray,Black);

    for(int i=0; i < N; i++) cout << states[0][i] << " ";

    cout << "\n\nТекущий шаг d: " << d << endl;

    SetColor(LightGreen,Black);
    cout << "\nТекущий массив: " << endl;
    SetColor(LightGray,Black);
    for(int i=0; i < N; i++) cout << states[0][i] << " ";


}

void ShowState() // вывод состояния массива (шаг сортировки)
{

    string blank(50, ' ');
    int d = counterStates[state][0];
    int I = counterStates[state][1];
    int J = counterStates[state][2];
    int color = counterStates[state][3]; // цвет

    gotoXY(0,12);
    if (d == 1)
    {
        cout << "Так как значение d = 1, переходим к";
        SetColor(LightGreen,Black);
        cout << " сортировке вставками." << endl;
        SetColor(LightGray,Black);
    } else
    {
        cout << "Текущий шаг d: " << d << blank << endl;
    }


    gotoXY(0,15);
    for(int i=0; i < N; i++)
    {
        if((i==I)||(i==J))
        {
            if (color == Yellow) SetColor(Black,Yellow);
            else SetColor(White,Green);
        }
        if ((state == 0) || (state == statesCount-1)) SetColor(LightGray,Black);
        cout << states[state][i];
        SetColor(LightGray,Black); //Установить "нормальный" цвет текста;

        cout << " ";
    }

    if(state == statesCount-1)
    {
        cout << endl;
        SetColor(Green,Black);
        cout << "\nМассив отсортирован (Array is sorted)" << endl;
        SetColor(LightGray,Black);
        cout << "\nНажмите 's', если вы хотите сохранить в файл все шаги сортировки" << endl;
        cout << "Нажмите '\\/' для возврата в Главное меню..." << endl;
    }

}

// демонстрация
void Demonstrate(int input_N)
{
    ShowHelp(); // инструкция
    Init(input_N); // ввод размера N от 5 до 15
    system("cls");
    ShowHelp();
    ShowDefaultState(); // показать изначальное состояние
    bool working = true;
    while(working)
    {
        switch(getch())
        {
        case 77:
            NextStep();
            break;
        case 75:
            PreviousStep();
            break;
        case 's':
            if (state == statesCount-1)
            {
                SaveToFile();
                cout << "\nПроизведено сохранение всех шагов сортировки в файл sortsteps.txt\n" << endl;
            }
            break;
        case 80:
            working = false;
            break;
        }

        ShowState(); // массив на текущем шаге сортировки
    }
    states.clear(); // очистка для следующий сеансов демонстраций
	counterStates.clear();
	state = 0;
	statesCount = 0;
}

void SaveToFile()
{
    ofstream fout("files\\sortsteps.txt");

    for(int i=0; i<statesCount; i++)      //номер состояния
    {
        for(int j=0; j<N; j++)         //номер элемента
            fout << states[i][j] << " ";  //запись в файл;
        fout << endl;
    }
    fout.close();
}

void ShowHelp()
{
    system ("cls");
    SetColor(LightGreen,Black);
    cout << "Вы выбрали режим демонстрации сортировки Шелла: \n" << endl;
    SetColor(LightGray,Black);
    cout << "Нажмите '->', для вывода массива по шагам" << endl;
    cout << "Нажмите '<-', для возврата предыдущего шага сортировки массива" << endl;
    cout << "Нажмите '\\/', если вы хотите закончить сортировку\n" << endl;
    SetColor(White,Green);
    cout << "Зеленый цвет";
    SetColor(LightGray,Black);
    cout << ": сортируемые в данный момент элементы." << endl;
    SetColor(Black,Yellow);
    cout << "Жёлтый цвет";
    SetColor(LightGray,Black);
    cout << ": отсортированные элементы.\n" << endl;
}

