#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "color.h"
#include "demo.h"

using namespace std;

typedef vector<int> row;
typedef vector<row> matrix;

int N; // ���������� ������ �������

vector<int> mas; // �������� ������
int statesCount = 0; // ������� ���-�� ���������, ���������� ����� ����������

matrix states; // ��� ��������� ������� ��� ����������; (���-�� ����� - ������� ���������, ���-�� �������� N)
matrix counterStates; // �������� I � J ������� ���������, �������� d � ���� ���������

int state; //���� ��������� (���. � �-�� NextStep � PreviousStep)

void GenerateMas() // ���������� ������;
{
    srand(time(NULL));
    mas.resize(N);
    for(int i=0; i < N; i++)
    {
        mas[i] = rand() % 101 - 50; //�������������� ����� �� -50 �� 50;
    }
}

void GenerateMasFromKeyboard() // ���������� ������;
{
    cout << "������� �������� ��������� �������: " << endl;
    mas.resize(N);
    for (int i = 0; i < N; i++)
    {
        cout << "��� " << i+1 << "-��: ";
        cin >> mas[i];
        while (mas[i] > 50 || mas[i] < -50)
        {
            cout << "�� ����� ����� ��������� �� �������� [-50;50]" << endl;
            cout << "��� " << i+1 << "-��: ";
            cin >> mas[i];
        }

    }
}

bool GenerateMasFromFile(string filename) // ���������� ������;
{
    ifstream input("files\\" + filename + ".txt");
    if (input) // �������� ��������
    {
        mas.resize(N);
        for (int i = 0; i < N; i++)
        {
            input >> mas[i];
        }
        input.close();
        return true;
    } else return false;
}

void sposob_zapolneniya ()
{
    int choice;
    cout << "*** ����� ������� ���������� ***" << endl;
    cout << "1. ���������� � ����������." << endl;
    cout << "2. ���������� ���������� �������." << endl;
    cout << "3. ���������� �� �����." << endl;
    cout << "�������� ������ ����������: ";
    cin >> choice;
    while (choice < 1 || choice > 3)
    {
        cout << "������!" << endl;
        cout << "�������� ������ ����������: ";
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
    {
        GenerateMasFromKeyboard();
        break;
    }
    case 2:
    {
        GenerateMas();
        break;
    }
    case 3:

        string filename;
        cout << "������� �������� ����� ��� ���������� txt: ";
        cin >> filename;
        while (!GenerateMasFromFile(filename))
        {
            cout << endl << "������! �� ������� ������� ���� " << filename << endl;
            cout << "������� �������� ����� ��� ���������� txt: ";
            cin >> filename;
            GenerateMasFromFile(filename);
        }
        break;
    }
    system("cls");
}

void Init(int input_N) //������������� - ��������� ������� � ����������;
{
    N = input_N;
    sposob_zapolneniya();
    Sort(); // ���������
}

void SaveState(int d, int I, int J, int color) //�������, ����������� ��������� �������;
{

    states.push_back(mas);
    row temp (4,0);
    temp[0] = d; // ���������� �������������� ������� I ����� ���������
    temp[1] = I; // ���������� �������������� ������� J ����� ���������
    temp[2] = J; // ����
    temp[3] = color; // ����
    counterStates.push_back(temp);

    statesCount++;  //���������� �������� ���������� �������� ���������
}

void Sort() // ���������� � ���������� ��������� �������
{
    int d = N/2;
    SaveState (d, 0, 0, LightGray);
    while (d > 0)
    {
        for(int i=0; i<N-d; i++)
        {
            int j = i;

            SaveState (d,j,j+d,Green);

            while(j >= 0 && mas[j] > mas[j+d])
            {
                int temp = mas[j];
                mas[j] = mas[j+d];
                mas[j+d] = temp;

                SaveState (d,j,j+d,Yellow);

                j--;

                if (d == 1)
                {
                   SaveState (d,j,j+d,Green);
                }
            }
        }
        d /= 2;
    }
}

void PreviousStep() //��� ������ �� ���������� ���� ����������;
{
    if(state > 0)
        state--;
}

