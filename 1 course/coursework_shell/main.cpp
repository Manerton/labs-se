#include <iostream>
#include <windows.h>
#include <string>
#include <conio.h>
#include "color.h" // ������� �����
#include "teo.h"
#include "test.h"
#include <ctime>
#include "demo.h"

#define teo_ch 3
#define demo_ch 4
#define test_ch 5
#define admin_ch 6
#define exit_ch 7
#define login "work"
#define pass "work_asoiu"

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // ���������� ����
CONSOLE_SCREEN_BUFFER_INFO csbi; // �������������� ����

void ShowTitle()
{
	cout<<"\t   ������������ ��������������� ����������� �����������\n";
	cout<<"\t     �������� �������������� ���������� � ������������\n\n\n";
	cout<<"\t\t\t\t\t�������\n";
	cout<<"\t\t\t\t\t������������������ ������\n";
	cout<<"\t\t\t\t\t��������� ���������� � ����������\n\n\n\n";
	cout<<"\t\t\t�������� ������ �� ����������\n";
	cout<<"\t\t       '���������������� � �����������'\n";

	SetColor(Yellow, Black);

	cout<<"  ������-���������������� ��������� ������� ����������: ���������� �����\n\n\n\n\n";

	SetColor(LightGray,Black);

	cout<<"\t\t\t\t\t\t��������: ������� ������ �����-11\n\n";
	cout<<"\t\t\t\t\t\t\t  ������� �.�.\n\n\n";
	cout<<"\t\t\t     �.���������  2019 �.\t\n\n ";
	system("pause");
}

void ShowAdminHelp()
{
    system("cls");
    SetColor(LightGreen,Black);
    cout << "�� ������� ����� ��������������: \n\n" << endl;
    SetColor(LightGray,Black);
    SetColor(LightGreen,Black);
    cout << "���������� �� �������������� ����� � �������: " << endl;
    SetColor(LightGray,Black);
    cout << " � ��� ����, ����� ���������� ������� �� ��������� ��������, �������� �� ��������� ������ ��� [next_page]" << endl;
    cout << " � �� ����� �������� ������ ���� �� ����� 10 � �� ����� 20 �����." << endl;
    cout << " � �� ����� ������ ������ ���� �� ����� 60 ��������, ������� �� ������, ������������ �� ������." << endl;
    cout << " � ��� ����, ����� �������� ������ ����� ������, ��������� ����� ������ ���� [color]..[/color]\n" << endl;
    SetColor(LightGreen,Black);
    cout << "���������� �� �������������� ����� � ����� ��������: " << endl;
    SetColor(LightGray,Black);
    cout << " � �� ������ ������ ��������� ����� �������." << endl;
    cout << " � �� ���������� ������� ������� - �������� �������." << endl;
    cout << " � ������ ������� ������ ����� ��������� ����������." << endl;
    system("pause");
    system("cls");
}

bool correct_symbols (const char &symbol)
{
    string symbols = "!@#$%^&*()_-+=~1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
    return (symbols.find(symbol) != string::npos);
}

void pre_crypt (int ch)
{
    switch (ch)
    {
    case 1:
        decrypt("teo");
        system("files\\teo.txt");
        crypt("teo");
        remove("files\\teo.txt");
        break;
    case 2:
        decrypt("quest");
        system("files\\quest.txt");
        crypt("quest");
        remove("files\\quest.txt");
        break;
    }
}


void input_admin_menu (int &punkt, bool &ready)
{
    int button = getch();
    if (button == 224) button = getch();
    switch(button)
    {
    case '1':
        punkt = 3;
        ready = true;
        break;
    case '2':
        punkt = 4;
        ready = true;
        break;
    case '3':
        punkt = 5;
        ready = true;
    case 80:
        if (punkt != 5) punkt++;
        break;
    case 72:
        if (punkt != 3) punkt--;
        break;
    case 13:
        ready = true;
        break;
    default:
        input_admin_menu(punkt, ready);
        break;
    }
}

void display_admin_menu (const int &punkt)
{
    gotoXY(0,0);
    string blank(32, ' ');
    vector<string>vMenu = {
        "---------------------------------------------------",
        "|-------------- ���� �������������� --------------|",
        "---------------------------------------------------",
        "|1. ������� � ������������ ���� � �������         |",
        "|2. ������� � ������������ ���� � ����� ��������  |",
        "|3. ����� � ������� ����                          |",
        "---------------------------------------------------"
    };
    cout << "\n\n\n\n\n\n\n\n";
    int N = vMenu.size();
    for (int i = 0; i < N; i++)
    {
        if (i == punkt || i == 1)
        {
            int len = vMenu[i].length();
            cout << blank << vMenu[i][0];
            SetColor(White,Green);
            for (int j = 1; j < len-1; j++) cout << vMenu[i][j];
            SetColor(LightGray,Black);
            cout << vMenu[i][len-1] << endl;
        } else cout << blank << vMenu[i] << endl;
    }
}

