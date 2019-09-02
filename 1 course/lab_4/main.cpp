#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

void input(int &n)
{
    cout << "������� �������� n (�� 1 �� 10) -> ";
    cin >> n;
    while (n <= 0 || n > 10)
    {
        cout << "������������ ����." << endl;
        cout << "������� �������� n -> ";
        cin >> n;
    }
}

int find_nod (int a, int b) {
    if (a < 0) a = -a;
    if (b == 0) return a;
    return find_nod(b, a%b);
}

void sokr_drob (int &chis, int &znam)
{
    int nod = find_nod(chis, znam);
    if (nod > 1)
    {
        chis /= nod;
        znam /= nod;
    }
}

// ���� ����� ������� ������ �� ����� - �� ������ �������� - false, ���� �� ����� � � ���� - file = true;
void display_drob (int chis, int znam, bool file)
{
    bool znak_minus = false;
    int defis_chis(1), defis_znam(1);
    int count_of_defis;
    int chis_temp = chis;
    int znam_temp = znam;
    if (chis < 0)
    {
        znak_minus = true;
    }
    while (chis_temp / 10)
    {
        defis_chis++;
        chis_temp /= 10;
    }
    while (znam_temp / 10)
    {
        defis_znam++;
        znam_temp /= 10;
    }
    if (defis_chis >= defis_znam) count_of_defis = defis_chis;
    else if (defis_chis < defis_znam) count_of_defis = defis_znam;

    if (znak_minus && (defis_chis == defis_znam))
    {
        cout << " " << chis << endl;
        cout << "  " << setfill('-') << setw(++count_of_defis) << "\n";
        cout << "  " << znam << endl;
    } else {
        cout << " " << chis << endl;
        cout << " " << setfill('-') << setw(++count_of_defis) << "\n";
        cout << " " << znam << endl;
    }

    if (file)
    {
        ofstream out("Out.txt");
        out << " " << chis << endl;
        out << " " << setfill('-') << setw(count_of_defis) << "\n";
        out << " " << znam << endl;
        out.close();
    }
}

void func (int n, int &chis, int &znam)
{
    int slag_chis, slag_znam;
    //�������� ����� ������ ���� ��������� ��� ����� ������� ������
    chis = 1;
    znam = 3;
    slag_chis = -2;
    slag_znam = 4;
    chis = (chis * slag_znam) + (znam * slag_chis);
    znam = znam * slag_znam;
    //���������� ����� ���������
    sokr_drob(chis, znam);
    cout << "����� ������ ���� ���������: " << endl;
    display_drob(chis, znam, false);

    slag_chis = 3; // ��������� ��� n = 2
    slag_znam = 5;

    for (int i = 2; i <= n; i++) // �� ��� ���, ���� �� ��������� ��� �����, ��������� � ��������� ����� ��������� ��������� � ��� ������������� ��������
    {
        if (i % 2 == 0)
        {
            chis = (chis * slag_znam) - (znam * slag_chis);
        } else {
            chis = (chis * slag_znam) + (znam * slag_chis);
        }

        znam = znam * slag_znam;
        sokr_drob(chis, znam);
        cout << "����� ������ ��� n = " << i << endl;
        display_drob(chis, znam, false);
        slag_chis += 2;
        slag_znam += 2;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "������������ ������ �4 ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    cout << "������ 1. ��������� � ����������� - ��� ��������� ����������." << endl << endl;
    int n;
    input(n);
    // �������� ������
    // ��� n = -1 � 0 - ����� �������� ��������� ����
    // ��� n = 1 -> -1/6
    // ��� n = 2 -> -23/30
    // ��� n = 3 -> -11/210
    // ��� n = 4 -> -523/630
    // ��� n = 5 -> -83/6930
    // ��� n = 6 -> -77309/90090

    int chis, znam;
    func(n, chis, znam);
    cout << "�������������� ������: " << endl;
    display_drob(chis, znam, true);

    return 0;
}
