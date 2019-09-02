//ver 1
#include <iostream>
#include <cmath>

using namespace std;

double func_Sn(double x, int N)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    S = mnoz2 = ((x-1) / (x+1)); // Первоначальное значение для S и mnoz2 - это значение S0.
    stepen = mnoz2 * mnoz2; // Степень, на которую отличается mnoz2 при n от mnoz2 при n+1
    for (int n = 1; n <= N; n++) // Считаем сразу с n = 1
    {
        mnoz1 += 2; // Первый множитель для Sn, грубо говоря коэффициент
        mnoz2 *= stepen; // Наращиваем степень второго множителя Sn на 2
        Sn = mnoz2 / mnoz1; // Вычисляем Sn;
        S += Sn; // Ищем сумму
    }
    return S;
}

double func_Se (double x, double e)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    Sn = 1; // Чтобы запустить цикл, иначе Sn = 0 и Sn < e
    S = mnoz2 = ((x-1) / (x+1)); // Первоначальное значение для S и mnoz2 - это значение S0.
    stepen = mnoz2 * mnoz2; // Степень, на которую отличается mnoz2 при n от mnoz2 при n+1
    for (int n = 1; fabs(Sn) > e; n++) // Считаем сразу с n = 1
    {
        mnoz1 += 2; // Первый множитель для Sn, грубо говоря коэффициент
        mnoz2 *= stepen; // Наращиваем степень второго множителя Sn на 2
        Sn = mnoz2 / mnoz1; // Вычисляем Sn;
        S += Sn; // Ищем сумму
    }
    return S;
}

double func_Sx(double x)
{
    return 0.5 * log(x);
}

double func(double x, int N)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    S = mnoz2 = ((x-1) / (x+1)); // Первоначальное значение для S и mnoz2 - это значение S0.
    stepen = mnoz2 * mnoz2; // Степень, на которую отличается mnoz2 при n от mnoz2 при n+1
    for (int n = 1; n <= N; n++) // Считаем сразу с n = 1
    {
        mnoz1 += 2; // Первый множитель для Sn, грубо говоря коэффициент
        mnoz2 *= stepen; // Наращиваем степень второго множителя Sn на 2
        Sn = mnoz2 / mnoz1; // Вычисляем Sn;
        S += Sn; // Ищем сумму
    }
    return S;
}

double func (double x, double e)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    Sn = 1; // Чтобы запустить цикл, иначе Sn = 0 и Sn < e
    S = mnoz2 = ((x-1) / (x+1)); // Первоначальное значение для S и mnoz2 - это значение S0.
    stepen = mnoz2 * mnoz2; // Степень, на которую отличается mnoz2 при n от mnoz2 при n+1
    for (int n = 1; fabs(Sn) > e; n++) // Считаем сразу с n = 1
    {
        mnoz1 += 2; // Первый множитель для Sn, грубо говоря коэффициент
        mnoz2 *= stepen; // Наращиваем степень второго множителя Sn на 2
        Sn = mnoz2 / mnoz1; // Вычисляем Sn;
        S += Sn; // Ищем сумму
    }
    return S;
}

