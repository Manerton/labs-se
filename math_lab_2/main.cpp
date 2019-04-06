//ver 2 - 25.03.19 // 15:16


#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <algorithm>


using namespace std;

typedef vector<bool> row;
typedef vector<row> matrix;


//�������� �� ���������� ��������, M - ������, k - ����� �� ��������
bool checknapovtor (vector<int> M, int k)
{
    for (vector<int>::iterator i = M.begin(); i != M.end(); ++i)
    {
         if(*i == k) return false;
    }

    return true;
}

//��������� �������
void generation_vector (vector<int> &M)
{
    // ������ int k = a + rand() % ( b - a + 1 );
    int k; // �������� [-10;10]
    for (vector<int>::iterator i = M.begin(); i != M.end(); ++i)
    {
        do {
            k = -10 + rand() % ( 21 );
        } while (!checknapovtor(M, k));
         *i = k; // ����������� �������� ��� �������� i �������� k
    }
}

//����� �������
void print_array (const vector<int> &M)
{
    for (vector<int>::const_iterator i = M.begin(); i != M.end(); ++i) cout << *i << " ";
    cout << endl;
}

void create_empty_matrix (matrix &A)
{
    row Row(5,0);
    for (int i = 0; i < 5; i++)
    {
        A.push_back(Row);
    }
}
void print_matrix (const matrix &A) // �������� const vector<int> &M
{
   // cout << '\t';
    //for (vector<int>::const_iterator i = M.begin(); i != M.end(); ++i) cout << *i << '\t';
    //cout << endl;
    for (int i = 0; i < 5; i++)
    {
       // cout << M[i] << '\t';
        for(int j = 0; j < 5; j++)
        {
            cout << A[i][j] << '\t';
        }
        cout << endl;
    }
}


bool uslovie_ex1 (int x, int y) // ������� ���������
{
    if ( (x % 2 == 0) && (x <= y) ) return 1;
    else return 0;
}

void ex1 (matrix &A, const vector<int> &M) // ������ ������� (������� A 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            A[i][j] = uslovie_ex1(M[i], M[j]);
        }
    }
}

void ex2 (matrix &B, const matrix &A) // ������ ������� (������� B 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            B[i][j] = 1 - A[i][j];
        }
    }
}

void ex3 (matrix &C, const vector<int> &M) // ������ ������� (������� C 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            C[i][j] = uslovie_ex1(M[j], M[i]);
        }
    }
}

void ex4 (matrix &D, const matrix &A, const matrix &B) // ��������� ������� (������� D 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                D[i][j] =+ (A[i][k] && B[k][j]);
            }
        }
    }
}

void ex5 (matrix &E, const matrix &D) // ����� ������� (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            E[j][i] = 1 - D[i][j];
        }
    }
}

void ex6 (matrix &F, const matrix &A, const matrix &D) // ������ ������� (������� F 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            F[i][j] = (A[i][j] && D[i][j]);
        }
    }
}

bool ex7_a (const matrix &E) // �������� �������������� (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        if (E[i][i] != 1) return false;
    }
    return true;
}

bool ex7_b (const matrix &E) // �������� ������������������ (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        if (E[i][i] != 0) return false;
    }
    return true;
}

bool ex7_g (const matrix &E) // �������� �������������� (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = (i+1); j < 5; j++)
        {
            if (E[i][j] != E[j][i]) return false;
        }
    }
    return true;
}

bool ex7_d (const matrix &E) // �������� ������������������ (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = (i+1); j < 5; j++)
        {
            if (E[i][j] == 1 && E[i][j] == E[j][i]) return false;
        }
    }
    return true;
}

bool ex7_e (const matrix &E) // �������� ��������������� (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (E[i][j] == E[j][i]) return false;
        }
    }
    return true;
}

bool ex7_z (const matrix &E) // �������� �������������� (������� E 5x5)
{
    // R = { (x,y) E A^2 | x <= y, x - ������}
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if ((E[i][k] == E[k][j]) && (E[i][k] == 1) && (E[i][j] == 0)) return false;
            }
        }
    }
    return true;
}

