// ver 1.3 - 02.04.19 -- ������ �����
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

vector<short int> filling_vector_from_keyboard (int N)
{
    vector<short int> A(N);
    cout << "*** �������������� ���������� ������� ���������� � ���������� ***" << endl;
    cout << "������� �������� ��������� �������: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "��� " << i+1 << "-��: ";
        cin >> chislo;
        A[i] = chislo;
    }
    return A;
}

vector<short int> filling_vector_random (int N, int min, int max)
{
    vector<short int> A(N);
    cout << "*** �������������� ���������� ������� ���������� ������ ������� ***" << endl;
    int chislo;
    int ostatok = (max - min) + 1;
    for (int i = 0; i < N; i++)
    {
        chislo = min + rand() % ( ostatok );
        A[i] = chislo;
    }
    return A;
}

vector<short int> filling_vector_from_file (int N, string filename)
{
    vector<short int> A(N);
    cout << "*** �������������� ���������� ������� �� ����� " << filename << " ***" << endl;
    int chislo;
    ifstream input(filename);
    if (input) // �������� ��������
    {
        for (int i = 0; i < N; i++)
        {
            input >> chislo;
            A[i] = chislo;
        }
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� " << filename << endl;
    return A;
}

void print_array (const vector<short int> &A)
{
    for (vector<short int>::const_iterator i = A.begin(); i != A.end(); ++i) cout << *i << " ";
    cout << endl;
}

void diapazon_rand (int &min, int& max)
{
        cout << "������� �������� ������������� ��������� �����." << endl;
        cout << "������ �������: ";
        cin >> min;
        cout << "������� �������: ";
        cin >> max;
        while (min >= max)
        {
            cout << "error!" << endl;
            cout << "������� �������: ";
            cin >> max;
        }
}

vector<short int> sposob_zapolneniya (int N)
{
    vector<short int> A;
    int choice;
    cout << "*** ����� ������� ���������� ***" << endl;
    cout << "1. ���������� � ����������." << endl;
    cout << "2. ���������� ���������� �������." << endl;
    cout << "3. ���������� �� �����." << endl;
    cout << "�������� ������ ����������: ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "error!" << endl;
        cout << "�������� ������ ����������: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        A = filling_vector_from_keyboard(N);
        break;
    }
    case 2:
    {
        int min, max;
        diapazon_rand(min,max);
        A = filling_vector_random(N,min,max);
        break;
    }
    case 3:
        string filename;
        cout << "������� �������� �����: ";
        cin >> filename;
        A = filling_vector_from_file(N,filename);
        break;
    }
    return A;
}

//�������� �� ������ ������� (���� ������ double ����� ����� ����� ������������ � int, �� ����� - ������ �������)
bool full_square (short int chislo)
{
    double sqrt_chislo = sqrt(chislo);
    if (chislo >= 0 && sqrt_chislo == static_cast<short int> (sqrt_chislo)) return true;
    else return false;
}

short int first_full_square (const vector<short int> &A)
{
    unsigned int N = A.size();
    for(unsigned int i = 0; i != N; i++)
    {
        if (full_square( A[i] )) return A[i];
    }
    return -1; // ������ ������ ��������� � ������� ���
}

bool is_fibonachi (short int chislo)
{
    int summa(0), a1 (0), a2(1);
    while (summa < chislo)
    {
        summa = a1 + a2;
        a1 = a2;
        a2 = summa;
    }
    if (chislo == summa) return true;
    else return false;

}
//����� var - �����, � ������� ��� �������� �����
bool find_var (short int chislo, short int zadannaya_cifra)
{
    short int ostatok;
    if (chislo == zadannaya_cifra) return true;
    while (chislo > 0)
    {
        ostatok = chislo % 10;
        if (ostatok == zadannaya_cifra) return true;
        chislo /= 10;
    }
    return false;
}

// ���������� ������� ������� �������� ����� �����, � ������� ��� �������� �����
void insert_after_var (vector<short int> &A, short int fullsquare, short int zadannaya_cifra)
{
    for (unsigned int i = 0; i != A.size(); i++)
    {
        if (!find_var(A[i],zadannaya_cifra))
        {
            A.reserve(A.size()+1);
            A.insert(A.begin()+i+1,fullsquare);
            i++; // ����� ����������� ������ ��� ��������� ������� i
        }
    }
}
// ���������� �� �����, � ������� ��� �������� �����
void insert_before_var (vector<short int> &A, short int fullsquare, short int zadannaya_cifra)
{
    for (unsigned int i = 0; i != A.size(); i++)
    {
        if (!find_var(A[i],zadannaya_cifra))
        {
            A.reserve(A.size()+1);
            A.insert(A.begin()+i,fullsquare);
            i++; // ����� ����������� ������ ��� ��������� ������� i
        }
    }
}

