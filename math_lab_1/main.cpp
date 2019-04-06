// ver 1.4 - 04.03.19

#include <iostream>
#include <cmath>
#include <windows.h>
#include <time.h>

using namespace std;

//проверка на повторение элемента, Arr - массив, L - кол-во элем в массиве, n - число на проверку
bool checknapovtor (const int* Arr, int L, int n)
{
    for (int i = 0; i < L; i++)
    {
        if(Arr[i] == n)
        {
            return false;
        }
    }
    return true;
}

//генерация массива
void generation_array (int* Arr, int L, int l)
{
    int n;
    for (int i = 0; i < L ; i++) {
        do {
            n = rand()%l;
        } while (!checknapovtor(Arr, L, n));
        Arr[i] = n;
    }
}

//изменение отдельно взятого элемента массива
void change (int* Arr, int L, int l) {
    int n;
    int new_elem;
    cout << "\n Не забывайте, что массив будет отсортирован." << endl;
    do {
        cout << " Какое число хотите поменять? (введите порядковый номер числа)- ";
        cin >> n;
    } while ((n <= 0) || (n > L));
    do {
        cout << " Введите новое значение " << n << "ого элемента множества ( его значение не должно быть больше " << (l-1) << ") - ";
        cin >> new_elem;
    } while ((new_elem < 0) || (new_elem >= l) || !checknapovtor(Arr, L, new_elem));
    Arr[n-1] = new_elem;
}

//сортировка по возрастанию
void sort (int* Arr, int L)
{
    for(int i = 0; i < L - 1; i++){
        for (int j = i+1; j < L; j++){
            if (Arr[i] > Arr[j])
            {
                swap(Arr[i],Arr[j]);
            }
        }
    }
}

//вывод массива
void print (const int* Arr, int L)
{
    for (int i = 0; i < L; i++) {
        cout << Arr[i] << " ";
    }
    cout << endl;
}
// первое задание, n - кол-во совпадений, j - счетчик для массива B, i - счетчик для массива A.
int ex1 (int* A, int N, int* B, int M)
{
    int n = 0;
    for (int j = 0; j < M; j++)
    {
        for (int i = 0; i < N; i++)
        {
            if (B[j] == A[i])
            {
                n += 1;
                i = N;
            }
        }
    }
    return n;
}

// второе задание

void ex2 (int* ab, int &m, int* A, int N, int* B, int M)
{
    // ab - множество, результат пересечения A и B
    // m - кол-во элем мн-ва ab
    m = 0;
    for (int j = 0; j < M; j++)
    {
        for (int i = 0; i < N; i++)
        {
            if (B[j] == A[i])
            {
                ab[m] = B[j];
                m++;
                i = N;
            }
        }
    }
}

// третье задание

void ex3 (int* AB, int &P, int* A, int N, int* B, int M)
{
    // AB - множество, объединенное из A и B
    // P - кол-во элем мн-ва AB

    // Переписываем в новое множество AB все элементы из A
    for (int i = 0; i < N; i++)
    {
        AB[i] = A[i];
    }
    int n; // переменная для хранения элемента мн-ва B
    int i = N;
    //Если элемент из B уникальный (не равен элементам из A)
    //то помещаем его в AB, иначе уменьшаем кол-во элем AB на единицу (так как P = N + M, т.е изначально кол-во элем в AB было максимально возможное)
    for (int j = 0; j < M; j++)
    {
        n = B[j];
        if (checknapovtor(A, N, n))
        {
            AB[i] = n;
            i++;
        } else { P--; }
    }
}

void ex4 (int* ba, int &ba_m, int* A, int N, int* B, int M)
{
    // ba - множество B\A
    int n; // переменная для хранения элемента мн-ва B
    ba_m = 0; // ba_m - кол-во элем мн-ва ba
    //Перебираем элементы из мн-ва B.
    //Если элемент уникальный (не равен элементам из A)
    //то помещаем его в ba
    for (int j = 0; j < M; j++)
    {
        n = B[j];
        if (checknapovtor(A, N, n))
        {
            ba[ba_m] = n;
            ba_m++;
        }
    }
}

