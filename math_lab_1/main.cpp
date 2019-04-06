// ver 1.4 - 04.03.19

#include <iostream>
#include <cmath>
#include <windows.h>
#include <time.h>

using namespace std;

//�������� �� ���������� ��������, Arr - ������, L - ���-�� ���� � �������, n - ����� �� ��������
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

//��������� �������
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

//��������� �������� ������� �������� �������
void change (int* Arr, int L, int l) {
    int n;
    int new_elem;
    cout << "\n �� ���������, ��� ������ ����� ������������." << endl;
    do {
        cout << " ����� ����� ������ ��������? (������� ���������� ����� �����)- ";
        cin >> n;
    } while ((n <= 0) || (n > L));
    do {
        cout << " ������� ����� �������� " << n << "��� �������� ��������� ( ��� �������� �� ������ ���� ������ " << (l-1) << ") - ";
        cin >> new_elem;
    } while ((new_elem < 0) || (new_elem >= l) || !checknapovtor(Arr, L, new_elem));
    Arr[n-1] = new_elem;
}

//���������� �� �����������
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

//����� �������
void print (const int* Arr, int L)
{
    for (int i = 0; i < L; i++) {
        cout << Arr[i] << " ";
    }
    cout << endl;
}
// ������ �������, n - ���-�� ����������, j - ������� ��� ������� B, i - ������� ��� ������� A.
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

// ������ �������

void ex2 (int* ab, int &m, int* A, int N, int* B, int M)
{
    // ab - ���������, ��������� ����������� A � B
    // m - ���-�� ���� ��-�� ab
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

// ������ �������

void ex3 (int* AB, int &P, int* A, int N, int* B, int M)
{
    // AB - ���������, ������������ �� A � B
    // P - ���-�� ���� ��-�� AB

    // ������������ � ����� ��������� AB ��� �������� �� A
    for (int i = 0; i < N; i++)
    {
        AB[i] = A[i];
    }
    int n; // ���������� ��� �������� �������� ��-�� B
    int i = N;
    //���� ������� �� B ���������� (�� ����� ��������� �� A)
    //�� �������� ��� � AB, ����� ��������� ���-�� ���� AB �� ������� (��� ��� P = N + M, �.� ���������� ���-�� ���� � AB ���� ����������� ���������)
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
    // ba - ��������� B\A
    int n; // ���������� ��� �������� �������� ��-�� B
    ba_m = 0; // ba_m - ���-�� ���� ��-�� ba
    //���������� �������� �� ��-�� B.
    //���� ������� ���������� (�� ����� ��������� �� A)
    //�� �������� ��� � ba
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
    // razb - ��������� ��������� A

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
    cout << "�������� ����� � ������� ������, �����-11. \n ���������� ����������, ������������ ������ �1." << endl;

    int N, M, l; // l - max for random int
    int m, P, ba_m, razb_size; // ������� �������� ��� ������� 2, 3, 4, 5
    int n; // ���������� ����������� ��� ������� 5
    int K = -1;

    do {
        cout << "\n ������� �������� N �� ��������� [6,10] - ";
        cin >> N;
    } while ((N < 6) || (N > 10));

    do {
        cout << " ������� �������� M (������� ��� ������ N) �� ��������� [4,8] - ";
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
                cout << "\n ������ ���������� � ����� �������? \n 0. ����� \n 1. ����������������� ��������� A \n 2. ����������������� ��������� B \n 3. ����������������� ��� ��������� \n 4. �������� ��������� ������� ��������� \n 5. ���������� \n " << endl;
                cin >> ch;
            } while ((ch < 0) || (ch > 5));

            if (ch == 4)
            {
                do {
                    cout << " ����� ��������� ������ ��������? \n 1. A \n 2. B \n " << endl;
                    cin >> ch;
                } while ((ch < 1) || (ch > 2));
                ch *= 10;
            }
            // �������� ������ ��������� - �������
            if (ch == 5)
            {
                //������ �������
                cout << " \n ������� �1. \n\n ����������� �� ������ ������� ��������� B ��������� A? - ";
                if (ex1 (A, N, B, M) == M) {
                    cout << " �� ";
                } else { cout << " ��� "; }
                cout << endl;

                //������ �������
                cout << " \n ������� �2. \n\n ���������-����������� �������� A � B: ";
                m = M;
                int* ab = new int [m];
                ex2(ab, m, A, N, B, M);
                print(ab, m);
                delete[] ab;

                //������ �������
                cout << " \n ������� �3. \n\n ���������-����������� �������� A � B:  ";
                P = N + M;
                int* AB = new int [P];
                ex3(AB, P, A, N, B, M);
                sort(AB, P);
                print(AB, P);
                delete[] AB;

                //��������� �������
                cout << " \n ������� �4. \n\n �������� �������� B � � (B\\A):  ";
                ba_m = M;
                int* ba = new int [ba_m];
                ex4(ba, ba_m, A, N, B, M);
                sort(ba, ba_m);
                print(ba, ba_m);
                delete[] ba;

                //����� �������
                cout << " \n ������� �5. \n\n ��������� ��������� �, ��������� �� � �����������:  ";
                //---���� K---
                if (K == -1) {
                    do {
                    cout << "\n\n ������� �������� K �� ��������� [2, " << (N-2) << "] - ";
                    cin >> K;
                    } while ((K < 2) || (K > N-2));
                }
                ////---����� �����---
                cout << endl;
                razb_size = N / K; // ������ ��������� K
                // ����� ������������ �������, ����� �������� ��� ��������� A � �������� 9 �������� �� 4 ������������ � 1 ��������� + 1 ������������ � 4 ����������
                // � �������� 4 ������������ � 2 ���������� + 1 ������������ � 1 ���������.
               // if ((razb_size+1) * (K-1) < N) razb_size++;

                int* razb = new int [razb_size]; // ��������� ��������� K_i
                n = 0; // ����� �������� �������� �� A

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

                //����� ������ �������
                ch = -1;
            }
            // ��������� ������ ���������
            if (ch == -1)
            {
                 do {
                     do {
                     cout << "\n ������ ���������� � �������� ������� (N = " << N << ", M = " << M << ", K = "<< K << ")?" << endl;
                     cout << "\n 0. ����� \n 1. �������� �������� N \n 2. �������� �������� M \n 3. �������� �������� K \n 4. ����������\n  ";
                     cin >> ch;
                    } while ((ch < 0) || (ch > 4));

                    if (ch == 1)
                    {
                         do {
                         cout << "\n ������� �������� N �� ��������� [6,10] - ";
                         cin >> N;
                        } while ((N < 6) || (N > 10));

                        if (K > N-2)
                        {
                            cout << "\n �������� K ����� �� ���� �������, ��� ��� �� �������� �������� N, � ��������������, �������� ������������ ������� ��� K." << endl;
                            K = N-2;
                            cout << " �������� K ���� �������� � ������ K = " << K;
                        }

                        if (M > N)
                        {
                            cout << "\n �������� M �� ������������� ������� M <= N, ��� ��� �� �������� N � ������� ��� ������ ��� M." << endl;
                            M = N;
                            cout << " �������� M ���� �������� � ������ M = " << M;
                        }

                    }

                    if (ch == 2)
                    {
                        do {
                        cout << "\n ������� �������� M (������� ��� ������ N) �� ��������� [4,8] - ";
                        cin >> M;
                        } while (M > N || M < 4 || M > 8);
                    }

                    if (ch == 3)
                    {
                        do {
                        cout << "\n ������� �������� K �� ��������� [2, " << (N-2) << "] - ";
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

    cout << "\n���������� ������ ��������� �� ������� ������������.";
    return 0;

}
