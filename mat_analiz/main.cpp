#include <iostream>
#include <cmath>

using namespace std;

struct point {double x; double y;};

double z(double x, double y)
{
    //return x*x + x*y + y*y - 2*x - y; // (1;0)
   // return -x*x+6*x-16*y+((y*y*y)/3); // ������� ���
    //return (x-3)*(x-3)+(y-4)*(y-4); // 3 � 4
    return x*x+2*(y*y*y)-6*x*y;
}

bool point_is_equal (point a, point b)
{
    if ((a.x == b.x) && (a.y == b.y)) return true;
    return false;
}

point isled_poisk (point b2, double h)
{

    double value_b2 = z(b2.x, b2.y); // ��������� �������� � �������� �����

    if ( z((b2.x+h),b2.y) < value_b2 )
    {
        b2.x += h;
        value_b2 = z(b2.x, b2.y);
    }
    else if ( z((b2.x-h),b2.y) < value_b2 )
    {
        b2.x -= h;
        value_b2 = z(b2.x, b2.y);
    }
    else
    {
        if ( z(b2.x,(b2.y+h)) < value_b2 )
        {
            b2.y += h;
            value_b2 = z(b2.x, b2.y);
        }
        else if ( z(b2.x,(b2.y-h)) < value_b2 )
        {
            b2.y -= h;
            value_b2 = z(b2.x, b2.y);
        }
    }

    return b2;
}

void okruglenie (double &chislo, double e)
{
    if (fabs(chislo - static_cast<int>(chislo)) < e) chislo = static_cast<int>(chislo);
}
int main()
{
    system("chcp 1251 > nul");
    point b1, b2;
    double h,e;
    cout << "������� ���������� �������� �����." << endl;
    cout << "��� x: ";
    cin >> b1.x;
    cout << "��� y: ";
    cin >> b1.y;
    cout << "������� ��� h: ";
    cin >> h;
    cout << "������� �������� eps: ";
    cin >> e;
    b2 = b1;

    cout << "�������� ����� (" << b1.x << ";" << b1.y << ")" << endl;
    cout << "�������� � �������� �����: " << z(b1.x,b1.y) << endl;
    unsigned int i;
    for (i = 0; h > e; i++)
    {
        /// ����������� �����
        b2 = isled_poisk(b2,h);
        /// ����� ������������

        // ���� ������� �������� ������ ��� �������� � �������� �����
        // ��������� ����� ���� � ��������� ������������
        if (point_is_equal(b1,b2)) h /= 10;

        ///������ 1 ��� ������ �� �������
        else
        {
            bool poisk = true;
            for (unsigned int i_poisk = 0; poisk == true; i_poisk++)
            {
                point P;
                P.x = b1.x + 2 * (b2.x-b1.x); // ������������ � �������
                P.y = b1.y + 2 * (b2.y-b1.y);
                /// ����������� �����
                P = isled_poisk(P,h);
                /// ����� ������������
                /// ���� �������� � ����� ��������� �������� ����� P > ��� � ���������� �������� ����� b2, �� ������������ � ������� ����� b2
                if (z(P.x,P.y) >= z(b2.x,b2.y) )
                {
                    b1 = b2;
                    poisk = false;
                }
                /// ����� ����� ��������� �������� ����� P �� ����� �������� ����� b2
                else
                {
                    b1 = b2;
                    b2 = P;
                }
                if (i_poisk == 100)
                {
                    int menu;
                    cout << "���� ����������� 100 ����� ������ �� �������." << endl;
                    cout << "�������� ������� �� ����� ��������." << endl;
                    cout << "������� �������� ����� (" << b2.x << ";" << b2.y << ")" << endl;
                    cout << "�������� � ���� �����:" << z(b2.x,b2.y) << endl;
                    cout << "������ ���������� ������ ���������?" << endl;
                    cout << "1. ��." << endl;
                    cout << "2. ��������� ������ ���������." << endl;
                    cout << "����: ";
                    cin >> menu;
                    while (menu < 1 || menu > 2)
                    {
                        cout << "������! ��������� ����: ";
                        cin >> menu;
                    }
                    switch (menu)
                    {
                        case 1:
                        {
                            i_poisk = 0;
                            break;
                        }
                        case 2:
                        {
                            poisk = false;
                            h = e;
                            break;
                        }
                    }
                }
            }
        }
    }
    cout << "������ ��������� ���������!" << endl;
    cout << "����� �������� ���������: " << i << endl;
    okruglenie(b2.x,e);
    okruglenie(b2.y,e);
    cout << "�������� ����� (" << b2.x << ";" << b2.y << ")" << endl;
    double value_output = z(b2.x,b2.y);
    okruglenie(value_output,e);
    cout << "��������:" << value_output;
    return 0;
}