double func(double x)
{
    return 0.5 * log(x);
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №3. Вариант 13.\nАвтор: Катунин Сергей. ДИПРБ-11.\n" << endl;
    int menu;
    do {
        cout << "Меню:\n" << endl;
        cout << "1. Задание 1 \n" << "2. Задание 2 \n" << "0. Выход" << endl;
        cout << "Введите номер задания для выполнения -> ";
        cin >> menu;
    } while (menu < 0 || menu > 2);

    switch (menu)
    {
        case 1: // первое задание
        {
            double a, b, e, x;
            int N;

            cout << "Ввод интервала [a;b] (интервал лежит целиком в интервале [0,2; 1])" << endl;

            cout << "Введите a -> ";
            cin >> a;
            while (a < 0.2 || a > 1)
            {
                cout << "Некорректный ввод." << endl;
                cout << "Введите a -> ";
                cin >> a;
            }

            cout << "Введите b -> ";
            cin >> b;

            while (b <= a || b < 0.2 || b > 1)
            {
                cout << "Некорректный ввод." << endl;
                cout << "Введите b -> ";
                cin >> b;
            }

            cout << "Введите N - > ";
            cin >> N;

            while (N <= 0 || N > 10)
            {
                cout << "Некорректный ввод." << endl;
                cout << "Введите N -> ";
                cin >> N;
            }

            cout << "Введите e -> ";
            cin >> e;

            while (e == 0 || e > 0.1 || e < 0)
            {
                cout << "Некорректный ввод." << endl;
                cout << "Введите e -> ";
                cin >> e;
            }

            double h = (b-a) / 10;

            int i=1;
            // **** ПРОГРАММА 1 ****
            cout << "Программа 1 (вложенные циклы, функции пользователя отсутствуют)." << endl;
            cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // заголовок таблицы
            double Sn, Se, Sx; // это значения функций для вывода
            double S0, mnoz1(1), mnoz2, stepen; // переменные для Sn и Se

            for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
            {
                Sn = mnoz2 = ((x-1) / (x+1)); // Первоначальное значение для S и mnoz2 - это значение S0.
                stepen = mnoz2 * mnoz2; // Степень, на которую отличается mnoz2 при n от mnoz2 при n+1

                for (int n = 1; n <= N; n++) // Для Sn
                {
                    mnoz1 += 2; // Первый множитель для S0, грубо говоря коэффициент
                    mnoz2 *= stepen; // Наращиваем степень второго множителя S0 на 2
                    S0 = mnoz2 / mnoz1; // Вычисляем S0;
                    Sn += S0; // Ищем сумму
                }

                Se = mnoz2 = ((x-1) / (x+1)); // Первоначальное значение для S и mnoz2 - это значение S0.
                S0 = 1;

                for (int n = 1; fabs(S0) > e; n++) // Для Se
                {
                    mnoz1 += 2; // Первый множитель для S0, грубо говоря коэффициент
                    mnoz2 *= stepen; // Наращиваем степень второго множителя S0 на 2
                    S0 = mnoz2 / mnoz1; // Вычисляем S0;
                    Se += S0; // Ищем сумму
                }

                Sx = 0.5 * log(x);

                cout << i << "\t" << x << "\t" << Sn << "\t" << Se << "\t" << Sx << endl;
                i++;
            }
            // **** КОНЕЦ ПРОГРАММЫ 1 ****
            i = 1;
            // **** ПРОГРАММА 2 ****
            cout << "Программа 2 (три пользовательских функции)." << endl;
             cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // заголовок таблицы
            for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
            {
                cout << i << "\t" << x << "\t" << func_Sn(x, N) << "\t" << func_Se(x, e) << "\t" << func_Sx(x) << endl;
                i++;
            }
            // **** КОНЕЦ ПРОГРАММЫ 2 ****
            i = 1;
            // **** ПРОГРАММА 3 ****
            cout << "Программа 3 (Sn, Se, S вычисляются с помощью перегруженной функции)." << endl;
            cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // заголовок таблицы
            for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
            {
                cout << i << "\t" << x << "\t" << func(x, N) << "\t" << func(x, e) << "\t" << func(x) << endl;
                i++;
            }
            // **** КОНЕЦ ПРОГРАММЫ 3 ****
            break;
        }
        case 2: // второе задание
        {
            //проверяю, чтобы введенное число было натуральное (больше нуля) и при этом не выходило за границы unsigned int
            long long int proverka_N;
            unsigned int N;
            cout << "Введите N - > ";
            cin >> proverka_N;
            while (proverka_N <= 0 || proverka_N > 4294967295)
            {
                cout << "Натуральное число N не может быть нулём или отрицательным числом." << endl;
                cout << "Введите N - > ";
                cin >> proverka_N;
            }
            N = proverka_N;

            int summa_chet = 0;
            int summa_nechet = 0;
            int x;

            while ((N / 10 > 0) || (N > 0 && N < 9)) // пока число делится на 10 (т.е состоит более чем из 1 цифры) или пока число однозначное
            {
                x = N % 10; // получаю последнюю цифру числа N
                if (x % 2 == 0) summa_chet += x; // смотрю какая это цифра - четная или нет
                else summa_nechet += x;          // добавляю к сумме чет/нечет в зависимости от ситуации
                N /= 10; // убираю у числа N последнюю цифру
            }
            if (summa_chet == summa_nechet) cout << "Да, суммы чётных и нечётных цифр в записи числа N равны.";
            else cout << "Нет, суммы чётных и нечётных цифр в записи числа N не равны.";

        }

    }
}