// �������� ����� ���������
void delete_fibonachi (vector<short int> &A)
{
    for (unsigned int i = 0; i != A.size(); i++)
    {
        if (is_fibonachi(A[i]))
        {
            A.erase(A.begin()+i);
            i--; // ��� ��� ������� ��� i ��� ������ � ������ �� ���� ����� ������ �������
        }
    }
}
int main()
{
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "������������ ������ �6 ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    bool FirstTime = true;
    vector<short int> A;
    unsigned int N;
    int menu;
    while (menu != 4 && N > 0) { //���� ������������ ����� ���������� ������ ��� ������ > 0 - ������ ������������
        //����
        cout << "*** ���� ***" << endl;
        cout << "1. ������� 1.1 - �������� ������ �������, ���������� ������ ��������� ����� �����, � ������� ��� �������� �����." << endl;
        cout << "2. ������� 1.2 - �������� ������ �������, ���������� ������ ��������� ����� ������, � ������� ��� �������� �����." << endl;
        cout << "3. ������� 2 - ������� ��� ����� ���������." << endl;
        cout << "4. ��������� ������ ���������." << endl;
        cout << "�������� ����� ����: ";
        cin >> menu; // �������� ����� ���� � ���������� �����
        //�������� �� ������
        while (menu < 0 || menu > 4)
        {
            cout << "error!" << endl;
            cout << "�������� ����� ����: ";
            cin >> menu;
        }
        if (menu != 4) // ��������� ������ ���� ������������ ������ 4 ����� ����
        {
            if (FirstTime) // ��� ������ ������� ���������, ����� ���� ������ ����� ����������� � ����������� �������, ������� ���������� ����
            {
                cout << "������� ������ �������: ";
                cin >> N;
                while (N < 1)
                {
                    cout << "error!" << endl;
                    cout << "������� ������ �������: ";
                    cin >> N;
                }
                A = sposob_zapolneniya(N); // ������ (������, ������) ����������� ���� ���
                FirstTime = false;
            }
            switch (menu) // ��������� ��������������� � ���������� �������
            {
                case 1:
                {
                    cout << "���������� ������� 1.1 - �������� ������ �������, ���������� ������ ��������� ����� �����, � ������� ��� �������� �����." << endl;
                    short int add_elem = first_full_square(A);
                    short int zadannaya_cifra;
                    if (add_elem == -1)
                    {

                        cout << "*** � ������� ����������� ������ ��������! ***" << endl;
                        cout << "������� �����, ������� ���������� ��������: ";
                        cin >> add_elem;
                    }

                    cout << "������� �������� �����: ";
                    cin >> zadannaya_cifra;
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "������� �������� �����: ";
                        cin >> zadannaya_cifra;
                    }
                    cout << " ������ �� ���������: ";
                    print_array(A);

                    /// ���������� �������
                    insert_after_var(A,add_elem,zadannaya_cifra);
                    if (A.size() == N)
                    {
                        cout << "������ �� ��������� ����������, ��� ��� �� ���� ������� �����, � ������� ��� �������� �����." << endl;
                    }
                    else
                    {
                        cout << " ������ ����� ���������: ";
                        print_array(A);
                        N = A.size();
                    }
                    cout << "������� 1.1 ���������!" << endl;
                    break;
                }
                case 2:
                {
                    cout << "���������� ������� 1.2 - �������� ������ �������, ���������� ������ ��������� ����� ������, � ������� ��� �������� �����." << endl;
                    short int add_elem = first_full_square(A);
                    short int zadannaya_cifra;
                    if (add_elem == -1)
                    {

                        cout << "*** � ������� ����������� ������ ��������! ***" << endl;
                        cout << "������� �����, ������� ���������� ��������: ";
                        cin >> add_elem;
                    }

                    cout << "������� �������� �����: ";
                    cin >> zadannaya_cifra;
                    while (zadannaya_cifra < 0 || zadannaya_cifra > 9)
                    {
                        cout << "error!" << endl;
                        cout << "������� �������� �����: ";
                        cin >> zadannaya_cifra;
                    }
                    cout << " ������ �� ���������: ";
                    print_array(A);

                    /// ���������� �������
                    insert_before_var(A,add_elem,zadannaya_cifra);
                    if (A.size() == N)
                    {
                        cout << "������ �� ��������� ����������, ��� ��� �� ���� ������� �����, � ������� ��� �������� �����." << endl;
                    }
                    else
                    {
                        cout << " ������ ����� ���������: ";
                        print_array(A);
                        N = A.size();
                    }
                    cout << "������� 1.2 ���������!" << endl;
                    break;
                }
                case 3:
                {
                    cout << " ������ �� ���������: ";
                    print_array(A);
                    delete_fibonachi(A);
                    if (A.size() == N)
                    {
                        cout << "������ �� ��������� ����������, ��� ��� �� ���� ������� ����� ���������." << endl;
                    }
                    else
                    {
                        cout << " ������ ����� ���������: ";
                        print_array(A);
                        N = A.size();
                    }
                    cout << "������� 2 ���������!" << endl;
                    break;
                }
            }
        }
    }
    if (N == 0) cout << "������ ������� � ���������� �������� ���������.";
    else cout << "��������� ���� ��������� �� ������� ������������.";

    return 0;

}
