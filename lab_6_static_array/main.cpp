#include <iostream>

using namespace std;

void filling_array_from_keyboard (int A[], const int N)
{
    cout << "������� �������� ��������� �������: " << endl;
    int chislo;
    for (int i = 0; i < N; i++)
    {
        cout << "��� " << i << "-��: ";
        cin >> chislo;
        A[i] = chislo;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "������������ ������ �6 ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    const int N = 255;
    int A[N];
    return 0;
}