void NextStep()    //��� ������ �� ��������� ���� ����������;
{
    if(state < statesCount-1)
        state++;
}


void ShowDefaultState() // ���������� ������ �� ������� ����;
{

    int d = counterStates[0][0];

    SetColor(LightGreen,Black);
    cout << "�������� ������: " << endl;
    SetColor(LightGray,Black);

    for(int i=0; i < N; i++) cout << states[0][i] << " ";

    cout << "\n\n������� ��� d: " << d << endl;

    SetColor(LightGreen,Black);
    cout << "\n������� ������: " << endl;
    SetColor(LightGray,Black);
    for(int i=0; i < N; i++) cout << states[0][i] << " ";


}

void ShowState() // ����� ��������� ������� (��� ����������)
{

    string blank(50, ' ');
    int d = counterStates[state][0];
    int I = counterStates[state][1];
    int J = counterStates[state][2];
    int color = counterStates[state][3]; // ����

    gotoXY(0,12);
    if (d == 1)
    {
        cout << "��� ��� �������� d = 1, ��������� �";
        SetColor(LightGreen,Black);
        cout << " ���������� ���������." << endl;
        SetColor(LightGray,Black);
    } else
    {
        cout << "������� ��� d: " << d << blank << endl;
    }


    gotoXY(0,15);
    for(int i=0; i < N; i++)
    {
        if((i==I)||(i==J))
        {
            if (color == Yellow) SetColor(Black,Yellow);
            else SetColor(White,Green);
        }
        if ((state == 0) || (state == statesCount-1)) SetColor(LightGray,Black);
        cout << states[state][i];
        SetColor(LightGray,Black); //���������� "����������" ���� ������;

        cout << " ";
    }

    if(state == statesCount-1)
    {
        cout << endl;
        SetColor(Green,Black);
        cout << "\n������ ������������ (Array is sorted)" << endl;
        SetColor(LightGray,Black);
        cout << "\n������� 's', ���� �� ������ ��������� � ���� ��� ���� ����������" << endl;
        cout << "������� '\\/' ��� �������� � ������� ����..." << endl;
    }

}

// ������������
void Demonstrate(int input_N)
{
    ShowHelp(); // ����������
    Init(input_N); // ���� ������� N �� 5 �� 15
    system("cls");
    ShowHelp();
    ShowDefaultState(); // �������� ����������� ���������
    bool working = true;
    while(working)
    {
        switch(getch())
        {
        case 77:
            NextStep();
            break;
        case 75:
            PreviousStep();
            break;
        case 's':
            if (state == statesCount-1)
            {
                SaveToFile();
                cout << "\n����������� ���������� ���� ����� ���������� � ���� sortsteps.txt\n" << endl;
            }
            break;
        case 80:
            working = false;
            break;
        }

        ShowState(); // ������ �� ������� ���� ����������
    }
    states.clear(); // ������� ��� ��������� ������� ������������
	counterStates.clear();
	state = 0;
	statesCount = 0;
}

void SaveToFile()
{
    ofstream fout("files\\sortsteps.txt");

    for(int i=0; i<statesCount; i++)      //����� ���������
    {
        for(int j=0; j<N; j++)         //����� ��������
            fout << states[i][j] << " ";  //������ � ����;
        fout << endl;
    }
    fout.close();
}

void ShowHelp()
{
    system ("cls");
    SetColor(LightGreen,Black);
    cout << "�� ������� ����� ������������ ���������� �����: \n" << endl;
    SetColor(LightGray,Black);
    cout << "������� '->', ��� ������ ������� �� �����" << endl;
    cout << "������� '<-', ��� �������� ����������� ���� ���������� �������" << endl;
    cout << "������� '\\/', ���� �� ������ ��������� ����������\n" << endl;
    SetColor(White,Green);
    cout << "������� ����";
    SetColor(LightGray,Black);
    cout << ": ����������� � ������ ������ ��������." << endl;
    SetColor(Black,Yellow);
    cout << "Ƹ���� ����";
    SetColor(LightGray,Black);
    cout << ": ��������������� ��������.\n" << endl;
}

