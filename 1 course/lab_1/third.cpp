#include <fstream>
#include <iostream>
#include <cmath>
#include <windows.h>

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    int formula;
    double x, y;
    cout << "������������ ������ �1. ������� 13.\n\n�����: ������� ������. �����-11.\n" << endl;
    cout << "������ ������." << endl;
    cout << "������� �������� x: " << endl;
    cin >> x;
    cout << "�������� ������� (�� 1 �� 4): " << endl;
    cin >> formula;

  // �������� �� ���� ������������ ������

    while (formula < 1 || formula > 4)
    {
        cout << "������������ ����\n";
        cout << "�������� ������� (�� 1 �� 4): " << endl;
        cin >> formula;
    }
    switch (formula) {
    case 1:
        y = sqrt(abs(x)) + 0.5*sin(x);
        break;
    case 2:
        y = sin(x-5);
        break;
    case 3:
        if (fabs(x+3)< 0.0001 || x+6 < 0)
        {
            cout << "Error: " << "x: " << x << " �� ������ � ������� ����������� ������� �" << formula << endl;
            y = 0;
        } else {
            y = sqrt(x+6) / (x+3);
        }
        break;
    case 4:
        if (fabs(x+3)< 0.0001)
        {
             cout << "Error: " << "x: " << x << " �� ������ � ������� ����������� ������� �" << formula << endl;
            y = 0;
        } else {
            y = 4.0 / (3+x);
        }
        break;
    }
    cout << "�������� y: " << y << endl;

    ofstream output("Out.txt");
    output << y << endl;
    output.close();
    return 0;
}

