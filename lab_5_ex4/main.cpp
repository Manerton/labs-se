//ver 5 - 23.03.19

#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

void ex1_random (int N) // ������������� ���������� �������
{
    ofstream input("input.txt"); // ��������� �������� �����-������
    if (input) // �������� ��������
    {
        input << N; // ������ � ���� ����� ������������������ �� 1 �����
        int rand_min, rand_max;
        cout << "������� �������� ������������� ��������� �����." << endl;
        cout << "������ �������: ";
        cin >> rand_min;
        cout << "������� �������: ";
        cin >> rand_max;
        while (rand_min >= rand_max)
        {
            cout << "error!" << endl;
            cout << "������� �������: ";
            cin >> rand_max;
        }
        int ostatok = (rand_max - rand_min) + 1;
        int chislo;
        for (int i = 1; i <= N; i++)
        {
            chislo = rand_min + rand() % ostatok;
            input << " " << chislo;
        }
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;
}

void ex1_from_keyboard (int N) // ���� � ����������
{
    ofstream input("input.txt"); // ��������� �������� �����-������
    if (input) // �������� ��������
    {
        input << N; // ������ � ���� ����� ������������������ �� 1 �����
        cout << "������� �������� ��������� ������������������: " << endl;
        int chislo;
        for (int i = 1; i <= N; i++)
        {
            cout << "��� " << i << "-��: ";
            cin >> chislo;
            input << " " << chislo;
        }
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;
}

void ex1 () // ������ �������
{
    ifstream input("input.txt"); // ��������� �������� �����-������
    if (input) // �������� ��������
    {
        int N;
        input >> N; // ������ ����� ������������������
        cout << "*** ���������� ������� ������� ***" << endl;
        bool zero_flag = false; // ����� ����� ������ ������ ������� ������� - ������ true
        int summa = 0; // ����� ����� ����� ������� �������� ��������
        int count_of_elem = 0; // ���-�� ����� ����� ������� �������� ��������
        int chislo = -1; // ����� zero_flag ����� �� ��������
        for (int i = 1; i <= N; i++)
        {
            if (chislo == 0) zero_flag = true;
            input >> chislo;
            if (zero_flag)
            {
                summa += chislo;
                count_of_elem++;
            }
        }
        double sredne_arif;
        if (summa == 0) sredne_arif = 0; // �������� ������� �� ����, ���� � ������������������ �� ���� �������� ��������
        else sredne_arif = double(summa) / count_of_elem;
        cout << "��������� ���������� 1 �������: " << sredne_arif << endl;
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;
}

void display_input ()
{
    ifstream input("input.txt"); // ��������� �������� �����-������
    if (input) // �������� ��������
    {
        cout << "*** ���������� ����� input.txt ***" << endl;
        int chislo;
        while (!input.eof())
        {
            input >> chislo;
            cout << chislo << " ";
        }
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;
    cout << endl;
}

bool dvyznachnoe_or_not (int A)
{
    int kolvo_deleniy(0); // ���� ������ ����������� ���� ��� - ����� ����������
    if (A < 0) A = -A; // ��� ��� �� ������� ������ ��� ���������� ��� ���������� ����� (������������� ����)
    while ((A / 10) > 0 && kolvo_deleniy < 2) // ���� �����, ��� ����� ����������� ������ 2 ����, �� ������ ������ ������ ���
    {
        kolvo_deleniy++;
        A /= 10;
    }
    if (kolvo_deleniy == 1) // ���� ������ ����������� ���� ��� - ����� ����������
    {
        return true;
    }
    return false;
}

void ex2()
{
    ifstream input("input.txt"); // ��������� ��������� ���� ��� ������
    if (input) // �������� ��������
    {
        int N;
        input >> N; // ������ ����� ������������������
        int A;
        display_input();
        ofstream output("output.txt"); // ��������� �������� ����� ��� 2 �������
        if (output) // �������� ��������
        {
            cout << "*** ��������� ���������� ������� ������� ***" << endl; // ����� �� ����� � � ���� ����� �� input �����. ������� 2 ������
            for (int i = 1; i <= N; i++)
            {
                input >> A;
                if (dvyznachnoe_or_not(A))
                {
                    output << " " << A;
                    cout << A << " ";
                }
            }
            output.close(); // ��������� �����-����
        } else cout << endl << "error! �� ������� ������� ���� output.txt" << endl;
        input.close();
        cout << endl << "���������� ������� ������� ���� ���������." << endl;
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;
}

void display_output ()
{
    ifstream output("output.txt"); // ��������� �������� �����-������
    if (output) // �������� ��������
    {
        cout << "*** ���������� ����� output.txt ***" << endl;
        int chislo;
        while (!output.eof()) // ���� �� ����� �����
        {
            output >> chislo;
            cout << chislo << " ";
        }
        output.close();
    } else cout << endl << "error! �� ������� ������� ���� output.txt" << endl;
    cout << endl;
}

void ex3()
{
    ifstream output("output.txt"); // ��������� ��������� ���� ��� ������
    if (output) // �������� ��������
    {
        //���������, ����� ���� ����
        long file_size; // ������ �����
        output.seekg(0, ios::end); // ���������� ��������� ����� � ����� �����
        file_size = output.tellg(); // ���������� ������� ������� ��������� �����, ���� �� ����� ����, ������ ������ ��� � ���� �����, � ��� ������ ��� ���� ����
        if (file_size == 0) cout << "error! ���� ����.";
        else {
            display_output(); // ����� output
            output.seekg(0); // ��������� ��������� �� �����
            int min, max, temp;
            output >> min;
            max = min;
            while (!output.eof()) // ���� �� ��������� ����� �����
            {
                output >> temp;
                if (temp < min) min = temp;
                if (temp > max) max = temp;
            }
            double result = (min + max) * 0.5;
            cout << "�������: " << min << endl;
            cout << "��������: " << max << endl;
            cout << "��������� ���������� 3 �������: " << result << endl;
        }
        output.close();
    } else cout << endl << "error! �� ������� ������� ���� output.txt" << endl;
}
int main()
{
    srand(time(NULL)); // ��� ��������� ��������� ����� ������� � ������������� �����

    system("chcp 1251 > nul");
    cout << "������������ ������ �5 ������� 13.\n�����: ������� ������. �����-11.\n" << endl;

    /// ��������� ����� ������������������ �����

    int N = 10 + rand()%21; // ����� ������������������ �����

    N = 11; // ��� ������������
    /// ����� ������� ������������� ������������������

    cout << "������� 1. ������������� ������������������ ��������� �����." << endl << endl;
    cout << "����� ������������������: " << N << endl << endl;

    int user_choice;

    cout << "�������� ������ ������������� ������������������: " << endl;
    cout << "1. ������������� ���������� �������." << endl;
    cout << "2. ���� � ����������." << endl;
    cout << "��� �����: ";
    cin >> user_choice;

    /// �������� �� ������������ ����

    while (user_choice < 1 || user_choice > 2)
    {
        cout << "error!" << endl;
        cout << "�������� ������ ������������� ������������������: " << endl;
        cout << "1. ������������� ���������� �������." << endl;
        cout << "2. ���� � ����������." << endl;
        cout << "��� �����: ";
        cin >> user_choice;
    }

    /// ������������ ������������������ � ������ � � ���� input.txt
    /// � ��� �� ���������� ������� 1 (� ����� ���������� ���������� �� �����)

        switch (user_choice)
        {
            case 1: // ������������� ���������� �������
            {
                ex1_random(N);
                break;

            }
            case 2: // � ����������
            {
                ex1_from_keyboard(N);
                break;
            }
        }
        display_input();
        ex1();

    /// ���������� ������� �������
    cout << endl << "*** ���������� ������� 2: ***" << endl;
    cout << "� ���� output.txt �������� ������ ���������� ����� (��� �������������, ��� � �������������) �� ����� input.txt" << endl;
    ex2();
     /// ���������� �������� �������
    cout << endl << "*** ���������� ������� 3: ***" << endl << "����� ��������� �������� � ��������� �� ����� output.txt" << endl;
    ex3();

    return 0;
}
