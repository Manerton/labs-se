#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct drob
{ int chis, znam; };

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

int find_nod (drob ab) {
    if (ab.chis < 0) ab.chis = -ab.chis;
    if (ab.znam == 0) return ab.chis;
    int ostatok = ab.chis%ab.znam;
    ab.chis = ab.znam;
    ab.znam = ostatok;
    return find_nod(ab);
}

void sokr_drob (drob &ab)
{
    int nod = find_nod(ab);
    if (nod > 1)
    {
        ab.chis /= nod;
        ab.znam /= nod;
    }
}

// ���� ����� ������� ������ �� ����� - �� ������ �������� - false, ���� �� ����� � � ���� - file = true;
void display_drob (drob ab, bool file)
{
    bool znak_minus = false;
    int defis_chis(1), defis_znam(1);
    int count_of_defis;
    int chis_temp = ab.chis;
    int znam_temp = ab.znam;
    if (ab.chis < 0)
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
        cout << " " << ab.chis << endl;
        cout << "  " << setfill('-') << setw(++count_of_defis) << "\n";
        cout << "  " << ab.znam << endl;
    } else {
        cout << " " << ab.chis << endl;
        cout << " " << setfill('-') << setw(++count_of_defis) << "\n";
        cout << " " << ab.znam << endl;
    }

    if (file)
    {
        ofstream out("Out.txt");
        out << " " << ab.chis << endl;
        out << " " << setfill('-') << setw(count_of_defis) << "\n";
        out << " " << ab.znam << endl;
        out.close();
    }
}

void func (int n, drob &ab)
{
    int slag_chis, slag_znam;
    //�������� ����� ������ ���� ��������� ��� ����� ������� ������
    ab.chis = 1;
    ab.znam = 3;
    slag_chis = -2;
    slag_znam = 4;
    ab.chis = (ab.chis * slag_znam) + (ab.znam * slag_chis);
    ab.znam = ab.znam * slag_znam;
    //���������� ����� ���������
    sokr_drob(ab);
    cout << "����� ������ ���� ���������: " << endl;
    display_drob(ab, false);

    slag_chis = 3; // ��������� ��� n = 2
    slag_znam = 5;

    for (int i = 2; i <= n; i++) // �� ��� ���, ���� �� ��������� ��� �����, ��������� � ��������� ����� ��������� ��������� � ��� ������������� ��������
    {
        if (i % 2 == 0)
        {
            ab.chis = (ab.chis * slag_znam) - (ab.znam * slag_chis);
        } else {
            ab.chis = (ab.chis * slag_znam) + (ab.znam * slag_chis);
        }

        ab.znam = ab.znam * slag_znam;
        sokr_drob(ab);
        cout << "����� ������ ��� n = " << i << endl;
        display_drob(ab, false);
        slag_chis += 2;
        slag_znam += 2;
    }
}

int main()
{
    system("chcp 1251 > nul");
    cout << "������������ ������ �4 ������� 13.\n�����: ������� ������. �����-11.\n" << endl;
    cout << "������ 2. ����� ������������ � ���� ���������" << endl << endl;
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

    drob ab;
    func(n, ab);
    cout << "�������������� ������: " << endl;
    display_drob(ab, true);

    return 0;
}