void ex5 (int* razb, int razb_size, int* A, int N, int &n)
{
    // razb - разбиение множества A

    for (int i = 0; (i < razb_size && n < N); i++)
    {
        razb[i] = A[n];
        n++;
    }
}
int main()
{
    srand(time(0));
    system("chcp 1251 > nul");
    cout << "Измайлов Наиль и Катунин Сергей, ДИПРБ-11. \n Дискретная математика, Лабораторная работа №1." << endl;

    int N, M, l; // l - max for random int
    int m, P, ba_m, razb_size; // размеры массивов для заданий 2, 3, 4, 5
    int n; // переменная необходимая для задания 5
    int K = -1;

    do {
        cout << "\n Введите значение N из интервала [6,10] - ";
        cin >> N;
    } while ((N < 6) || (N > 10));

    do {
        cout << " Введите значение M (меньшее или равное N) из интервала [4,8] - ";
        cin >> M;
    } while (M > N || M < 4 || M > 8);

    int ch = 3; // user's choice

    do {
        int* A = new int [N];
        int* B = new int [M];
        ch = 3;
        do {
            l = N + 6; //max for random int

            if ((ch == 1) || (ch == 3)) {
                generation_array(A, N, l);
            }
            if ((ch == 2) || (ch == 3)) {
                generation_array(B, M, l);
            }
            if (ch == 10) {
                change (A, N, l);
            } else if (ch == 20) {
                change (B, M, l);
            }
            sort (A, N);
            sort (B, M);
            cout << " A: ";
            print (A, N);
            cout << " B: ";
            print (B, M);

            do {
                cout << "\n Хотите продолжить с этими данными? \n 0. Выйти \n 1. Пересгенирировать множество A \n 2. Пересгенирировать множество B \n 3. Пересгенирировать оба множества \n 4. Изменить отдельный элемент множества \n 5. Продолжить \n " << endl;
                cin >> ch;
            } while ((ch < 0) || (ch > 5));

            if (ch == 4)
            {
                do {
                    cout << " Какое множество хотите изменить? \n 1. A \n 2. B \n " << endl;
                    cin >> ch;
                } while ((ch < 1) || (ch > 2));
                ch *= 10;
            }
            // Основная работа программы - задания
            if (ch == 5)
            {
                //первое задание
                cout << " \n Задание №1. \n\n Принадлежит ли каждый элемент множества B множеству A? - ";
                if (ex1 (A, N, B, M) == M) {
                    cout << " Да ";
                } else { cout << " Нет "; }
                cout << endl;

                //второе задание
                cout << " \n Задание №2. \n\n Множество-пересечение множеств A и B: ";
                m = M;
                int* ab = new int [m];
                ex2(ab, m, A, N, B, M);
                print(ab, m);
                delete[] ab;

                //третье задание
                cout << " \n Задание №3. \n\n Множество-объединение множеств A и B:  ";
                P = N + M;
                int* AB = new int [P];
                ex3(AB, P, A, N, B, M);
                sort(AB, P);
                print(AB, P);
                delete[] AB;

                //четвертое задание
                cout << " \n Задание №4. \n\n Разность множеств B и А (B\\A):  ";
                ba_m = M;
                int* ba = new int [ba_m];
                ex4(ba, ba_m, A, N, B, M);
                sort(ba, ba_m);
                print(ba, ba_m);
                delete[] ba;

                //пятое задание
                cout << " \n Задание №5. \n\n Разбиение множества А, состоящее из К подмножеств:  ";
                //---ВВОД K---
                if (K == -1) {
                    do {
                    cout << "\n\n Введите значение K из интервала [2, " << (N-2) << "] - ";
                    cin >> K;
                    } while ((K < 2) || (K > N-2));
                }
                ////---КОНЕЦ ВВОДА---
                cout << endl;
                razb_size = N / K; // размер разбиений K
                // поиск оптимального размера, чтобы например при разбиении A с размером 9 получить не 4 подмножества с 1 элементом + 1 подмножество с 4 элементами
                // а получить 4 подмножества с 2 элементами + 1 подмножество с 1 элементом.
               // if ((razb_size+1) * (K-1) < N) razb_size++;

                int* razb = new int [razb_size]; // объявляем разбиение K_i
                n = 0; // номер текущего элемента из A

                int i_for_ex5;
                for (i_for_ex5 = 0; i_for_ex5 < K-1; i_for_ex5++)
                {
                    ex5(razb, razb_size, A, N, n);
                    cout << " K" << i_for_ex5+1 << ": ";
                    print(razb, razb_size);
                }
                razb_size = N - ((K-1) * razb_size);
                ex5(razb, razb_size, A, N, n);
                cout << " K" << i_for_ex5+1 << ": ";
                print(razb, razb_size);

                delete [] razb;

                //конец пятого задания
                ch = -1;
            }
            // Повторная работа программы
            if (ch == -1)
            {
                 do {
                     do {
                     cout << "\n Хотите продолжить с текущими данными (N = " << N << ", M = " << M << ", K = "<< K << ")?" << endl;
                     cout << "\n 0. Выйти \n 1. Изменить значение N \n 2. Изменить значение M \n 3. Изменить значение K \n 4. Продолжить\n  ";
                     cin >> ch;
                    } while ((ch < 0) || (ch > 4));

                    if (ch == 1)
                    {
                         do {
                         cout << "\n Введите значение N из интервала [6,10] - ";
                         cin >> N;
                        } while ((N < 6) || (N > 10));

                        if (K > N-2)
                        {
                            cout << "\n Значение K вышло за свою границу, так как вы изменили значение N, и соответственно, изменили максимальная границу для K." << endl;
                            K = N-2;
                            cout << " Значение K было изменено и теперь K = " << K;
                        }

                        if (M > N)
                        {
                            cout << "\n Значение M не удовлетворяет условию M <= N, так как вы изменили N и указали его меньше чем M." << endl;
                            M = N;
                            cout << " Значение M было изменено и теперь M = " << M;
                        }

                    }

                    if (ch == 2)
                    {
                        do {
                        cout << "\n Введите значение M (меньшее или равное N) из интервала [4,8] - ";
                        cin >> M;
                        } while (M > N || M < 4 || M > 8);
                    }

                    if (ch == 3)
                    {
                        do {
                        cout << "\n Введите значение K из интервала [2, " << (N-2) << "] - ";
                        cin >> K;
                        } while ((K < 2) || (K > N-2));
                    }
                } while ((ch != 0) && (ch != 4));
                if (ch == 4){
                    ch = -2;
                }
                delete [] A;
                delete [] B;
            }
        } while (ch != -2 && ch != 0);
    } while (ch != 0);

    cout << "\nЗавершение работы программы по желанию пользователя.";
    return 0;

}
