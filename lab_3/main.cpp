//ver 1
#include <iostream>
#include <cmath>

using namespace std;

double func_Sn(double x, int N)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    S = mnoz2 = ((x-1) / (x+1)); // �������������� �������� ��� S � mnoz2 - ��� �������� S0.
    stepen = mnoz2 * mnoz2; // �������, �� ������� ���������� mnoz2 ��� n �� mnoz2 ��� n+1
    for (int n = 1; n <= N; n++) // ������� ����� � n = 1
    {
        mnoz1 += 2; // ������ ��������� ��� Sn, ����� ������ �����������
        mnoz2 *= stepen; // ���������� ������� ������� ��������� Sn �� 2
        Sn = mnoz2 / mnoz1; // ��������� Sn;
        S += Sn; // ���� �����
    }
    return S;
}

double func_Se (double x, double e)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    Sn = 1; // ����� ��������� ����, ����� Sn = 0 � Sn < e
    S = mnoz2 = ((x-1) / (x+1)); // �������������� �������� ��� S � mnoz2 - ��� �������� S0.
    stepen = mnoz2 * mnoz2; // �������, �� ������� ���������� mnoz2 ��� n �� mnoz2 ��� n+1
    for (int n = 1; fabs(Sn) > e; n++) // ������� ����� � n = 1
    {
        mnoz1 += 2; // ������ ��������� ��� Sn, ����� ������ �����������
        mnoz2 *= stepen; // ���������� ������� ������� ��������� Sn �� 2
        Sn = mnoz2 / mnoz1; // ��������� Sn;
        S += Sn; // ���� �����
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
    S = mnoz2 = ((x-1) / (x+1)); // �������������� �������� ��� S � mnoz2 - ��� �������� S0.
    stepen = mnoz2 * mnoz2; // �������, �� ������� ���������� mnoz2 ��� n �� mnoz2 ��� n+1
    for (int n = 1; n <= N; n++) // ������� ����� � n = 1
    {
        mnoz1 += 2; // ������ ��������� ��� Sn, ����� ������ �����������
        mnoz2 *= stepen; // ���������� ������� ������� ��������� Sn �� 2
        Sn = mnoz2 / mnoz1; // ��������� Sn;
        S += Sn; // ���� �����
    }
    return S;
}