void admin_menu ()
{
    system("cls");
    int punkt_menu = 3;
    while (punkt_menu != 5)
    {
        hide_cursor(false);
        display_admin_menu(punkt_menu); // ����� ���� � ������ ���
        bool ready = false;
        while (!ready)
        {
            input_admin_menu(punkt_menu, ready); // ����� ������ ����
            display_admin_menu(punkt_menu); // ��������� ������ ������
        }
        system("cls");
        switch (punkt_menu)
        {
        case 3:
            pre_crypt(1);
            break;
        case 4:
            pre_crypt(2);
            break;
        }
        system("cls");
    }
}


bool auth ()
{
    string input_login;
    string input_password;
    cout << "������� �����: ";
    cin >> input_login;
    char bukva = 0;
    int i = 0;
    cout << "\n������� ������: ";
    while (bukva != 13)
    {
        do {
            bukva = getch();
        } while (!correct_symbols(bukva) && bukva != 8 && bukva != 13);
        if (bukva == 8)
        {
            if (i > 0)
            {
                cout << "\b" << " " << "\b";
                input_password.pop_back();
                i--;
            }
        }
        if (bukva != 8 && bukva != 13)
        {
            input_password.push_back(bukva);
            cout << "*";
            i++;

        }
    }
    if (input_login == login && input_password == pass)
    {
        return true;
    } else
    {
        cout << "�� ����� ������������ ����� ��� ������" << endl;
        return false;
    }
}

void gotoXY( short x, short y ) // ������� �� ������ ������ �������
{
    COORD coord = {x, y};
    SetConsoleCursorPosition(console,coord);
}

void set_console_buffer_size(int Y)
{
    GetConsoleScreenBufferInfo(console,&csbi);
    csbi.dwSize.Y= Y;//10000 �����
    SetConsoleScreenBufferSize(console,csbi.dwSize);
}

void SetColor(int textcolor, int backgroundcolor)
{
   SetConsoleTextAttribute(console, (WORD)((backgroundcolor << 4) | textcolor));
}


void hide_cursor(bool hide)
{
    CONSOLE_CURSOR_INFO CCI;
    CCI.bVisible=hide;
    CCI.dwSize=1;
    SetConsoleCursorInfo(console,&CCI);
}

void teo ()
{
    system("color F0");
    SetColor(Black,White);
    text TEXT;
    creation_text(TEXT);
    teo_display(TEXT);
    system("color 07");
    SetColor(LightGray,Black);
}

void demo ()
{
    ShowHelp();
    cout << "������� ������ N: ";
    int N;
    cin >> N;
    while (N < 5 || N > 15)
    {
        cout << "������!" << endl;
        cout << "������� ������ N: ";
        cin >> N;
    }
	Demonstrate(N);
}

/// ����
void display_menu (const int &punkt)
{
    gotoXY(0,0);
    string blank(38, ' ');
    vector<string>vMenu = {
        "------------------------------------",
        "|---------- ������� ���� ----------|",
        "------------------------------------",
        "|1. ������� � ����� ������ ������  |",
        "|2. ������� � ����� ������������   |",
        "|3. ������ ������������            |",
        "|4. ������� � ����� �������������� |",
        "|5. ��������� ������ ���������     |",
        "------------------------------------"
    };
    cout << "\n\n\n\n\n\n\n";
    int N = vMenu.size();
    for (int i = 0; i < N; i++)
    {
        if (i == punkt || i == 1)
        {
            int len = vMenu[i].length();
            cout << blank << vMenu[i][0];
            SetColor(White,Green);
            for (int j = 1; j < len-1; j++) cout << vMenu[i][j];
            SetColor(LightGray,Black);
            cout << vMenu[i][len-1] << endl;
        } else cout << blank << vMenu[i] << endl;
    }
}
void input_menu (int &punkt, bool &ready)
{
    int button = getch();
    if (button == 224) button = getch();
    switch(button)
    {
    case '1':
        punkt = teo_ch;
        ready = true;
        break;
    case '2':
        punkt = demo_ch;
        ready = true;
        break;
    case '3':
        punkt = test_ch;
        ready = true;
        break;
    case '4':
        punkt = admin_ch;
        ready = true;
        break;
    case '5':
        punkt = exit_ch;
        ready = true;
        break;
    case 80:
        if (punkt != exit_ch) punkt++;
        break;
    case 72:
        if (punkt != teo_ch) punkt--;
        break;
    case 13:
        ready = true;
        break;
    default:
        input_menu(punkt, ready);
        break;
    }
}
/// ����� ����

int main()
{
    srand(time(0));
    system("mode con cols=114 lines=27");
    system("color 07");
    system("chcp 1251 > nul");
    ShowTitle();
    int punkt_menu = teo_ch;
    while (punkt_menu != exit_ch)
    {
        system("cls");
        hide_cursor(false);
        display_menu(punkt_menu); // ����� ���� � ������ ���
        bool ready = false;
        while (!ready)
        {
            input_menu(punkt_menu, ready); // ����� ������ ����
            display_menu(punkt_menu); // ��������� ������ ������
        }
        system("cls");
        switch (punkt_menu)
        {
        case teo_ch:
            teo();
            break;
        case demo_ch:
            demo();
            break;
        case test_ch:
            hide_cursor(true);
            test_menu();
            break;
        case admin_ch:
            hide_cursor(true);
            if ( auth() )
            {
                ShowAdminHelp();
                admin_menu();
            }
            break;
        }

    }
    return 0;
}
