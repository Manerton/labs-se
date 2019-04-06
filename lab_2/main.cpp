#include <iostream>
#include <fstream>
#include <cmath>
#include <windows.h>
#include <iomanip>

using namespace std;

int main()
{
    system("chcp 1251 > nul");
    cout << "������������ ������ �2. ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    // ����� ������ ����
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
        //-----������� �������-----
        cout << "\n ��������� � �������� �� ����� �������� ������� y = f(x) � ������ x0=-Pi, x1=x0+h, x2=x1+h, � x12 = +Pi, ��� h = Pi/6." << endl;
        cout << " ����� �������: ������������� ������� y = f(x) �� ������� [-Pi ; Pi] � ����� h." << endl;
        cout << " y = f(x) : sin^4(a) + sin^4(x)" << endl;
        cout << "\t    -------------------" << endl;
        cout << "\t\t    a+x" << endl << endl;
        const int N = 12;               //// ����������,
        double a, x_n, x, x_k, h; //  ������������ � 1 �������
        a = 0.5;
        x_n = -M_PI;
        x = x_n;
        x_k = M_PI;
        h = (x_k - x_n) / N;
        double func_1 = pow(sin(a),4); // ����� �� ������� � ������� ���� � ����
        double y; // ���� �������
        int sposob_for_ex1;
        //-----����� �������-----
        do {
            cout << "1. ������ 1 - ���� � ������������" << endl;
            cout << "2. ������ 2 - ���� � ������������" << endl;
            cout << "3. ������ 3 - ���� � ����������" << endl;
            cout << "������� ����� ������� -> ";
            cin >> sposob_for_ex1;
        } while (sposob_for_ex1 < 1 || sposob_for_ex1 > 3);
        //-----��������� ����-----
        ofstream output("Out.txt");
        //-----��������� ������ �� �����------
        cout << "\t" << "x" << "\t" << "|" << "\t" << "y" << endl;
        cout << setfill('-') << setw(35); // ����� ����
        cout << "-" << endl;
        cout << setfill(' '); // ��������� ��������� setfill
        //-----��������� ������ � ����-----
        output << "\t" << "x" << "\t" << "|" << "\t" << "y" << endl;
        output << setfill('-') << setw(35); // ����� ����
        output << "-" << endl;
        output << setfill(' ');
        //-----�������-----
        switch (sposob_for_ex1)
        {
        case 1:
            while (x <= x_k)
            { // ���� � ������������
                y = (func_1 + pow(sin(x),4))/(a+x); // �������
                //����� �� �����
                cout << fixed << setprecision(2); // �������� � ������������� ����� ������ ��������������� �����, ����� �� ��������� �� 2 ���� ����� �������
                cout << "\t" << x << "\t" << "|" << "\t" << y << endl;
                //����� � ����
                output << fixed << setprecision(2);
                output << "\t" << x << "\t" << "|" << "\t" << y << endl;
                x += h;
            }
            break;
        case 2:
            do { // ���� � ������������
                y = (func_1 + pow(sin(x),4))/(a+x); // �������
                //����� �� �����
                cout << fixed << setprecision(2); // �������� � ������������� ����� ������ ��������������� �����, ����� �� ��������� �� 2 ���� ����� �������
                cout << "\t" << x << "\t" << "|" << "\t" << y << endl;
                //����� � ����
                output << fixed << setprecision(2);
                output << "\t" << x << "\t" << "|" << "\t" << y << endl;
                x += h;
            } while (x <= x_k);
            break;
        case 3:
           for (x = x_n; x <= x_k; x += h) {
                y = (func_1 + pow(sin(x),4))/(a+x); // �������
                //����� �� �����
                cout << fixed << setprecision(2); // �������� � ������������� ����� ������ ��������������� �����, ����� �� ��������� �� 2 ���� ����� �������
                cout << "\t" << x << "\t" << "|" << "\t" << y << endl;
                //����� � ����
                output << fixed << setprecision(2);
                output << "\t" << x << "\t" << "|" << "\t" << y << endl;
            }
            break;
        }
        //-----����� �������� � ������� �������-----
        //-----��������� ����-----
        output.close();
        break;
    }
    case 2:
    {
        //-----������� �������-----
        cout << "\n* ������ E - ������ �������������� ���������." << endl;
        cout << "\n �������������� ������� Z=f(x,y) � ������ dx � dy ������ �������������� x E [a,b], y E [c,d] \n � ������� ������������� ������ �������." << endl << endl;
        //-----����� �������-----
        double a, b, c, d, x, y, dx, dy; //  ������������ � 2 �������
        //-----�������� �����-----
        do {
            cout << "������� �������� a -> ";
            cin >> a;
            cout << "������� �������� b -> ";
            cin >> b;
            if (fabs(a - b) < 0.001 || a > b) cout << "������������ ���� ������." << endl;
        } while (fabs(a - b) < 0.001 || a > b);
        do {
            cout << "������� �������� c -> ";
            cin >> c;
            cout << "������� �������� d -> ";
            cin >> d;
            if (fabs(c - d) < 0.001 || c > d) cout << "������������ ���� ������." << endl;
        } while (fabs(c - d) < 0.001 || c > d);
        //-----����� �����-----
        dx = (b - a) / 10;
        dy = (d - c) / 10;
        cout << "dx � dy ���� ��������� � ������ 10 �����." << endl;
        cout << "dx -> " << dx << endl;
        cout << "dy -> " << dy << endl << endl;
        double z; // ���� �������
        int sposob_vivoda;

        //-----����� ������� ������-----
        do {
            cout << "1. ������ 1 - ����� �� �����" << endl;
            cout << "2. ������ 2 - ����� � ����" << endl;
            cout << "������� ����� ������� -> ";
            cin >> sposob_vivoda;
        } while (sposob_vivoda < 1 || sposob_vivoda > 2);
        //-----����� ������-----

        //-----������� ������ � ������ �������----
        switch (sposob_vivoda)
        {
        case 1:
        {
            //-----��������� ������ �� �����------
            cout << setw(3) << "\\" << setw(4) << "X" << setw(2) << "|" << endl;
            cout << setw(4) << "\\" << setw(5) << "|" << endl;
            cout << setw(2) << "Y" << setw (3) << "\\" << setw(4) << "|";
            for (x = a; x <= b; x += dx) {
                cout << "\t" << x;
            }
            cout << endl;
            cout << setfill('-') << setw(113);
            cout << "\n";
            cout << setfill(' ');
            //-----����� ���������-----
            //-----����� �������-----
            for (y = c; y <= d; y += dy)
            {
                cout << y << "\t" << "|";
                for (x = a; x <= b; x += dx)
                {
                    if (fabs(x - y) < 0.001) cout  << "\t*";
                    else
                    {
                        z = exp(x*y/(x-y));
                        cout << "\t";
                        cout << setprecision (2) << fixed << z;
                    }
                }
               // cout.unsetf(std::ios_base::floatfield); // �������� ��������, ����� �������� �� ������ ������������ ������ defaultfloat
               cout << defaultfloat << endl; // ����� "y" ��� ������ �������, � ���������� �����
               cout << setfill('-') << setw(113);
               cout << "\n";
               cout << setfill(' ');
               //-----����� ������ �������-----
            }
            break;
        }
        case 2:
        {
            ofstream output("Out.txt");
            //-----��������� ������ � ����------
            output << setw(3) << "\\" << setw(4) << "X" << setw(2) << "|" << endl;
            output << setw(4) << "\\" << setw(5) << "|" << endl;
            output << setw(2) << "Y" << setw (3) << "\\" << setw(4) << "|";
            for (x = a; x <= b; x += dx) {
                output << "\t" << x;
            }
            output << endl;
            output << setfill('-') << setw(113);
            output << "\n";
            output << setfill(' ');
            //-----����� ���������-----
            //-----����� �������-----
            for (y = c; y <= d; y += dy)
            {
                output << y << "\t" << "|";
                for (x = a; x <= b; x += dx)
                {
                    if (fabs(x - y) < 0.001) output  << "\t*";
                    else
                    {
                        z = exp(x*y/(x-y));
                        output << "\t";
                        output << setprecision (2) << fixed << z;
                    }
               }
               // cout.unsetf(std::ios_base::floatfield); // �������� ��������, ����� �������� �� ������ ������������ ������ defaultfloat
               output << defaultfloat << endl; // ����� "y" ��� ������ �������, � ���������� �����
               output << setfill('-') << setw(113);
               output << "\n";
               output << setfill(' ');
               //-----����� ������ �������-----
            }
            output.close();
            break;
        }
        }
        //����� �������� � ������� �������
        cout << "��� ����������� ����� � ����.";
        break;
    }
    return 0;
    }
}