double func (double x, double e)
{
    double S, Sn, mnoz1(1), mnoz2, stepen;
    Sn = 1; // ����� ��������� ����, ����� Sn = 0 � Sn < e
    S = mnoz2 = ((x-1) / (x+1)); // �������������� �������� ��� S � mnoz2 - ��� �������� S0.
    stepen = mnoz2 * mnoz2; // �������, �� ������� ���������� mnoz2 ��� n �� mnoz2 ��� n+1
    for (int n = 1; fabs(Sn) > e; n++) // ������� ����� � n = 1
    {
        mnoz1 += 2; // ������ ��������� ��� Sn, ����� ������ �����������
        mnoz2 *= stepen; // ���������� ������� ������� ��������� Sn �� 2
        Sn = mnoz2 / mnoz1; // ��������� Sn;
        S += Sn; // ���� �����
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
    cout << "������������ ������ �3. ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    int menu;
    do {
        cout << "����:\n" << endl;
        cout << "1. ������� 1 \n" << "2. ������� 2 \n" << "0. �����" << endl;
        cout << "������� ����� ������� ��� ���������� -> ";
        cin >> menu;
    } while (menu < 0 || menu > 2);

    switch (menu)
    {
        case 1: // ������ �������
        {
            double a, b, e, x;
            int N;

            cout << "���� ��������� [a;b] (�������� ����� ������� � ��������� [0,2; 1])" << endl;

            cout << "������� a -> ";
            cin >> a;
            while (a < 0.2 || a > 1)
            {
                cout << "������������ ����." << endl;
                cout << "������� a -> ";
                cin >> a;
            }

            cout << "������� b -> ";
            cin >> b;

            while (b <= a || b < 0.2 || b > 1)
            {
                cout << "������������ ����." << endl;
                cout << "������� b -> ";
                cin >> b;
            }

            cout << "������� N - > ";
            cin >> N;

            while (N <= 0 || N > 10)
            {
                cout << "������������ ����." << endl;
                cout << "������� N -> ";
                cin >> N;
            }

            cout << "������� e -> ";
            cin >> e;

            while (e == 0 || e > 0.1 || e < 0)
            {
                cout << "������������ ����." << endl;
                cout << "������� e -> ";
                cin >> e;
            }

            double h = (b-a) / 10;

            int i=1;
            // **** ��������� 1 ****
            cout << "��������� 1 (��������� �����, ������� ������������ �����������)." << endl;
            cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // ��������� �������
            double Sn, Se, Sx; // ��� �������� ������� ��� ������
            double S0, mnoz1(1), mnoz2, stepen; // ���������� ��� Sn � Se

            for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
            {
                Sn = mnoz2 = ((x-1) / (x+1)); // �������������� �������� ��� S � mnoz2 - ��� �������� S0.
                stepen = mnoz2 * mnoz2; // �������, �� ������� ���������� mnoz2 ��� n �� mnoz2 ��� n+1

                for (int n = 1; n <= N; n++) // ��� Sn
                {
                    mnoz1 += 2; // ������ ��������� ��� S0, ����� ������ �����������
                    mnoz2 *= stepen; // ���������� ������� ������� ��������� S0 �� 2
                    S0 = mnoz2 / mnoz1; // ��������� S0;
                    Sn += S0; // ���� �����
                }

                Se = mnoz2 = ((x-1) / (x+1)); // �������������� �������� ��� S � mnoz2 - ��� �������� S0.
                S0 = 1;

                for (int n = 1; fabs(S0) > e; n++) // ��� Se
                {
                    mnoz1 += 2; // ������ ��������� ��� S0, ����� ������ �����������
                    mnoz2 *= stepen; // ���������� ������� ������� ��������� S0 �� 2
                    S0 = mnoz2 / mnoz1; // ��������� S0;
                    Se += S0; // ���� �����
                }

                Sx = 0.5 * log(x);

                cout << i << "\t" << x << "\t" << Sn << "\t" << Se << "\t" << Sx << endl;
                i++;
            }
            // **** ����� ��������� 1 ****
            i = 1;
            // **** ��������� 2 ****
            cout << "��������� 2 (��� ���������������� �������)." << endl;
             cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // ��������� �������
            for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
            {
                cout << i << "\t" << x << "\t" << func_Sn(x, N) << "\t" << func_Se(x, e) << "\t" << func_Sx(x) << endl;
                i++;
            }
            // **** ����� ��������� 2 ****
            i = 1;
            // **** ��������� 3 ****
            cout << "��������� 3 (Sn, Se, S ����������� � ������� ������������� �������)." << endl;
            cout << "N n/n" << "\t" << "x" << "\t" << "Sn" << "\t\t" << "Se" << "\t\t" << "S" << endl; // ��������� �������
            for (x = a; x < b || fabs(x-b) < 0.0001; x += h)
            {
                cout << i << "\t" << x << "\t" << func(x, N) << "\t" << func(x, e) << "\t" << func(x) << endl;
                i++;
            }
            // **** ����� ��������� 3 ****
            break;
        }
        case 2: // ������ �������
        {
            //��������, ����� ��������� ����� ���� ����������� (������ ����) � ��� ���� �� �������� �� ������� unsigned int
            long long int proverka_N;
            unsigned int N;
            cout << "������� N - > ";
            cin >> proverka_N;
            while (proverka_N <= 0 || proverka_N > 4294967295)
            {
                cout << "����������� ����� N �� ����� ���� ���� ��� ������������� ������." << endl;
                cout << "������� N - > ";
                cin >> proverka_N;
            }
            N = proverka_N;

            int summa_chet = 0;
            int summa_nechet = 0;
            int x;

            while ((N / 10 > 0) || (N > 0 && N < 9)) // ���� ����� ������� �� 10 (�.� ������� ����� ��� �� 1 �����) ��� ���� ����� �����������
            {
                x = N % 10; // ������� ��������� ����� ����� N
                if (x % 2 == 0) summa_chet += x; // ������ ����� ��� ����� - ������ ��� ���
                else summa_nechet += x;          // �������� � ����� ���/����� � ����������� �� ��������
                N /= 10; // ������ � ����� N ��������� �����
            }
            if (summa_chet == summa_nechet) cout << "��, ����� ������ � �������� ���� � ������ ����� N �����.";
            else cout << "���, ����� ������ � �������� ���� � ������ ����� N �� �����.";

        }

    }
}