int main()
{
     // R = { (x,y) E A^2 | x <= y, x - ������}
    srand((unsigned)time(0));
    system("chcp 1251 > nul");
    cout << "�������� ����� � ������� ������, �����-11. \n ���������� ����������, ������������ ������ �2." << endl;
    vector<int> M(5);
    int ch = 1;
    while (ch == 1)
    {
        generation_vector(M);
        sort( M.begin(), M.end() ); // ����������
        cout << "���������� ��������� M: ";
        print_array(M);
        cout << "\nR = { (x,y) E A^2 | x <= y, x - ������ }." << endl;
        cout << "\n1. ��������� ������� � ��������� R." << endl;
        matrix A; // ���������� �������
        create_empty_matrix(A); // �������� ������ �������
        ex1(A, M); // ���������� ������� � ������������ � �������� 1
        print_matrix(A); // ����� �������

        cout << "\n2. ��������� ������� B ���������� R` ��������� R." << endl;
        matrix B; // ���������� �������
        create_empty_matrix(B); // �������� ������ �������
        ex2(B, A); // ���������� ������� � ������������ � �������� 2
        print_matrix(B); // ����� �������

        cout << "\n3. ��������� ������� C ��������� R^-1 ��������� R." << endl;
        matrix C; // ���������� �������
        create_empty_matrix(C); // �������� ������ �������
        ex3(C, M); // ���������� ������� � ������������ � �������� 3
        print_matrix(C); // ����� �������

        cout << "\n4. ��������� ������� D ������������ ��������� R� = R * R`." << endl;
        matrix D; // ���������� �������
        create_empty_matrix(D); // �������� ������ �������
        ex4(D, A, B); // ���������� ������� � ������������ � �������� 3
        print_matrix(D); // ����� �������

        cout << "\n5. ��������� ������� E ������������� ��������� (R�)^d � ��������� R�." << endl;
        matrix E; // ���������� �������
        create_empty_matrix(E); // �������� ������ �������
        ex5(E, D); // ���������� ������� � ������������ � �������� 3
        print_matrix(E); // ����� �������

        cout << "\n6. ��������� ������� F ��������� R (�����������) R�." << endl;
        matrix F; // ���������� �������
        create_empty_matrix(F); // �������� ������ �������
        ex6(F, A, D); // ���������� ������� � ������������ � �������� 3
        print_matrix(F); // ����� �������

        cout << "\n7. ��������� �������� (R�)^d:" << endl;
        cout << "(1 - ��, 0 - ���)" << endl;
        cout << "�) �������������� - " << ex7_a(E) << endl; // � ������� ��������� ������ �������
        cout << "�) ������������������ - "; // � ������� ��������� ������ ����
            if (ex7_a(E)) cout << false << endl;
            else cout << ex7_b(E) << endl;
        cout << "�) ���������������� - "; // � ������� ��������� ���� ���� �� ���� �������
            if (ex7_b(E)) cout << false << endl;
            else cout << true << endl;
        cout << "�) �������������� - " << ex7_g(E) << endl; // ��������� ����� ������� ���������
        cout << "�) ������������������ - " << ex7_d(E) << endl; // ������������ ������� ��������� ��� ������������ ������
        cout << "�) ��������������� - " << ex7_e(E) << endl; // ������� ��������� ������� � ������������ ��������� ������������ ��������� ���
        cout << "�) �������������� - " << ex7_z(E) << endl; // ������� ������� ����� ������� ��������, ��� ������� � ex7_z
        cout << "\n����:" << endl;
        cout << "1. ����������������� ��������� M." << endl;
        cout << "2. ��������� ������ ���������." << endl;
        cin >> ch;
        while (ch < 1 || ch > 2)
        {
            cout << "error!" << endl;
            cout << "��������� ����." << endl;
            cin >> ch;
        }
        // ��� ����� �������
        /*if (!ex7_a(E)) ch = 1;
        else cin >> ch;*/
    }
    return 0;
}
