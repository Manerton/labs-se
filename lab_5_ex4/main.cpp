//ver 5 - 23.03.19

#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

void ex1_random (int N) // генерирование случайными числами
{
    ofstream input("input.txt"); // открываем выходной поток-объект
    if (input) // проверка открытия
    {
        input << N; // вводим в файл длину последовательности на 1 место
        int rand_min, rand_max;
        cout << "Задайте диапазон генерирования случайных чисел." << endl;
        cout << "Нижняя граница: ";
        cin >> rand_min;
        cout << "Верхняя граница: ";
        cin >> rand_max;
        while (rand_min >= rand_max)
        {
            cout << "error!" << endl;
            cout << "Верхняя граница: ";
            cin >> rand_max;
        }
        int ostatok = (rand_max - rand_min) + 1;
        int chislo;
        for (int i = 1; i <= N; i++)
        {
            chislo = rand_min + rand() % ostatok;
            input << " " << chislo;
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;
}

void ex1_from_keyboard (int N) // ввод с клавиатуры
{
    ofstream input("input.txt"); // открываем выходной поток-объект
    if (input) // проверка открытия
    {
        input << N; // вводим в файл длину последовательности на 1 место
        cout << "Задайте значения элементов последовательности: " << endl;
        int chislo;
        for (int i = 1; i <= N; i++)
        {
            cout << "Для " << i << "-го: ";
            cin >> chislo;
            input << " " << chislo;
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;
}

void ex1 () // первое задание
{
    ifstream input("input.txt"); // открываем выходной поток-объект
    if (input) // проверка открытия
    {
        int N;
        input >> N; // читаем длину последовательности
        cout << "*** Выполнение первого задания ***" << endl;
        bool zero_flag = false; // когда будет найден первый нулевой элемент - станет true
        int summa = 0; // сумма чисел после первого нулевого элемента
        int count_of_elem = 0; // кол-во чисел после первого нулевого элемента
        int chislo = -1; // чтобы zero_flag сразу не сработал
        for (int i = 1; i <= N; i++)
        {
            if (chislo == 0) zero_flag = true;
            input >> chislo;
            if (zero_flag)
            {
                summa += chislo;
                count_of_elem++;
            }
        }
        double sredne_arif;
        if (summa == 0) sredne_arif = 0; // исключаю деление на ноль, если в последовательности не было нулевого элемента
        else sredne_arif = double(summa) / count_of_elem;
        cout << "Результат выполнения 1 задания: " << sredne_arif << endl;
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;
}

void display_input ()
{
    ifstream input("input.txt"); // открываем выходной поток-объект
    if (input) // проверка открытия
    {
        cout << "*** Содержимое файла input.txt ***" << endl;
        int chislo;
        while (!input.eof())
        {
            input >> chislo;
            cout << chislo << " ";
        }
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;
    cout << endl;
}

bool dvyznachnoe_or_not (int A)
{
    int kolvo_deleniy(0); // если нацело разделилось один раз - число двузначное
    if (A < 0) A = -A; // так как по условию задачи нас интересуют все двузначные числа (отрицательные тоже)
    while ((A / 10) > 0 && kolvo_deleniy < 2) // если видим, что число разделилось нацело 2 раза, то дальше смысла делить нет
    {
        kolvo_deleniy++;
        A /= 10;
    }
    if (kolvo_deleniy == 1) // если нацело разделилось один раз - число двузначное
    {
        return true;
    }
    return false;
}

void ex2()
{
    ifstream input("input.txt"); // открываем текстовый файл для чтения
    if (input) // проверка открытия
    {
        int N;
        input >> N; // читаем длину последовательности
        int A;
        display_input();
        ofstream output("output.txt"); // открываем выходной поток для 2 задания
        if (output) // проверка открытия
        {
            cout << "*** Результат выполнения второго задания ***" << endl; // вывод на экран и в файл чисел из input удовл. условию 2 задачи
            for (int i = 1; i <= N; i++)
            {
                input >> A;
                if (dvyznachnoe_or_not(A))
                {
                    output << " " << A;
                    cout << A << " ";
                }
            }
            output.close(); // закрываем поток-файл
        } else cout << endl << "error! Не удалось открыть файл output.txt" << endl;
        input.close();
        cout << endl << "Выполнение второго задания было завершено." << endl;
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;
}

void display_output ()
{
    ifstream output("output.txt"); // открываем выходной поток-объект
    if (output) // проверка открытия
    {
        cout << "*** Содержимое файла output.txt ***" << endl;
        int chislo;
        while (!output.eof()) // пока не конец файла
        {
            output >> chislo;
            cout << chislo << " ";
        }
        output.close();
    } else cout << endl << "error! Не удалось открыть файл output.txt" << endl;
    cout << endl;
}

void ex3()
{
    ifstream output("output.txt"); // открываем текстовый файл для чтения
    if (output) // проверка открытия
    {
        //Проверяем, вдруг файл пуст
        long file_size; // размер файла
        output.seekg(0, ios::end); // перемещаем указатель ввода в конец файла
        file_size = output.tellg(); // определяем текущую позицию указателя ввода, если он равен нулю, значит начало это и есть конец, а это значит что файл пуст
        if (file_size == 0) cout << "error! Файл пуст.";
        else {
            display_output(); // вывод output
            output.seekg(0); // возвращаю указатель на место
            int min, max, temp;
            output >> min;
            max = min;
            while (!output.eof()) // пока не достигнут конец файла
            {
                output >> temp;
                if (temp < min) min = temp;
                if (temp > max) max = temp;
            }
            double result = (min + max) * 0.5;
            cout << "Минимум: " << min << endl;
            cout << "Максимум: " << max << endl;
            cout << "Результат выполнения 3 задания: " << result << endl;
        }
        output.close();
    } else cout << endl << "error! Не удалось открыть файл output.txt" << endl;
}
int main()
{
    srand(time(NULL)); // для генерации случайных чисел начиная с произвольного числа

    system("chcp 1251 > nul");
    cout << "Лабораторная работа №5 Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;

    /// генерация длины последовательности чисел

    int N = 10 + rand()%21; // длина последовательности чисел

    N = 11; // для ТЕСТИРОВАНИЯ
    /// выбор способа генерирования последовательности

    cout << "Задание 1. Генерирование последовательности случайных чисел." << endl << endl;
    cout << "Длина последовательности: " << N << endl << endl;

    int user_choice;

    cout << "Выберите способ генерирования последовательности: " << endl;
    cout << "1. Генерирование случайными числами." << endl;
    cout << "2. Ввод с клавиатуры." << endl;
    cout << "Ваш выбор: ";
    cin >> user_choice;

    /// проверка на некорректный ввод

    while (user_choice < 1 || user_choice > 2)
    {
        cout << "error!" << endl;
        cout << "Выберите способ генерирования последовательности: " << endl;
        cout << "1. Генерирование случайными числами." << endl;
        cout << "2. Ввод с клавиатуры." << endl;
        cout << "Ваш выбор: ";
        cin >> user_choice;
    }

    /// формирование последовательности и запись её в файл input.txt
    /// А так же выполнение задания 1 (и вывод результата выполнения на экран)

        switch (user_choice)
        {
            case 1: // генерирование случайными числами
            {
                ex1_random(N);
                break;

            }
            case 2: // с клавиатуры
            {
                ex1_from_keyboard(N);
                break;
            }
        }
        display_input();
        ex1();

    /// выполнение второго задания
    cout << endl << "*** Выполнение задания 2: ***" << endl;
    cout << "В файл output.txt записать только двузначные числа (как положительные, так и отрицательные) из файла input.txt" << endl;
    ex2();
     /// выполнение третьего задания
    cout << endl << "*** Выполнение задания 3: ***" << endl << "Найти полусумму минимума и максимума из файла output.txt" << endl;
    ex3();

    return 0;
}
