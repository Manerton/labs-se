//ver 4 - 23.03.19

#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;
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

    ofstream input("input.txt"); // ��������� �������� �����-������
    if (input) // �������� ��������
    {
        input << N; // ������ � ���� ����� ������������������ �� 1 �����

        int chislo = -1;
        bool zero_flag = false; // ����� ����� ������ ������ ������� ������� - ������ true
        int summa = 0; // ����� ����� ����� ������� �������� ��������
        int count_of_elem = 0; // ���-�� ����� ����� ������� �������� ��������

        switch (user_choice)
        {
            case 1: // ������������� ���������� �������
            {
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
                for (int i = 1; i <= N; i++)
                {
                    if (chislo == 0) zero_flag = true;
                    chislo = rand_min + rand() % ostatok;
                    input << " " << chislo;
                    if (zero_flag)
                    {
                        summa += chislo;
                        count_of_elem++;
                    }

                }
                break;

            }
            case 2: // � ����������
            {
                cout << "������� �������� ��������� ������������������: " << endl;
                for (int i = 1; i <= N; i++)
                {
                    if (chislo == 0) zero_flag = true;
                    cout << "��� " << i << "-��: ";
                    cin >> chislo;
                    input << " " << chislo;
                    if (zero_flag)
                    {
                        summa += chislo;
                        count_of_elem++;
                    }
                }
                break;
            }
        }
        double sredne_arif;
        if (summa == 0) sredne_arif = 0; // �������� ������� �� ����, ���� � ������������������
        else sredne_arif = double(summa) / count_of_elem; // �� ���� �������� ��������
        cout << "��������� ���������� 1 �������: " << sredne_arif << endl;
        input.close();
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;

    /// ���������� ������� �������
    cout << endl << "*** ���������� ������� 2: ***" << endl << "� ���� output.txt �������� ������ ���������� ����� (��� �������������, ��� � �������������) �� ����� input.txt" << endl;
    ifstream input_ex2("input.txt"); // ��������� ��������� ���� ��� ������
    if (input_ex2) // �������� ��������
    {
        input_ex2 >> N; // ������ ����� ������������������
        int A, A_temp, kolvo_deleniy(0); // ���� ������ ����������� ���� ��� - ����� ����������

        ofstream output("output.txt"); // ��������� �������� ����� ��� 2 �������
        if (output) // �������� ��������
        {
            for (int i = 1; i <= N; i++)
            {
                input_ex2 >> A;
                if (A < 0) A_temp = -A; // ��� ��� �� ������� ������ ��� ���������� ��� ���������� ����� (������������� ����)
                else A_temp = A;

                while ((A_temp / 10) > 0 && kolvo_deleniy < 2)
                {
                    kolvo_deleniy++;
                    A_temp /= 10;
                }
                if (kolvo_deleniy == 1) // ���� ������ ����������� ���� ��� - ����� ����������
                {
                    output << A << " ";
                }
                kolvo_deleniy = 0;
            }
            output.close(); // ��������� �����-����
        } else cout << endl << "error! �� ������� ������� ���� output.txt" << endl;
        input_ex2.close();
        cout << "���������� ������� ������� ���� ���������." << endl;
    } else cout << endl << "error! �� ������� ������� ���� input.txt" << endl;

     /// ���������� �������� �������
    cout << endl << "*** ���������� ������� 3: ***" << endl << "����� ��������� �������� � ��������� �� ����� output.txt" << endl;
    ifstream output_ex3("output.txt"); // ��������� ��������� ���� ��� ������
    if (output_ex3) // �������� ��������
    {
        //���������, ����� ���� ����
        long file_size; // ������ �����
        output_ex3.seekg(0, ios::end); // ���������� ��������� ����� � ����� �����
        file_size = output_ex3.tellg(); // ���������� ������� ������� ��������� �����, ���� �� ����� ����, ������ ������ ��� � ���� �����, � ��� ������ ��� ���� ����
        if (file_size == 0) cout << "error! ���� ����.";
        else {
            output_ex3.seekg(0);
            int min, max, temp;
            output_ex3 >> min;
            max = min;
            while (!output_ex3.eof()) // ���� �� ��������� ����� �����
            {
                output_ex3 >> temp;
                if (output_ex3.eof()) break;
                if (temp < min) min = temp;
                if (temp > max) max = temp;
            }
            double result = (min + max) * 0.5;
            cout << "�������: " << min << endl;
            cout << "��������: " << max << endl;
            cout << "��������� ���������� 3 �������: " << result << endl;
        }
        output_ex3.close();
    } else cout << endl << "error! �� ������� ������� ���� output.txt" << endl;

    return 0;
}
