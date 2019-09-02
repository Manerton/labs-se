//ver 4 - 23.03.19

#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;
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

    ofstream input("input.txt"); // открываем выходной поток-объект
    if (input) // проверка открытия
    {
        input << N; // вводим в файл длину последовательности на 1 место

        int chislo = -1;
        bool zero_flag = false; // когда будет найден первый нулевой элемент - станет true
        int summa = 0; // сумма чисел после первого нулевого элемента
        int count_of_elem = 0; // кол-во чисел после первого нулевого элемента

        switch (user_choice)
        {
            case 1: // генерирование случайными числами
            {
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
                for (int i = 1; i <= N; i++)
                {
                    if (chislo == 0) zero_flag = true;
                    chislo = rand_min + rand() % ostatok;
                    input << " " << chislo;
                    if (zero_flag)
                    {
                        summa += chislo;
                        count_of_elem++;
                    }

                }
                break;

            }
            case 2: // с клавиатуры
            {
                cout << "Задайте значения элементов последовательности: " << endl;
                for (int i = 1; i <= N; i++)
                {
                    if (chislo == 0) zero_flag = true;
                    cout << "Для " << i << "-го: ";
                    cin >> chislo;
                    input << " " << chislo;
                    if (zero_flag)
                    {
                        summa += chislo;
                        count_of_elem++;
                    }
                }
                break;
            }
        }
        double sredne_arif;
        if (summa == 0) sredne_arif = 0; // исключаю деление на ноль, если в последовательности
        else sredne_arif = double(summa) / count_of_elem; // не было нулевого элемента
        cout << "Результат выполнения 1 задания: " << sredne_arif << endl;
        input.close();
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;

    /// выполнение второго задания
    cout << endl << "*** Выполнение задания 2: ***" << endl << "В файл output.txt записать только двузначные числа (как положительные, так и отрицательные) из файла input.txt" << endl;
    ifstream input_ex2("input.txt"); // открываем текстовый файл для чтения
    if (input_ex2) // проверка открытия
    {
        input_ex2 >> N; // читаем длину последовательности
        int A, A_temp, kolvo_deleniy(0); // если нацело разделилось один раз - число двузначное

        ofstream output("output.txt"); // открываем выходной поток для 2 задания
        if (output) // проверка открытия
        {
            for (int i = 1; i <= N; i++)
            {
                input_ex2 >> A;
                if (A < 0) A_temp = -A; // так как по условию задачи нас интересуют все двузначные числа (отрицательные тоже)
                else A_temp = A;

                while ((A_temp / 10) > 0 && kolvo_deleniy < 2)
                {
                    kolvo_deleniy++;
                    A_temp /= 10;
                }
                if (kolvo_deleniy == 1) // если нацело разделилось один раз - число двузначное
                {
                    output << A << " ";
                }
                kolvo_deleniy = 0;
            }
            output.close(); // закрываем поток-файл
        } else cout << endl << "error! Не удалось открыть файл output.txt" << endl;
        input_ex2.close();
        cout << "Выполнение второго задания было завершено." << endl;
    } else cout << endl << "error! Не удалось открыть файл input.txt" << endl;

     /// выполнение третьего задания
    cout << endl << "*** Выполнение задания 3: ***" << endl << "Найти полусумму минимума и максимума из файла output.txt" << endl;
    ifstream output_ex3("output.txt"); // открываем текстовый файл для чтения
    if (output_ex3) // проверка открытия
    {
        //Проверяем, вдруг файл пуст
        long file_size; // размер файла
        output_ex3.seekg(0, ios::end); // перемещаем указатель ввода в конец файла
        file_size = output_ex3.tellg(); // определяем текущую позицию указателя ввода, если он равен нулю, значит начало это и есть конец, а это значит что файл пуст
        if (file_size == 0) cout << "error! Файл пуст.";
        else {
            output_ex3.seekg(0);
            int min, max, temp;
            output_ex3 >> min;
            max = min;
            while (!output_ex3.eof()) // пока не достигнут конец файла
            {
                output_ex3 >> temp;
                if (output_ex3.eof()) break;
                if (temp < min) min = temp;
                if (temp > max) max = temp;
            }
            double result = (min + max) * 0.5;
            cout << "Минимум: " << min << endl;
            cout << "Максимум: " << max << endl;
            cout << "Результат выполнения 3 задания: " << result << endl;
        }
        output_ex3.close();
    } else cout << endl << "error! Не удалось открыть файл output.txt" << endl;

    return 0;
}
